#include <ArduinoIoTCloud.h>
#include "ArduinoIoTCloudTCP.h"
#include <Arduino_ConnectionHandler.h>
#include "WiFiConnectionHandler.h"
#include "secrets.h"

void onCloudConnect();
void onCloudSync();
void onCloudDisconnect();
void onToggleBatSignal();

bool bat_signal;

void initThingProperties() {
  ArduinoCloud.setBoardId(DEVICE_ID);
  ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET_KEY);

  // allow this Thing to read and write updates to the `bat_signal` bool in the Cloud
  ArduinoCloud.addProperty(bat_signal, Permission::ReadWrite).onUpdate(onToggleBatSignal);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onCloudConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onCloudSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onCloudDisconnect);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PW);
