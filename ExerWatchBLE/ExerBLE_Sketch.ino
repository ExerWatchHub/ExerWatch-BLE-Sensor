/*
 BLE code based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
*/

// #include <M5StickC.h>
// uncomment ^ here for M5StickC
// comment next line for M5SticlC
#include "M5StickCPlus.h"
#include "EasyButton.h"

// BLE Related
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
char message[100];

void dumpBLE(char *msg);
String blename;

// BLE Related

// find appropriate values for M5Stick for BUTTON A,B - see reverse
// of the device for numbers
#define BTN_A 37
#define BTN_B 39

int press_duration = 2000;
EasyButton Button_A(BTN_A, 40);
EasyButton Button_B(BTN_B, 40);
bool in_loop = false;

char *_version_ = "";
int dest = 1;
int delta_T = 100;
int delta_offset = 25;
int delta_adjusted;
// delta_offset is computed manually based on delta_T = 100
// it is the time to execute the code between reading and presenting
// data . so setup() will adjust down delta_T by delta_offset
// 23/Feb/2021

// for future
float threshold = 1.2;
int scale = 10;
bool start_read = false;

float accX;
float accY;
float accZ;
float gyroX;
float gyroY;
float gyroZ;
// readings buffer
// 1+3+3=7 floats x 4 bytes x 4000 = 64k
float acc_buff[21000];
// float acc_buff[210];
int buf_size = sizeof(acc_buff);

long idx = 0;
long last_dump_index = 0;
bool folded = false;
long seqno = 0;
//  secs since start :
// Beware: Folding of mils is possible no logic implemented as of now
unsigned long mils_initial = 0;
unsigned long mils = 0;
float secs = 0.0;
bool ice_breaker = true;

//////////////////////
// func declarations
void acc_init(int dest);
void acc_read(void);
void Display_readings(float X, float Y, float Z);
void on_B_Pressed();
void on_A_Pressed();

void show_deltas()
{
  // char *msg;
  // sprintf(msg,"%s \t %d\n","Delta_T(ms)",delta_T);
  Serial.print("Delta_T(ms)\t: ");
  Serial.println(delta_T);

  // sprintf(msg,"%s \t %d\n","Delta_offset(ms)",delta_offset);
  Serial.print("Delta_offset(ms)\t : ");
  Serial.println(delta_offset);
  Serial.print("Delta_adjusted(ms)\t : ");
  Serial.println(delta_adjusted);
}

void acc_init(int dest)
{
  // find appropriate line for initialising IMU for M5StickC
  M5.Imu.Init();
  Serial.println("IMU Initialised ");
}

void acc_read(int seq, int dest)
{

  // find appropriate line for reading X,Y,Z values from IMU
  static bool first = true;

  M5.Imu.getAccelData(&accX, &accY, &accZ);
  M5.Imu.getGyroData(&gyroX, &gyroY, &gyroZ);

  if (first)
  {
    // corresponds to time of first ready
    mils_initial = millis();
    mils = mils_initial;
    first = false;
    // sprintf(message,"{\"Device\":\"%s\",\"period\":%d}\n",blename.c_str(),delta_T);
    sprintf(message, "{\"Version\":\"%s\",\"Device\":\"%s\",\"period(ms)\":%d}\n", _version_, blename.c_str(), delta_T);

    Serial.print(message);
    sprintf(message, "Secs,SeqNo,AccX,AccY,AccZ,GyroX,GyroY,GyroZ\n");
    Serial.print(message);
  }
  else
  {
    mils = millis();
  }

  if (dest == 1)
  {

    secs = (float)(mils - mils_initial) / 1000.;
    sprintf(message, "%6.3f, %04d, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f\n",
            secs, (int)seq,
            accX, accY, accZ, gyroX, gyroY, gyroZ);
    Serial.print(message);
  }
}

/// SCreen routines
void Display_readings(float X, float Y, float Z, float gX, float gY, float gZ, float secs)
{
  M5.Lcd.setCursor(40, 30);
  M5.Lcd.printf("%+6.2f  ", X);
  M5.Lcd.setCursor(140, 30);
  M5.Lcd.printf("%+6.2f  ", gX);

  M5.Lcd.setCursor(40, 50);
  M5.Lcd.printf("%+6.2f  ", Y);
  M5.Lcd.setCursor(140, 50);
  M5.Lcd.printf("%+6.2f  ", gY);
  M5.Lcd.setCursor(40, 70);
  M5.Lcd.printf("%+6.2f  ", Z);
  M5.Lcd.setCursor(140, 70);
  M5.Lcd.printf("%+6.2f  ", gZ);
  M5.Lcd.setCursor(70, 90);
  M5.Lcd.printf("%6.3f", secs);
}

void audio_beep()
{
  M5.Beep.tone(4000);
  delay(100);
  M5.Beep.mute();
  // delay to ensure user has released the button
  delay(100);
}

void on_A_Pressed()
{
  if (in_loop)
  {
    Serial.println("Button A has been pressed! Toggle read");
    start_read = !start_read;
  }
}

void on_A_pressedFor()
{
  Serial.println("Restarting device...");
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(40, 30);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Restarting");
  // lets restart device
  // feedback to let user know to release button
  audio_beep();
  ESP.restart();
}

void on_B_Pressed()
{
  // DUMP Data
  int seqno = 0;
  start_read = false;
  int upto = 0;

  Serial.print("Size of Buff ");
  Serial.println(sizeof(acc_buff));
  Serial.print("folded? ");
  Serial.println(folded);
  Serial.print("idx ");
  Serial.println(idx);

  // Assess how much of the buffer is full of readings
  // check if we have folded

  if (folded)
  {
    upto = buf_size / 4;
  }
  else
  {
    upto = idx;
  }

  // Get a header out including json header
  // sprintf(message,"{\"Device\":\"M5Craftsman\",\"period\":%d}\n",delta_T+delta_offset);
  if (ice_breaker)
  {
    sprintf(message, "{\"Version\":\"%s\",\"Device\":\"%s\",\"period(ms)\":%d}\n", _version_, blename.c_str(), delta_T);

    dumpBLE(message);
    Serial.print(message);

    sprintf(message, "%s\n", "Secs,AccX,AccY,AccZ,GyroX,GyroY,GyroZ");
    dumpBLE(message);
    Serial.print(message);
    ice_breaker = false;
  }
  // DUMP the buffer
  for (int i = last_dump_index; i < upto; i = i + 7)
  {
    seqno = (int)acc_buff[i];
    sprintf(message, "%6.3f, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f, %+6.2f\n",
            acc_buff[i],
            acc_buff[i + 1] * scale, acc_buff[i + 2] * scale, acc_buff[i + 3] * scale,
            acc_buff[i + 4], acc_buff[i + 5], acc_buff[i + 6]);
    dumpBLE(message);
    Serial.print(message);
    // Lets not tax the BLE - give it some rest!

    // delay(10);
  }
  last_dump_index = upto;
  start_read = true;
}

// a9a778fe-c8d9-4f73-af1d-f4e407e9a78c
// BLE related classes and call backs

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define EXER_CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define EXER_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// on connect/disconnect callbacks
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    Serial.println("BLE Connected");
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    Serial.println("BLE Disconnected");
  }
};

// for control purposes.
class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue();
    std::string cmd;
    std::string argstr;

    cmd = rxValue.substr(0, 1);
    argstr = rxValue.substr(1, 3);
    // Serial.println(cmd.c_str());
    // Serial.println(argstr.c_str());

    // its a Button
    if (cmd == "s" || cmd == "S")
    {
      // Serial.println("1 pressed/release : Toggling action of button A on M5");
      audio_beep();
      start_read = !start_read;
    }

    if (cmd == "d" || cmd == "D")
    {
      // dump data
      on_B_Pressed();
    }

    if (cmd == "r" || cmd == "R")
    {
      on_A_pressedFor();
    }

    if (cmd == "p" || cmd == "P")
    {
      // set period
      int new_period = atoi(argstr.c_str());
      if (new_period > delta_offset)
      {
        audio_beep();
        delta_T = new_period;
        delta_adjusted = delta_T - delta_offset;
        show_deltas();
      }
    }

    if (cmd == "o" || cmd == "O")
    {
      // set offset
      int new_offset = atoi(argstr.c_str());
      if (new_offset > 24 && new_offset < 30)
      {
        audio_beep();
        delta_offset = new_offset;
        delta_adjusted = delta_T - delta_offset;

        show_deltas();
      }
    }
  }
};
// BLE related classes and call backs - ENDS here

//
void setup()
{
  /// SETUP BLE SERVER
  // Create the BLE Device
  uint64_t chipid = ESP.getEfuseMac();
  blename = "ExerWatch" + String((uint32_t)(chipid >> 32), HEX);

  Serial.begin(115200);
  Button_A.begin();
  Button_B.begin();

  Button_A.onPressed(on_A_Pressed);
  Button_B.onPressed(on_B_Pressed);
  Button_A.onPressedFor(2000, on_A_pressedFor);

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 5);
  // M5.Lcd.println("IMU Readings");
  M5.Lcd.println("IMU: Accel  Gyro");
  M5.Lcd.setCursor(15, 30);
  M5.Lcd.println("X");
  M5.Lcd.setCursor(15, 50);
  M5.Lcd.println("Y");
  M5.Lcd.setCursor(15, 70);
  M5.Lcd.println("Z");
  M5.Lcd.setCursor(15, 90);
  M5.Lcd.println("Secs");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(15, 110);
  sprintf(message, "%s %s", blename.c_str(), _version_);
  M5.Lcd.println(message);
  M5.Lcd.setTextSize(2);

  // Initialise Acceleromenter 1 means serial port
  acc_init(1);
  // initialise buf_ptr
  // Adjust delta_T
  delta_adjusted = delta_T - delta_offset;
  show_deltas();

  BLEDevice::init(blename.c_str());

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      EXER_CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      EXER_CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to send Acc data...");
}

/// SEND DATA via BLE
void dumpBLE(char *msg)
{
  if (deviceConnected)
  {
    pTxCharacteristic->setValue((uint8_t *)msg, strlen(msg));
    pTxCharacteristic->notify();
  }
}

void loop()
{
  in_loop = true;
  Button_A.read();
  Button_B.read();

  if (start_read)
  {
    acc_read(seqno, dest);
    // acc_buff[idx]=float(seqno);
    //  swithing to Secs.999
    acc_buff[idx] = secs;

    acc_buff[idx + 1] = accX;
    acc_buff[idx + 2] = accY;
    acc_buff[idx + 3] = accZ;
    acc_buff[idx + 4] = gyroX;
    acc_buff[idx + 5] = gyroY;
    acc_buff[idx + 6] = gyroZ;
    Display_readings(accX, accY, accZ, gyroX, gyroY, gyroZ, secs);
    seqno++;
    idx += 7;
    if (idx > 140)
    {
      on_B_Pressed();
    }
    if (idx >= buf_size / 4)
    {
      // foldit
      folded = true;
      idx = 0;
    }
    // this delay is a result of adjustments made for processing time
    // between sub-sequent acc_read() call - the adjustment amount
    // is delta_offset and when subtracted from required period gives
    // us delta_adjusted. Note delta_T cannot be lower than processing time
    // ie delta_offset
    delay(delta_adjusted);
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}