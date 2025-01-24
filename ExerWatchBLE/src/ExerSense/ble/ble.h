#pragma once
#ifndef BLE_H
#define BLE_H

#include "exerble.h"
#include "char_callbacks.h"
#include "server_callbacks.h"
#include "../frequency_sync.h"

namespace ExerSense
{
  ExerBLECharacteristicCallbacks *exerCharCallbacks;
  WatchBLECharacteristicCallbacks *watchCharCallbacks;
  ExerBLEServerCallbacks *serverCallbacks;

  void ExerBLE::create_service(BLEServer *pServer, const char *uuid, const char* txCharUUID, const char* rxCharUUID, BLECharacteristicCallbacks *rxCharCallback)
  {
    BLEService *pService = pServer->createService(uuid);
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    // Without the following line, the service UUID won't be advertised even though it was specified in the service creation
    // https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/examples/BLE_server/BLE_server.ino#L34
    pAdvertising->addServiceUUID(uuid);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    if (txCharUUID != NULL)
    {
      // Create a BLE Characteristic
      pTxCharacteristic = pService->createCharacteristic(txCharUUID, BLECharacteristic::PROPERTY_NOTIFY);
      pTxCharacteristic->addDescriptor(new BLE2902());
    }
    if(rxCharUUID != NULL){
      pRxCharacteristic = pService->createCharacteristic(rxCharUUID, BLECharacteristic::PROPERTY_WRITE);
      if(rxCharCallback != nullptr){
        pRxCharacteristic->setCallbacks(rxCharCallback);
      }
    }

    delay(100); // This seems to be absolutely needed or nothing will work... Taken from the official test_ble.cpp code, go figure...

    pService->start();     // Start the service
    pAdvertising->start(); // Start advertising
  }

  void ExerBLE::init_ble()
  {
    exerCharCallbacks = new ExerBLECharacteristicCallbacks();
    watchCharCallbacks = new WatchBLECharacteristicCallbacks();
    serverCallbacks = new ExerBLEServerCallbacks();
    last_sent_time = millis();
    /// SETUP BLE SERVER
    // Create the BLE Device
    uint64_t chipid = ESP.getEfuseMac();
    blename = "ExerWatch" + String((uint32_t)(chipid >> 32), HEX);

    BLEDevice::init(blename.c_str());

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(serverCallbacks);

    // Create the BLE Service
    create_service(pServer, EXER_BLE_SERVICE_UUID, EXER_CHARACTERISTIC_UUID_TX, EXER_CHARACTERISTIC_UUID_RX, exerCharCallbacks);
    create_service(pServer, WATCH_SERVICE_UUID, WATCH_CHARACTERISTIC_UUID_TX, WATCH_CHARACTERISTIC_UUID_RX, watchCharCallbacks);
    // BLEService *pService = pServer->createService(UART_SERVICE_UUID);

    Serial.println("Waiting for a client connection to send Acc data...");
  }

  void ExerBLE::setConnectedDeviceName(const char *name)
  {
    connectedDeviceName = name;
  }

  /// SEND DATA via BLE
  void ExerBLE::send_data_ble(exer_data_t *data)
  {
    data_to_str(data, packet_str);
    // Serial.print(packet_str);
    if (!deviceConnected)
    {
      return;
    }
    pTxCharacteristic->setValue((uint8_t *)packet_str, strlen(packet_str));
    pTxCharacteristic->notify();
    totalSentPackets++;

    freqSync->tick();      // Update the number of sent packets and also the current delay and the required delay
    freqSync->syncDelay(); // Actually delay the code for the next time
  }

  void ExerBLE::data_to_str(exer_data_t *data, char *data_str)
  {
    sprintf(data_str, "%6.3f, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f\n",
            data->elapsed,
            data->acc.x * scale, data->acc.y * scale, data->acc.z * scale,
            data->gyr.x * scale, data->gyr.y * scale, data->gyr.z * scale);
  }

  void ExerBLE::send_chunk_ble()
  {
    int sent = 0;
    while (true)
    {
      dump_idx += 1;
      if (dump_idx >= BUFFER_SIZE - 1)
      {
        dump_idx = 0;
      }
      if (dump_idx >= idx)
      {
        return; // We don't want to send old data so we stop when the dump_idx reaches the current data idx
      }
      send_data_ble(&(buffer[idx]));
      sent++;
      if (sent > CHUNK_SIZE)
      {
        return; // We'll stop here if we have sent enough data for this chunk
      }
      last_sent_time = millis();

      delay(20); // Lets not tax the BLE - give it some rest!
    }
  }

  /// SCreen routines
  void ExerBLE::print_ble_data(Point2D pos)
  {
    char buf_size_str[16];

    screen_buf->setTextSize(1.4);
    screen_buf->setTextColor(TFT_SKYBLUE);
    screen_buf->setCursor(pos.x, pos.y);
    screen_buf->printf("%s", blename);
    screen_buf->setTextSize(fontSize);
    int xOff = blename.length() * fontSize * 7;
    screen_buf->setCursor(pos.x + xOff, pos.y);

    if (!deviceConnected && !oldDeviceConnected)
    {
      screen_buf->setTextColor(TFT_RED);
      screen_buf->printf("DISCONNECTED");
    }
    else
    {
      screen_buf->setTextColor(TFT_GREEN);
      if (!deviceConnected && oldDeviceConnected)
      {
        screen_buf->printf("DISCONNECTING...");
      }
      else if (deviceConnected && !oldDeviceConnected)
      {
        screen_buf->printf("CONNECTING...");
      }
      else
      {
        screen_buf->printf(connectedDeviceName.isEmpty() ? "CONNECTED" : connectedDeviceName.c_str());
      }
    }
    xOff += 12 * fontSize * 7;
    screen_buf->setTextColor(TFT_DARKGRAY);
    // screen_buf->setCursor(pos.x + xOff, pos.y);
    // screen_buf->printf("%d", totalSentPackets);
    pos.y += lineHeight + 2;
    screen_buf->setCursor(pos.x, pos.y);
    bytes_to_str(full ? sizeof(buffer) : sizeof(buffer[0]) * (idx + 1), buf_size_str);

    pos.y += lineHeight + 2;
    screen_buf->setCursor(pos.x, pos.y);
    int filled_buf = full ? BUFFER_SIZE : idx + 1;
    screen_buf->printf("Buf idx: %d/%d (%s)", idx, BUFFER_SIZE, buf_size_str);

    pos.y += lineHeight + 2;
    screen_buf->setCursor(pos.x, pos.y);
    screen_buf->printf("Freq: %2.1f", freqSync->currentFrequency);
    // Serial.print(message);
  }

  void ExerBLE::update_ble()
  {
    if (paused)
      return;
    ImuData data = StickCP2.Imu.getImuData();
    buffer[idx++] = (exer_data_t){.timestamp = millis(), .seq = seq, .total_time = total_time, .acc = (Point3D)data.accel, .gyr = (Point3D)data.gyro};
    if (idx >= BUFFER_SIZE - 1)
    {
      full = true;
      idx = 0;
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
      delay(200); // give the bluetooth stack the chance to get things ready
      pServer->getAdvertising()->start();
      // pServer->startAdvertising(); // restart advertising
      Serial.println("start advertising");
      oldDeviceConnected = false;
      deviceConnected = false;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
    }
    // At the moment DUMP_WAIT_TIME is 0 so we are not waiting at all
    // But if we want to only dump chunks of data everu once in a while we can increase DUMP_WAIT_TIME
    if (DUMP_WAIT_TIME > 0 && millis() - last_sent_time < DUMP_WAIT_TIME)
    {
      return;
    }

    // Might need to make sure this is actually the last previous data, especially when it loops back from BUFFER_SIZE-1 to 0
    if (idx > 0)
    {
      send_data_ble(&(buffer[idx - 1]));
    }
  }
}

#endif