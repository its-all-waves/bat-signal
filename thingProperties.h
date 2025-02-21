#include "WiFiConnectionHandler.h"
#include "ArduinoIoTCloudTCP.h"
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "secrets.h"

void initBatSignalStateInCloud();
void onToggleBatSignal();
boolean bat_signal_on;

void initThingProperties() {
    #if defined(HAS_TCP)
        ArduinoCloud.addProperty(bat_signal_on, Permission::Write).onUpdate(onToggleBatSignal);

    #if !defined(BOARD_HAS_SECURE_ELEMENT)
        ArduinoCloud.setBoardId(DEVICE_ID);
        ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET_KEY);
    #endif // BOARD_HAS_SECURE_ELEMENT
    #endif // HAS_TCP
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PW);
