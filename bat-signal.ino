/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266

TODO:
  - [ ] Block bat signal while in IM_COMING state
  - [ ] Make the LED more dyanmic
  - [x] Swap 3 LEDs for single RGB LED
   - [x] LED lights indicating cloud connection status
        - [x] YELLOW for awaiting connection
        - [x] GREEN indicating connection established
    - [x] External reset button
    - [x] External LED to mirror bat_signal_on (allows device user to see if cloud connection OK without lamp attached) 
*/

#include "thingProperties.h"
#include "RGBLed.h"

// PREPROCESSOR MACROS
#define RED_LED_PIN D1  // single RGB LED
#define GREEN_LED_PIN D2
#define BLUE_LED_PIN D7
#define AWAITING_CLOUD_CONNECT_LED_PIN RED_LED_PIN
#define CONNECTED_TO_CLOUD_LED_PIN GREEN_LED_PIN
#define BAT_SIGNAL_MIRROR_LED_PIN BLUE_LED_PIN
#define IM_COMING_LED_PIN RED_LED_PIN  // reuse the awaiting cloud connection LED, since it's idle when connection OK

#define BAT_SIGNAL_RELAY_PIN D6
#define BAT_SIGNAL_ON_TIME_MS 2000

#define IM_COMING_BUTTON_PIN D4
#define IM_COMING_DELAY_MS 2000

#define LED_BUILTIN_ON LOW

// FUNCTION PROTOTYPES
void configAndInitPins();
void imComing();
void onImComing();

RGBLed led = RGBLed(RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN);

/* Runs once at boot. */
void setup() {
  Serial.begin(9600);
  delay(1500);  // wait for a Serial Monitor w/o blocking if not found

  configAndInitPins();
  setDebugMessageLevel(DBG_INFO);

  initThingProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.printDebugInfo();

  led.emit(YELLOW);
}

enum State {
  DISCONNECTED,
  IDLE,
  BAT_SIGNAL_ON,
  IM_COMING,
};

volatile State state = DISCONNECTED;

uint16_t batSignalTimer = 0;
volatile uint16_t imComingTimer = 0;

/* Runs forever after `setup()`. */
void loop() {
  ArduinoCloud.update();

  uint16_t now = millis();

  // set LED according to state
  switch (state) {
    case DISCONNECTED:
      awaitingCloudConnectionLedOn();
      break;
    case IDLE:
      connectedToCloudLedOn();
      break;
    case BAT_SIGNAL_ON:
      batSignalOn();
      batSignalLedOn();
      if (now - batSignalTimer >= BAT_SIGNAL_ON_TIME_MS) {
        batSignalTimer = 0;
        batSignalOff();
        state = IDLE;
        bat_signal = false;
      };
      break;
    case IM_COMING:
      imComingLedOn();
      if (now - imComingTimer >= IM_COMING_DELAY_MS) {
        imComingTimer = 0;
        state = IDLE;
        im_coming = false;
      }
      break;
  }
}

void configAndInitPins() {
  // configure pins as INPUT or OUTPUT
  pinMode(AWAITING_CLOUD_CONNECT_LED_PIN, OUTPUT);
  pinMode(CONNECTED_TO_CLOUD_LED_PIN, OUTPUT);
  pinMode(BAT_SIGNAL_MIRROR_LED_PIN, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(BAT_SIGNAL_RELAY_PIN, OUTPUT);
  pinMode(IM_COMING_BUTTON_PIN, INPUT_PULLUP);  // PULLUP = use internal pullup resistor so pressing the button doesn't short to ground

  attachInterrupt(digitalPinToInterrupt(IM_COMING_BUTTON_PIN), imComing, FALLING);

  // set initial pin state
  digitalWrite(AWAITING_CLOUD_CONNECT_LED_PIN, HIGH);  // intially awaiting connection
}

// INTERRUPT SERVICE ROUTINE
// IRAM_ATTR: see https://arduino-esp8266.readthedocs.io/en/latest/reference.html#interrupts
/** Triggered by a button press. */
void IRAM_ATTR imComing() {
  im_coming = true;
  state = IM_COMING;
  imComingTimer = millis();
}

// LED CONTROL
void awaitingCloudConnectionLedOn() {
  led.emit(YELLOW);
}

void connectedToCloudLedOn() {
  led.emit(GREEN);
}

void batSignalLedOn() {
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);  // built-in LED also mirrors bat signal
  led.emit(BLUE);
}

void imComingLedOn() {
  led.emit(MAGENTA);
}

void batSignalOn() {
  Serial.printf("TURN ON BAT SIGNAL FOR %d SECONDS\n", BAT_SIGNAL_ON_TIME_MS / 1000);
  digitalWrite(BAT_SIGNAL_RELAY_PIN, HIGH);
}

void batSignalOff() {
  Serial.println("TURN OFF BAT SIGNAL");
  digitalWrite(BAT_SIGNAL_RELAY_PIN, LOW);
}

// CLOUD EVENT HANDLERS
void onCloudConnect() {
  Serial.println("CLOUD CONNECTED");
  state = IDLE;
}

void onCloudSync() {
  Serial.println("CLOUD SYNCED");
}

void onCloudDisconnect() {
  Serial.println("CLOUD DISCONNECTED");
  state = DISCONNECTED;
}

/* Responds to Arduino Cloud dashboard toggle switch. */
void onToggleBatSignal() {
  Serial.printf("CLOUD CHANGED `bat_signal` TO: %s\n", bat_signal ? "true" : "false");
  if (!bat_signal) return;  // only respond when bat_signal changes false -> true
  state = BAT_SIGNAL_ON;
  batSignalTimer = millis();
}
