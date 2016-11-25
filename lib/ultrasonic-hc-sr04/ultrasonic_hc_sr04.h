#ifndef __USONIC__DIST__H__
#define __USONIC__DIST__H__
#include <Arduino.h>

class USonicDist
{
 public:
  USonicDist();
  USonicDist(int trig_pin, int echo_pin);
  
  void attach(int trig_pin, int echo_pin);
  void detach();
  
  float getDuration();
  float getCentimeter();
 protected:
  int trig_pin;
  int echo_pin;
  unsigned long int duration;
};

#endif
