#include <ArduinoIoTCloud.h>
#include "ArduinoIoTCloudTCP.h"
#include <Arduino_ConnectionHandler.h>
#include "WiFiConnectionHandler.h"
#include "secrets.h"

void initBatSignalStateInCloud();
void onToggleBatSignal();
bool bat_signal;

void initThingProperties() {
    #if defined(HAS_TCP)
        ArduinoCloud.addProperty(bat_signal, Permission::Write).onUpdate(onToggleBatSignal);

    #if !defined(BOARD_HAS_SECURE_ELEMENT)
        ArduinoCloud.setBoardId(DEVICE_ID);
        ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET_KEY);
    #endif // BOARD_HAS_SECURE_ELEMENT
    #endif // HAS_TCP
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PW);
