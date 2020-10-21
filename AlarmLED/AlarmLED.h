#ifndef AlarmLED_h
#define AlarmLED_h

#include <Arduino.h>

class AlarmLED
{
  private:
    int brightness;
    int state;
    int states[4] = {65,150,65,600};
    int interval;
    int lastTime;
    int pin;

  public:
    AlarmLED(int pin, int brightness);
    void start();
    void stop();
    void loop();
    bool isRunning();
};

#endif
