#include "Motor.h"
#include "ColorSensor.h"
#include "TouchSensor.h"
#include "util.h"
#include "Clock.h"
#include "Steering.h"
//#include "Tracer.h"

using namespace ev3api;

class Parking {
public:
  Parking();
  //Tracer tracer;
  void init();
  void run();
  void start();
  void turn(int speed, int32_t angle, char direction);
  float calc_prop_value();
  
private:
  TouchSensor touchSensor;
  Motor armWheel;
  Clock clock;
  Steering steering;
  void stop();
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  char color_val;
  char color_search();
  int color;
  char color_judge;
  uint32_t b_time;
  const int8_t pwm = 10;
  void linetracer();
  void nolinetracer();
  //int speed = 30; 
  //int32_t angle = 536;
  //char direction = 'r';
};
