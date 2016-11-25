#include <radar.h>
#include <Servo.h>
#include <canvas.h>
#include <OLED_I2C.h>
#include <ultrasonic_hc_sr04.h>

OLED        myOLED(SDA, SCL, 8);
Canvas      canvas(&myOLED);
Servo       servo;
USonicDist  distancer;

RadarDrawer radar_drawer;
Radar       radar;


void setup()
{
  Serial.begin(9600);
  
  myOLED.begin();
  servo.attach(3);
  distancer.attach(11, 12);
  
  radar.attachServo(&servo);
  radar.attachDist(&distancer);
  radar.setup();
  
  radar_drawer.attachRadar(&radar);
  radar_drawer.attachCanvas(&canvas);
  radar_drawer.setCoor(64, 63, 63);
  
  delay(1000);
}

void loop()
{
  delay(10);
  radar.loopStep();
  radar_drawer.loopStep();
}
