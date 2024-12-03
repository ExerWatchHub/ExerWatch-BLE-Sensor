#pragma once
#ifndef FREQ_SYNC_H
#define FREQ_SYNC_H

#include <Arduino.h>

class FrequencySync
{
  // If this is 0, it will update the frequency with very high precision at every single loop
  // It might be a waster of power so I'll leave this for the future, so we can updated the frequency like every 100ms
  static const int UPDATE_TIME = 0; // milliseconds
public:
  int lastSentPackets = 0;
  int lastElapsed = 0;
  long currDelay = 0;
  double currentFrequency = 0;
  FrequencySync(float targetFreq)
  {
    targetPackFreq = targetFreq;
    currentFrequency = 0;
  }

  void syncDelay()
  {
    if (currDelay >= 0)
    {
      delay(currDelay);
    }
  }

  void tick()
  {
    long now = millis();
    sentPackets++;
    if (lastSentTime < 0)
    {
      lastSentTime = now; // only for the first tick
      return;
    }
    elapsed = now - lastSentTime; // On the first tick elapsed will just be 0
    lastSentTime = now;
    if (elapsed >= UPDATE_TIME)
    {
      currentFrequency = ((double)((double)sentPackets / elapsed)) * 1000;
      currDelay = (1000 / targetPackFreq) - elapsed;
      lastSentPackets = sentPackets;
      lastElapsed = elapsed;
      elapsed = 0;
      sentPackets = 0;
    }
  }

protected:
  float targetPackFreq = 0; // default to 10hz
  long packSyncDelay = 0;
  long lastSentTime = -1;
  long elapsed = 0;
  int sentPackets = 0;
};

#endif