#pragma once
#ifndef BLE_SERVER_CB_H
#define BLE_SERVER_CB_H


class ExerBLEServerCallbacks : public BLEServerCallbacks
{
public:
  void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param)
  {
    // param->
    // auto devices = pServer->getPeerDevices(false);
    auto exerBLE = ExerSense::ExerBLE::GetInstance();
    exerBLE->deviceConnected = true;
    Serial.println("BLE Connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    auto exerBLE = ExerSense::ExerBLE::GetInstance();
    exerBLE->deviceConnected = false;
    Serial.println("BLE Disconnected");
  }
};
#endif