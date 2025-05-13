/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266

TODO:
  - [ ] Block bat signal while in IM_COMING state
  - [ ] Make the LED more dyanmic
  - [x] Implement a state machine
  - [x] Swap 3 LEDs for single RGB LED
   - [x] LED lights indicating cloud connection status
        - [x] YELLOW for awaiting connection
        - [x] GREEN indicating connection established
    - [x] External reset button
    - [x] External LED to mirror bat_signal_on (allows device user to see if cloud connection OK without lamp attached) 
*/

#include "thingProperties.h"
#include "RGBLed.h"

// PIN ASSIGNMENTS

#define RED_LED_PIN D1  // single RGB LED
#define GREEN_LED_PIN D2
#define BLUE_LED_PIN D3

#define BAT_SIGNAL_RELAY_PIN D6
#define IM_COMING_BUTTON_PIN D4

// TIMING SETTINGS

#define SECOND *1000

#define BAT_SIGNAL_TIMEOUT_MS 30 SECOND
#define IM_COMING_TIMEOUT_MS 30 SECOND  // 30 seconds ideal

#define DISCONNECTED_LED_FADE_DUR_MS 1 SECOND
#define CONNECTED_LED_FADE_DUR_MS 3 SECOND
#define BAT_SIGNAL_LED_FADE_DUR_MS 0

// PROTOTYPES

void imComingButtonInterrupt();

// TYPES

enum State {
  DISCONNECTED,    // 0
  CONNECTED_IDLE,  // 1
  BAT_SIGNAL_ON,   // 2
  IM_COMING,       // 3
};

enum Event {
  NO_EVENT = 0,              // 0
  DISCONNECTED_FROM_CLOUD,   // 1
  CONNECTED_TO_CLOUD,        // 2
  RECEIVED_BAT_SIGNAL,       // 3
  PRESSED_IM_COMING_BUTTON,  // 4
  IM_COMING_TIMED_OUT,       // 5
  BAT_SIGNAL_TIMED_OUT,      // 6
};

// DEFINITIONS...

void configurePins() {
  // NOTE: LED pins are configured by RGBLed class
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BAT_SIGNAL_RELAY_PIN, OUTPUT);
  pinMode(IM_COMING_BUTTON_PIN, INPUT_PULLUP);  // PULLUP = use internal pullup resistor so pressing the button doesn't short to ground
  attachInterrupt(digitalPinToInterrupt(IM_COMING_BUTTON_PIN), imComingButtonInterrupt, RISING);
}

// LED

RGBLed led = RGBLed(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);
RGBLedFade disconnectedFade = RGBLedFade(LED_OFF, YELLOW, true, DISCONNECTED_LED_FADE_DUR_MS);
RGBLedFade connectedFade = RGBLedFade(LED_OFF, GREEN, true, CONNECTED_LED_FADE_DUR_MS);
RGBLedFade batSignalFade = RGBLedFade(CYAN, MAGENTA, true, BAT_SIGNAL_LED_FADE_DUR_MS, 30);

void disconnectedFromCloudLedOn() {
  led.setFade(&disconnectedFade);
}

void connectedToCloudLedOn() {
  led.setFade(&connectedFade);
}

void builtInLedOn() {
  digitalWrite(LED_BUILTIN, LOW);
}

void batSignalLedOn() {
  builtInLedOn();  // built-in LED also mirrors bat signal
  led.setFade(&batSignalFade);
}

void imComingLedOn() {
  led.emit(MAGENTA);
}

// BAT SIGNAL RELAY

void batSignalOn() {
  Serial.println("TURN ON BAT SIGNAL");
  digitalWrite(BAT_SIGNAL_RELAY_PIN, HIGH);
}

void batSignalOff() {
  Serial.println("TURN OFF BAT SIGNAL");
  digitalWrite(BAT_SIGNAL_RELAY_PIN, LOW);
}

// STATE

State state = DISCONNECTED;
Event event = NO_EVENT;

uint32_t batSignalReceivedTimeMs = 0;
uint32_t imComingButtonPressedTimeMs = 0;

void zeroOutState() {
  bat_signal = false;
  im_coming = false;
  batSignalReceivedTimeMs = 0;
  imComingButtonPressedTimeMs = 0;
  batSignalOff();
}

State setStateIdleConnected() {
  zeroOutState();
  connectedToCloudLedOn();
  return CONNECTED_IDLE;
}

State setStateDisconnected() {
  zeroOutState();
  disconnectedFromCloudLedOn();
  return DISCONNECTED;
}

State setStateBatSignalOn() {
  batSignalReceivedTimeMs = millis();
  batSignalOn();
  batSignalLedOn();
  return BAT_SIGNAL_ON;
}

State setStateImComing() {
  imComingButtonPressedTimeMs = millis();
  bat_signal = false;
  im_coming = true;
  imComingLedOn();
  return IM_COMING;
}

void transitionState() {
  State nextState = state;
  switch (state) {
    case DISCONNECTED:
      if (event == CONNECTED_TO_CLOUD) {
        nextState = setStateIdleConnected();
      }
      break;
    case CONNECTED_IDLE:
      if (event == RECEIVED_BAT_SIGNAL) {
        nextState = setStateBatSignalOn();
        break;
      }
      if (event == DISCONNECTED_FROM_CLOUD) {
        nextState = setStateDisconnected();
        break;
      }
      break;
    case BAT_SIGNAL_ON:
      // bat signal stays on until I'm coming button is pressed
      // or eventually times out
      if (event == PRESSED_IM_COMING_BUTTON) {
        nextState = setStateImComing();
        break;
      }
      if (event == BAT_SIGNAL_TIMED_OUT) {
        nextState = setStateIdleConnected();
        break;
      }
      break;
    case IM_COMING:
      if (event == IM_COMING_TIMED_OUT) {
        nextState = setStateIdleConnected();
      }
      break;
  }
  Serial.printf("STATE: %d -> %d :: EVENT: %d\n", state, nextState, event);
  state = nextState;
}

// LOCAL EVENT EMITTERS

// IRAM_ATTR: see https://arduino-esp8266.readthedocs.io/en/latest/reference.html#interrupts
void IRAM_ATTR imComingButtonInterrupt() {
  event = PRESSED_IM_COMING_BUTTON;
}

void dispatchTimedEvents() {
  switch (state) {
    case BAT_SIGNAL_ON:
      if (millis() - batSignalReceivedTimeMs >= BAT_SIGNAL_TIMEOUT_MS) {
        event = BAT_SIGNAL_TIMED_OUT;
      }
      break;
    case IM_COMING:
      if (millis() - imComingButtonPressedTimeMs >= IM_COMING_TIMEOUT_MS) {
        event = IM_COMING_TIMED_OUT;
      }
      break;
  }
}

// CLOUD EVENT HANDLERS WITH LOCAL EVENT EMITTERS

void onCloudConnect() {
  Serial.println("CLOUD CONNECTED");
}

void onCloudSync() {
  Serial.println("CLOUD SYNCED");
  event = CONNECTED_TO_CLOUD;
}

void onCloudDisconnect() {
  Serial.println("CLOUD DISCONNECTED");
  event = DISCONNECTED_FROM_CLOUD;
}

/* Responds to Arduino Cloud dashboard toggle switch. */
void onCloudToggleBatSignal() {
  Serial.printf("CLOUD CHANGED `bat_signal` TO: %s\n", bat_signal ? "true" : "false");
  if (!bat_signal) return;
  event = RECEIVED_BAT_SIGNAL;
}

// PROGRAM

void update() {
  ArduinoCloud.update();
  led.updateFade();
}

void setup() {
  Serial.begin(9600);
  delay(1500);  // wait for a Serial Monitor w/o blocking if not found
  configurePins();
  disconnectedFromCloudLedOn();
  setDebugMessageLevel(DBG_INFO);
  initThingProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  update();
  if (event != NO_EVENT) {
    transitionState();
    event = NO_EVENT;
  }
  dispatchTimedEvents();
}