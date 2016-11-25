#include <ultrasonic_hc_sr04.h>

USonicDist::USonicDist()
{
  trig_pin = -1;
  echo_pin = -1;
  duration = 0;
}

USonicDist::USonicDist(int trig_pin, int echo_pin)
{
  this->trig_pin = trig_pin;
  this->echo_pin = echo_pin;

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  
  duration = 0;
}

void USonicDist::attach(int trig_pin, int echo_pin)
{
  this->trig_pin = trig_pin;
  this->echo_pin = echo_pin;

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  
  duration = 0;
}

void USonicDist::detach()
{
  trig_pin = -1;
  echo_pin = -1;
  duration = 0;
}

float USonicDist::getDuration()
{
  if(trig_pin != -1 || echo_pin != -1)
  {
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    duration = pulseIn(echo_pin, HIGH); 
  }
  
  return duration;
}

float USonicDist::getCentimeter()
{
  getDuration();
  return float(duration / 58);
}


