#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "secrets.h"

void onCloudConnect();
void onCloudSync();
void onCloudDisconnect();
void onCloudToggleBatSignal();
void onToggleImComing();
bool bat_signal = false;
bool im_coming = false;

void initThingProperties() {
  ArduinoCloud.setBoardId(DEVICE_ID);
  ArduinoCloud.setSecretDeviceKey(DEVICE_SECRET_KEY);

  // allow this Thing to read and write updates to the `bat_signal` bool in the Cloud
  ArduinoCloud.addProperty(bat_signal, Permission::ReadWrite).onUpdate(onCloudToggleBatSignal);
  ArduinoCloud.addProperty(im_coming, Permission::Read);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onCloudConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onCloudSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onCloudDisconnect);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(WIFI_SSID, WIFI_PW);
