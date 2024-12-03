#pragma once
#ifndef BLE_CHAR_CB_H
#define BLE_CHAR_CB_H

using namespace ExerSense;
class ExerBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{
public:
  // BLE related classes and call backs - ENDS here

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();
    std::string cmd;
    std::string argstr;
    ExerBLE::GetInstance()->setConnectedDeviceName(rxValue.c_str());
    return;

    cmd = rxValue.substr(0, 1);
    argstr = rxValue.substr(1, 3);
    // Serial.println(cmd.c_str());
    // Serial.println(argstr.c_str());

    // its a Button
    if (cmd == "s" || cmd == "S")
    {
      // Serial.println("1 pressed/release : Toggling action of button A on M5");
      // audio_beep();
      start_read = !start_read;
    }

    if (cmd == "d" || cmd == "D")
    {
      // dump data
      // dump_ble_data();
    }

    if (cmd == "r" || cmd == "R")
    {
      // on_A_pressedFor();
    }

    if (cmd == "p" || cmd == "P")
    {
      // set period
      int new_period = atoi(argstr.c_str());
      if (new_period > delta_offset)
      {
        // audio_beep();
        delta_T = new_period;
        delta_adjusted = delta_T - delta_offset;
        // show_deltas();
      }
    }

    if (cmd == "o" || cmd == "O")
    {
      // set offset
      int new_offset = atoi(argstr.c_str());
      if (new_offset > 24 && new_offset < 30)
      {
        // audio_beep();
        delta_offset = new_offset;
        delta_adjusted = delta_T - delta_offset;

        // show_deltas();
      }
    }
  }
};
#endif