#include "thingProperties.h"

#define LED_BUILTIN_ON false
#define LED_BUILTIN_OFF true
#define INITIAL_BAT_SIGNAL_STATE LED_BUILTIN_OFF
#define BAT_SIGNAL_PIN LED_BUILTIN

void setup() {
    Serial.begin(9600);
    while(!Serial);
    setDebugMessageLevel(DBG_INFO);

    // initialize the LED pin as an output
    pinMode(BAT_SIGNAL_PIN, OUTPUT);

    // initialize LED state (on/off)
    digitalWrite(BAT_SIGNAL_PIN, INITIAL_BAT_SIGNAL_STATE);

    initThingProperties();

    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    ArduinoCloud.printDebugInfo();
}

void loop() {
    ArduinoCloud.update();
}

void initBatSignalStateInCloud() { /* TODO */ }

void onToggleBatSignal() {
    Serial.print("Bat Signal ");
    Serial.println(bat_signal_on);
    digitalWrite(BAT_SIGNAL_PIN, !bat_signal_on);
}
