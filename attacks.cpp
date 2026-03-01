#include <Arduino.h>
#include "attacks.h"

void startWifiScan() {
  Serial.println("Wifi scan started");
}

void stopWifiScan() {
  Serial.println("Wifi scan stopped");
}

void startPacketMonitor() {
  Serial.println("Packet monitor started");
}

void stopPacketMonitor() {
  Serial.println("Packet monitor stopped");
}

void startHandshakeCapture() {
  Serial.println("Handshake capture started");
}

void stopHandshakeCapture() {
  Serial.println("Handshake capture stopped");
}
