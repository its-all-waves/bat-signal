/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266

TODO:
    - [x] LED lights indicating cloud connection status
        - [x] YELLOW for awaiting connection
        - [x] GREEN indicating connection established
    - [x] External reset button
    - [ ] External bat_signal_on = false button (flip bat signal pin V)
    - [ ] External LED to mirror bat_signal_on to indicate change when no lamp connected
*/

#include "thingProperties.h"

// COMPILER MACROS
#define AWAITING_CLOUD_CONNECT_LED_PIN D2
#define CONNECTED_TO_CLOUD_LED_PIN D3
#define BUILTIN_LED_ON LOW
#define BAT_SIGNAL_PIN D6
#define BAT_SIGNAL_ON HIGH
#define BAT_SIGNAL_OFF LOW
#define BAT_SIGNAL_INITIAL_STATE BAT_SIGNAL_OFF
#define BAT_SIGNAL_ON_TIME_MS 2000
#define BAT_SIGNAL_MIRROR_LED_PIN D7

// FUNCTION PROTOTYPES
void configAndInitPins();

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
}

void configAndInitPins() {
  // configure pins as INPUT or OUTPUT
  pinMode(AWAITING_CLOUD_CONNECT_LED_PIN, OUTPUT);
  pinMode(CONNECTED_TO_CLOUD_LED_PIN, OUTPUT);
  pinMode(BAT_SIGNAL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BAT_SIGNAL_MIRROR_LED_PIN, OUTPUT);

  // set initial pin states
  digitalWrite(AWAITING_CLOUD_CONNECT_LED_PIN, HIGH);  // intially awaiting connection
  digitalWrite(CONNECTED_TO_CLOUD_LED_PIN, LOW);
  digitalWrite(LED_BUILTIN, !BAT_SIGNAL_INITIAL_STATE);    // toggle the built-in led along with the bat signal, for debugging
  digitalWrite(BAT_SIGNAL_PIN, BAT_SIGNAL_INITIAL_STATE);  // relay on = HIGH voltage, built-in LED on = LOW
  digitalWrite(BAT_SIGNAL_MIRROR_LED_PIN, LOW);
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
  digitalWrite(BAT_SIGNAL_PIN, BAT_SIGNAL_ON);
  digitalWrite(LED_BUILTIN, BUILTIN_LED_ON);
  digitalWrite(BAT_SIGNAL_MIRROR_LED_PIN, HIGH);
}

void turnOffBatSignal() {
  Serial.println("TURN OFF BAT SIGNAL");
  bat_signal = false;  // cloud lib handles syncing this with the cloud
  digitalWrite(BAT_SIGNAL_PIN, !BAT_SIGNAL_ON);
  digitalWrite(LED_BUILTIN, !BUILTIN_LED_ON);
  digitalWrite(BAT_SIGNAL_MIRROR_LED_PIN, LOW);
}

/* Responds to Arduino Cloud dashboard toggle switch. */
void onToggleBatSignal() {
  Serial.printf("CLOUD CHANGED `bat_signal` TO: %s\n", bat_signal ? "true" : "false");
  if (!bat_signal) return;  // only respond when bat_signal changes false -> true
  turnOnBatSignal();
  delay(BAT_SIGNAL_ON_TIME_MS);
  turnOffBatSignal();
}
