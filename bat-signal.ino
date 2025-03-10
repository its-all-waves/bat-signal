/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266

TODO:
    - [x] LED lights indicating cloud connection status
        - [x] YELLOW for awaiting connection
        - [x] GREEN indicating connection established
    - [x] External reset button
    - [x] External LED to mirror bat_signal_on (allows device user to see if cloud connection OK without lamp attached) 
*/

#include "thingProperties.h"

// PREPROCESSOR MACROS
#define AWAITING_CLOUD_CONNECT_LED_PIN D1
#define CONNECTED_TO_CLOUD_LED_PIN D2
#define LED_BUILTIN_ON LOW
#define BAT_SIGNAL_PIN D6
#define BAT_SIGNAL_ON_TIME_MS 2000
#define BAT_SIGNAL_MIRROR_LED_PIN D7
#define IM_COMING_BUTTON_PIN D4
#define IM_COMING_LED_PIN D1  // reuse the awaiting cloud connection LED, since it's idle when connection OK

// FUNCTION PROTOTYPES
void configAndInitPins();
void imComing();
void onImComing();

/* Runs once at boot. */
void setup() {
  Serial.begin(9600);
  delay(1500);  // wait for a Serial Monitor w/o blocking if not found

  configAndInitPins();
  setDebugMessageLevel(DBG_INFO);

  initThingProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.printDebugInfo();
}

/* Runs forever after `setup()`. */
void loop() {
  ArduinoCloud.update();
  onImComing();
}

void configAndInitPins() {
  // configure pins as INPUT or OUTPUT
  pinMode(AWAITING_CLOUD_CONNECT_LED_PIN, OUTPUT);
  pinMode(CONNECTED_TO_CLOUD_LED_PIN, OUTPUT);
  pinMode(BAT_SIGNAL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BAT_SIGNAL_MIRROR_LED_PIN, OUTPUT);
  pinMode(IM_COMING_BUTTON_PIN, INPUT_PULLUP);  // PULLUP = use internal pullup resistor so pressing the button doesn't short to ground

  attachInterrupt(digitalPinToInterrupt(IM_COMING_BUTTON_PIN), imComing, FALLING);

  // set initial pin state
  digitalWrite(AWAITING_CLOUD_CONNECT_LED_PIN, HIGH);  // intially awaiting connection
}

// INTERRUPT SERVICE ROUTINE (ISR)
// IRAM_ATTR: see https://arduino-esp8266.readthedocs.io/en/latest/reference.html#interrupts
void IRAM_ATTR imComing() {
  im_coming = true;
}

void onImComing() {
  if (!im_coming) return;
  Serial.println("I'm Coming!");
  digitalWrite(IM_COMING_LED_PIN, HIGH);
  delay(2000);
  digitalWrite(IM_COMING_LED_PIN, LOW);
  im_coming = false;
}

// CLOUD EVENT HANDLERS
void onCloudConnect() {
  Serial.println("CLOUD CONNECTED");
  digitalWrite(AWAITING_CLOUD_CONNECT_LED_PIN, LOW);
  digitalWrite(CONNECTED_TO_CLOUD_LED_PIN, HIGH);
}

void onCloudSync() {
  Serial.println("CLOUD SYNCED");
}

void onCloudDisconnect() {
  Serial.println("CLOUD DISCONNECTED");
  digitalWrite(CONNECTED_TO_CLOUD_LED_PIN, LOW);
  digitalWrite(AWAITING_CLOUD_CONNECT_LED_PIN, HIGH);
}

void turnOnBatSignal() {
  Serial.printf("TURN ON BAT SIGNAL FOR %d SECONDS\n", BAT_SIGNAL_ON_TIME_MS / 1000);
  bat_signal = true;
  digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);
  digitalWrite(BAT_SIGNAL_MIRROR_LED_PIN, HIGH);
  digitalWrite(BAT_SIGNAL_PIN, HIGH);
}

void turnOffBatSignal() {
  Serial.println("TURN OFF BAT SIGNAL");
  bat_signal = false;  // cloud lib handles syncing this with the cloud
  digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
  digitalWrite(BAT_SIGNAL_MIRROR_LED_PIN, LOW);
  digitalWrite(BAT_SIGNAL_PIN, LOW);
}

/* Responds to Arduino Cloud dashboard toggle switch. */
void onToggleBatSignal() {
  Serial.printf("CLOUD CHANGED `bat_signal` TO: %s\n", bat_signal ? "true" : "false");
  if (!bat_signal) return;  // only respond when bat_signal changes false -> true
  turnOnBatSignal();
  delay(BAT_SIGNAL_ON_TIME_MS);
  turnOffBatSignal();
}

void onToggleImComing() {
  Serial.printf("CLOUD CHANGED `bat_signal` TO: %s\n", bat_signal ? "true" : "false");
}
