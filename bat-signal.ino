/*
Board: NodeMCU 1.0 (ESP-12E) ESP8266
*/

#include "thingProperties.h"

// COMPILER MACROS
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
    while(!Serial);
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
    pinMode(BAT_SIGNAL_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(BAT_SIGNAL_PIN, INITIAL_BAT_SIGNAL_STATE);
    // relay turns on via HIGH voltage, LED turns on via LOW
    digitalWrite(LED_BUILTIN, !INITIAL_BAT_SIGNAL_STATE);
}

/* Sets the initial state of the bat signal (on | off) in Arduino cloud. */
void initBatSignalStateInCloud() { /* TODO */ }

/* Responds to Arduino Cloud dashboard toggle switch. */
void onToggleBatSignal() {
    Serial.print("Bat Signal ");
    Serial.println(bat_signal);
    digitalWrite(BAT_SIGNAL_PIN, bat_signal);
    // toggle the built in led for debugging
    digitalWrite(LED_BUILTIN, !bat_signal);
}
