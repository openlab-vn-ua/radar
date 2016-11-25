#ifndef __RADAR_H__
#define __RADAR_H__

#include <Servo.h>
#include <canvas.h>
#include <OLED_I2C.h>
#include <ultrasonic_hc_sr04.h>

#define d_type uint16_t

class Radar
{
 public:
  Radar();
  
  void attachServo(Servo *);
  void attachDist(USonicDist *);
  
  void setup();
  void loopStep();

  void rotate();
  void measure();

  int   getCurrAngleD();
  float getDistOnAngleD(int);

  int getMaxAngleD();
  int getMinAngleD();
  float getMaxDist();
  float getMinDist();
  
  
  int    getAngleStepD();
  d_type getLastData();
  bool   getRotateDir();
 protected:
  d_type     *data;
  d_type      last_data;
  Servo      *servo;
  USonicDist *distancer;

  int   prev_angle_d, curr_angle_d, angle_step_d;
  bool  rotate_direction;
  int   min_angle, max_angle;
  float min_dist,  max_dist;
};

class RadarDrawer
{
 public:
  RadarDrawer();

  void setup();
  void loopStep();
  
  void setCoor(int, int, int);
  void attachRadar (Radar *);
  void attachCanvas(Canvas *);

  void draw(int);
 protected:
  Radar *radar;
  Canvas *canvas;
  int x, y, r;
  bool first_draw;
};
#endif










  
