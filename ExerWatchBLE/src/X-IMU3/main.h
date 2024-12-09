#include "../ExerSense/exersense.h"
#include "../ExerSense/xIMU3/Reefwing_xIMU3.h"

namespace ExerSense
{
  class XIMU3Program : public ExerProgram
  {
  private:
    ExerDisplay *display = ExerDisplay::GetInstance();
    ExerBLE *ble;
    Ping pingPacket;
    Reefwing_xIMU3 rx;
    int cmdIndex = 0;
    char *cmdsHistory[CMDS_HISTORY_SIZE];
    char inertial_msg[256];
    char euler_msg[256];

    void parseCommand()
    {
      char *cmdPtr = rx.getCommand();
      char *valuePtr = rx.getValue();


      int cmd_id = rx.hash(cmdPtr) % HASH_SIZE;

      char msg[1024];
      sprintf(msg, "Received %d - %s: %s", cmd_id, cmdPtr, valuePtr);
      rx.sendNotification(msg);

      switch (cmd_id)
      {
        case xIMU3_Command::PING:
          rx.sendPing(pingPacket);
          break;
        case xIMU3_Command::DEVICE_NAME:
          rx.sendResponse("device_name", blename.c_str());
          break;
        case xIMU3_Command::SERIAL_NUMBER:
          rx.sendResponse("serial_number", "0123-4567");
          break;
        case xIMU3_Command::FIRMWARE_VERSION:
          rx.sendResponse("firmware_version", "v1.0");
          break;
        case xIMU3_Command::BOOTLOADER_VERSION:
          rx.sendResponse("bootloader_version", "Rel: 4.0.2");
          break;
        case xIMU3_Command::HARDWARE_VERSION:
          rx.sendResponse("hardware_version", "v1.0");
          break;
        case xIMU3_Command::SERIAL_MODE:
          rx.sendResponse("serial_mode", SerialMode::SERIAL_STANDARD);
          break;
        case xIMU3_Command::SERIAL_BAUD_RATE:
          rx.sendResponse("serial_baud_rate", 115200);
          break;
        case xIMU3_Command::SERIAL_RTS_CTS_ENABLED:
          rx.sendResponse("serial_rts_cts_enabled", "false");
          break;
        case xIMU3_Command::NOTE:
          rx.sendResponse("note", rx.getValue());
          break;

        case xIMU3_Command::INERTIAL_MESSAGE_RATE_DIVISOR:
          rx.sendResponse("inertial_message_rate_divisor", 8);
          break;
        case xIMU3_Command::SHUTDOWN:
          //  Shutdown Arduino command received
          rx.sendResponse("shutdown", "true");
          break;
        default:
          // char msg[100] = "Unhandled x-IMU3 command - ";
          // rx.sendError(strcat(msg, cmdPtr));
          break;
        }
        if (cmdPtr != NULL && strlen(cmdPtr) > 0)
        {
          cmdsHistory[cmdIndex] = new char[CMD_SIZE];
          sprintf(cmdsHistory[cmdIndex], "%d %s: %s", cmd_id, cmdPtr, valuePtr);
          cmdIndex += 1;
          if (cmdIndex >= CMDS_HISTORY_SIZE)
            cmdIndex = 0;
      }
    }

  public:
    void init()
    {
      for (int i = 0; i < CMDS_HISTORY_SIZE; i++)
      {
        cmdsHistory[i] = new char[CMD_SIZE];
        cmdsHistory[i][0] = 0;
      }

      //  Init Ping Packet - struct defined in Reefwing_imuTypes.h
      pingPacket.interface = (char*) "Serial";
      pingPacket.dName = (char *)blename.c_str();
      pingPacket.sNumber = (char *)blename.c_str();
    }

    void cleanup() {};

    void update_status()
    {
      if (start_time < 0)
      {
        start_time = millis();
      }
      if (!paused)
      {
        elapsed += millis() - last_update;
      }
      total_time = millis() - start_time;
      last_update = millis();
    }
    void update()
    {
      screen_buf->clear();
      StickCP2.Imu.update();
      ImuData imu_data = StickCP2.Imu.getImuData();
      inertial_msg[0] = 0;
      euler_msg[0] = 0;
      sprintf(inertial_msg, "I,%07d,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f\r\n", elapsed * 1000, imu_data.gyro.x, imu_data.gyro.y, imu_data.gyro.z, imu_data.accel.x, imu_data.accel.y, imu_data.accel.z);
      sprintf(euler_msg, "A,%07d,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f,%5.4f\r\n", elapsed * 1000, imu_data.gyro.x, imu_data.gyro.y, imu_data.gyro.z, imu_data.accel.x, imu_data.accel.y, imu_data.accel.z);
      bool a = Button_A.isPressed();
      short txtHeight = 14;
      int32_t y = 0;
      screen_buf->setTextSize(1.2);
      screen_buf->setTextColor(TFT_RED);
      screen_buf->setCursor(10, y);
      screen_buf->printf("X-IMU3 - Time : %6.2f / %6.2f ", elapsed / 1000.0, total_time / 1000.0);
      y += txtHeight;
      screen_buf->printf("%s", inertial_msg);
      rx.checkForCommand();
      if (rx.newCommand())
      {
        // Parse cmd string
        parseCommand();
      }

      for(int i = 0; i < CMDS_HISTORY_SIZE; i++)
      {
        y += txtHeight;
        screen_buf->setCursor(10, y);
        screen_buf->printf("%s", cmdsHistory[i]);
      }
      // if (Serial.available())
      // {
      //   cmdPtr = Serial.readString(); // ping request: {"ping": null}. response: {"deviceName" : "left_foot", "interface" : "UDP", "serialNumber" : "64F0EBCE"}
      // }

      Serial.print(inertial_msg);
      Serial.print(euler_msg);
      // send_inertial(imu_data, elapsed);
      if (a)
      {
        screen_buf->setTextSize(2.0);
        screen_buf->setTextColor(TFT_YELLOW);
        screen_buf->setCursor(10, 120);
        screen_buf->printf("PAUSE");
      }
      update_status();
      display->render();
    }
  };
}