/******************************************************************
  @file       xIMU3_Protocol.h
  @brief      Commands & MSG_IDs for the xIMU3 GUI
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.1.0
  Date:        24/04/23

  1.1.0     Original Release.       24/04/23

  Credit - Derived from the x-IMU-Arduino-Example by Seb Madgwick
           (https://github.com/xioTechnologies/x-IMU-Arduino-Example)
           and using information in the xIMU3 User Manual 
           (https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf).

******************************************************************/

#ifndef xIMU3_Protocol_h
#define xIMU3_Protocol_h

//  Based on API v1.1 - https://x-io.co.uk/downloads/x-IMU3-User-Manual-v1.1.pdf

#define xIMU3_API_VERSION_MAJOR     1
#define xIMU3_API_VERSION_MINOR     1
#define xIMU3_API_VERSION_REVISION  0

/******************************************************************
 *
 * xIMU3 Command Message Hash Table - Not all keys included
 * 
 ******************************************************************/

enum xIMU3_Command
{
  PING = 100,                      // Example: {"ping":null}\n
  SERIAL_NUMBER = 149,             // Example: {"serialnumber":null}\n
  SERIAL_MODE = 10,                // Example: {"devicename":"x-IMU3"}\n
  SERIAL_BAUD_RATE = 321,          // Example: {"devicename":"x-IMU3"}\n
  DEVICE_NAME = 529,               // Example: {"devicename":"x-IMU3"}\n
  DEFAULT_CMD = 555,               // Example: {"default":null}\n
  APPLY = 80,                      // Example: {"apply":null}\n
  SAVE = 329,                      // Example: {"devicename":null}\n
  TIME = 372,                      // Example Read: {"time":null}\n Example Write: {"time":"2020-01-01 00:00:00"}\n
  RESET = 639,                     // Example: {"reset":null}\n
  SHUTDOWN = 69,                   // Example: {"shutdown":null}\n
  STROBE = 367,                    // Example: {"devicename":null}\n
  COLOUR = 499,                    // Example: {"devicename":null}\n
  HEADING = 185,                   // Example: {"accessory":"hello \u0077\u006F\u0072\u006C\u0064"}\n
  ACCESSORY = 500,                 // Example: {"devicename":null}\n
  NOTE = 293,                      // Example: {"note":"Something happened."}\n
  FORMAT = 97,                     // Example: {"format":null}\n
  TEST = 735,                      // Example: {"test":null}\n
  BOOTLOADER = 259,                // Example: {"bootloader":null}\n
  FACTORY = 582,                   // Example: {"factory":null}\n
  ERASE = 624,                     // Example: {"erase":null}\n
  FIRMWARE_VERSION = 19,           // Example: {"devicename":null}\n
  BOOTLOADER_VERSION = 687,        // Example: {"devicename":null}\n
  HARDWARE_VERSION = 421,          // Example: {"devicename":null}\n
  SERIAL_RTS_CTS_ENABLED = 434,    // Example: {"devicename":null}\n
  WIRELESS_MODE = 36,              // Example: {"devicename":null}\n
  WIRELESS_FIRMWARE_VERSION = 178, // Example: {"devicename":null}\n
  EXTERNAL_ANTENNAE_ENABLED = 616, // Example: {"devicename":null}\n
  WIFI_REGION = 70,                // Example: {"devicename":null}\n
  WIFI_MAC_ADDRESS = 306,          // Example: {"devicename":null}\n
  SYNCHRONISATION_ENABLED = 453,   // Example: {"devicename":null}\n
  UNKNOWN_KEY = 120,               // Example: {"devicename":null}\n
  START = 170,                     // Example: {"start":"Test"}\n
  STOP = 523,                      // Example: {"stop":null}\n
  DELETE = 691,                    // Example: {"delete":"Test"}\n
  INERTIAL_MESSAGE_RATE_DIVISOR = 169,                    // Example: {"delete":"Test"}\n

};
/******************************************************************
 *
 * xIMU3 Global Constants
 * 
 ******************************************************************/

#define XIMU3_BUFFER_SIZE 1024
#define CMD_SIZE 128
#define NOTE_SIZE 256
#define HASH_SIZE 751

const char NULL_TERMINATOR  = '\0';

#endif