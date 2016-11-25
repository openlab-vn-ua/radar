#include <radar.h>
#include <Arduino.h>

void rotate(int x, int y, double angle,
            int &new_x, int &new_y)
{
  angle = (angle / 180.0) * PI;
  double cosA = cos(angle);
  double sinA = sin(angle);

  new_x = x*cosA - y*sinA;
  new_y = x*sinA + y*cosA;
}

Radar::Radar()
{
  data = NULL;
  servo = NULL;
  distancer = NULL;

  min_angle = 0;
  max_angle = 180;

  min_dist = 0;
  max_dist = 30;

  rotate_direction = true;
  curr_angle_d = min_angle;
  prev_angle_d = curr_angle_d;
  angle_step_d = 1.0;
}

void Radar::attachServo(Servo *servo)
{
  this->servo = servo;
}

void Radar::attachDist(USonicDist *distancer)
{
  this->distancer = distancer;
}

void Radar::setup()
{
  int numb_of_elem = max_angle - min_angle + 1;

  data = new d_type[numb_of_elem];

  for(int i = 0; i < numb_of_elem; ++i)
  {
    data[i] = max_dist;
  }
}

void Radar::loopStep()
{
  measure();
  rotate();
}

void Radar::rotate()
{
  prev_angle_d = curr_angle_d;
  
  if(rotate_direction)
  {
    curr_angle_d += angle_step_d;
  }
  else
  {
    curr_angle_d -= angle_step_d;
  }
    
  if(curr_angle_d > max_angle)
  {
    curr_angle_d  -= 2 * angle_step_d;
    rotate_direction = !rotate_direction;
  }
  else if(curr_angle_d < min_angle)
  {
    curr_angle_d  += 2 * angle_step_d;
    rotate_direction = !rotate_direction;
  }

  servo->write(curr_angle_d);
}

void Radar::measure()
{
  float value = 0.0;
  value = distancer->getCentimeter();
  value = constrain(value, min_dist, max_dist);
  last_data = data[curr_angle_d - min_angle];
  data[curr_angle_d - min_angle] = value;
}

int Radar::getCurrAngleD()
{
  return prev_angle_d;
}

float Radar::getDistOnAngleD(int angle)
{
  int diff = angle - min_angle;
  return data[diff];
}

int Radar::getMaxAngleD()
{
  return max_angle;
}

int Radar::getMinAngleD()
{
  return min_angle;
}

float Radar::getMaxDist()
{
  return max_dist;
}

float Radar::getMinDist()
{
  return min_dist;
}

int Radar::getAngleStepD()
{
  return angle_step_d;
}

d_type Radar::getLastData()
{
  return last_data;
}

bool Radar::getRotateDir()
{
  return rotate_direction;
}
//====================================================

RadarDrawer::RadarDrawer()
{
  x = y = r = 0;
  radar = NULL;
  first_draw = true;
}

void RadarDrawer::loopStep()
{
  canvas->beginPaint();

  if(first_draw)
  {
    draw(1);
    first_draw = !first_draw;
  }
  else
  {
    draw(0);
    draw(1);
  }
  
  canvas->endPaint();
  
  //Serial.print("curr_angle = "); Serial.println(radar->getCurrAngleD());
  //Serial.print("prev_dist = "); Serial.println(radar->getDistOnAngleD(radar->getCurrAngleD() - 1));
}

void RadarDrawer::setCoor(int x, int y, int r)
{
  this->x = x;
  this->y = y;
  this->r = r;
}

void RadarDrawer::attachRadar(Radar *radar)
{
  this->radar = radar;
}

void RadarDrawer::attachCanvas(Canvas *canvas)
{
  this->canvas = canvas;
  canvas->addCanvasUser();
}

void RadarDrawer::draw(int mode)
{
  OLED *screen = canvas->getScreen();
  void (OLED::*drawFunc)(int, int, int, int) = NULL;

  int angle_curr, dist_curr;
  int angle_next, dist_next;
  int angle_prev, dist_prev;
  angle_curr = radar->getCurrAngleD();
  if(mode == 0)
  {
    dist_curr = radar->getLastData();
    drawFunc = &OLED::clrLine;
    screen->clrPixel(x, y);
  }
  else if(mode == 1)
  {
    dist_curr = radar->getDistOnAngleD(angle_curr);
    drawFunc = &OLED::drawLine;
    screen->setPixel(x, y);
  }
  
  if(radar->getRotateDir())
  {
    angle_next = angle_curr - radar->getAngleStepD();
    angle_prev = angle_curr + radar->getAngleStepD();

    if(angle_next >= radar->getMinAngleD())
      dist_next = radar->getDistOnAngleD(angle_next);
    else
      dist_next = -1;
    if(angle_prev <= radar->getMaxAngleD())
      dist_prev = radar->getDistOnAngleD(angle_prev);
    else
      dist_prev = -1;
  }
  else
  {
    angle_next = angle_curr + radar->getAngleStepD();
    angle_prev = angle_curr - radar->getAngleStepD();

    if(angle_next <= radar->getMaxAngleD())
      dist_next = radar->getDistOnAngleD(angle_next);
    else
      dist_next = -1;
    
    if(angle_prev >= radar->getMinAngleD())
      dist_prev = radar->getDistOnAngleD(angle_prev);
    else
      dist_prev = -1;
  }
  /*
  Serial.println("---==============================---");
  Serial.print("curr_angle = "); Serial.println(angle_curr);
  Serial.print("dist_curr = "); Serial.println(dist_curr);
  Serial.print("dist_next = "); Serial.println(dist_next);
  Serial.print("dist_prev = "); Serial.println(dist_prev);
  Serial.println("---==============================---");
  */
  dist_curr = map(dist_curr,
                  radar->getMinDist(), radar->getMaxDist(),
                  0, r);
  if(dist_next != -1)
  {
    dist_next = map(dist_next,
                    radar->getMinDist(), radar->getMaxDist(),
                    0, r);
  }
  if(dist_prev != -1)
  {
    dist_prev = map(dist_prev,
                    radar->getMinDist(), radar->getMaxDist(),
                    0, r);
  }
  
  int x_curr = dist_curr, y_curr = 0;
  int x_next = dist_next, y_next = 0;
  int x_prev = dist_prev, y_prev = 0;

  rotate(x_curr, y_curr, angle_curr, x_curr, y_curr);
  if(dist_next != -1)
  {
    rotate(x_next, y_next, angle_next, x_next, y_next);
    (screen->*drawFunc)(x_curr + x, y - y_curr, x_next + x, y - y_next);
  }
  if(dist_prev != -1)
  {
    rotate(x_prev, y_prev, angle_prev, x_prev, y_prev);
    (screen->*drawFunc)(x_curr + x, y - y_curr, x_prev + x, y - y_prev);
  }
}

















