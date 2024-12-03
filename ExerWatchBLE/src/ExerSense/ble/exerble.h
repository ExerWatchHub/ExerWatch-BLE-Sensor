#pragma once
#ifndef EXER_BLE_H
#define EXER_BLE_H

// a9a778fe-c8d9-4f73-af1d-f4e407e9a78c
// BLE related classes and call backs
#include "../headers/commons.h"
#include "../frequency_sync.h"

namespace ExerSense
{
  class ExerBLE
  {
  protected:
    ExerBLE(){};
    static ExerBLE *singleton_;

  public:
    static ExerBLE *GetInstance();
    String connectedDeviceName = "";
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    long last_sent_time = 0;
    int totalSentPackets = 0;
    FrequencySync *freqSync = new FrequencySync(PACKET_FREQUENCY_HZ);

    void setConnectedDeviceName(const char *name);
    void data_to_str(exer_data_t *data, char *data_str);
    void send_data_ble(exer_data_t *data);
    void send_chunk_ble();
    void init_ble();
    void print_ble_data(Point2D pos);
    void update_ble();

  private:
    BLEServer *pServer = NULL;
    BLECharacteristic *pTxCharacteristic;
    BLECharacteristic *pRxCharacteristic;
  };
}

ExerSense::ExerBLE *ExerSense::ExerBLE::singleton_ = nullptr;
ExerSense::ExerBLE *ExerSense::ExerBLE::GetInstance()
{
  if (singleton_ == nullptr)
  {
    singleton_ = new ExerBLE();
  }
  return singleton_;
}
#endif