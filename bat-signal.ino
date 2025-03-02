/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266

TODO:
    - [ ] LED lights indicating cloud connection status
        - [ ] RED for awaiting connection
        - [ ] GREEN indicating connection established
    - [ ] External bat_signal_on = false button (flip bat signal pin V)
    - [ ] External reset button
*/

#include "thingProperties.h"

// COMPILER MACROS
#define WARNING_LED_PIN D2
#define SUCCESS_LED_PIN D3
#define BAT_SIGNAL_ON HIGH
#define BAT_SIGNAL_OFF LOW
#define INITIAL_BAT_SIGNAL_STATE BAT_SIGNAL_OFF
#define BAT_SIGNAL_PIN D6

// FUNCTION PROTOTYPES
void configAndInitPins();
void initBatSignalStateInCloud();

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
  pinMode(WARNING_LED_PIN, OUTPUT);
  pinMode(SUCCESS_LED_PIN, OUTPUT);
  pinMode(BAT_SIGNAL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(WARNING_LED_PIN, HIGH);
  digitalWrite(SUCCESS_LED_PIN, LOW);

  // toggle the built-in led along with the bat signal, for debugging
  // relay turns on via HIGH voltage, LED turns on via LOW
  digitalWrite(LED_BUILTIN, !INITIAL_BAT_SIGNAL_STATE);
  digitalWrite(BAT_SIGNAL_PIN, INITIAL_BAT_SIGNAL_STATE);
}

void onCloudConnect() {
  Serial.println("CLOUD CONNECTED");
  digitalWrite(WARNING_LED_PIN, LOW);
  digitalWrite(SUCCESS_LED_PIN, HIGH);
}

void onCloudSync() {
  Serial.println("CLOUD SYNCED");
}

void onCloudDisconnect() {
  Serial.println("CLOUD DISCONNECTED");
  digitalWrite(SUCCESS_LED_PIN, LOW);
  digitalWrite(WARNING_LED_PIN, HIGH);
}

/* Responds to Arduino Cloud dashboard toggle switch. */
void onToggleBatSignal() {
  Serial.print("BAT SIGNAL ");
  Serial.println(bat_signal ? "ON" : "OFF");
  digitalWrite(BAT_SIGNAL_PIN, bat_signal);
  digitalWrite(LED_BUILTIN, !bat_signal);
}
