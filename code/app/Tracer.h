#include "Motor.h"
#include "ColorSensor.h"
#include "TouchSensor.h"
#include "util.h"
#include "Clock.h"
#include "Steering.h"

using namespace ev3api;

class Tracer {
public:
  Tracer();
  void touchStart();
  void run();
  void init();
  void terminate();
  void initArm();
  void calibration();
  void LorR();
  void time_run(int8_t time_pwm, uint32_t time);
  void turn(int speed, int32_t angle, char direction);
  void move(int speed, char direction);
  void pause();
  bool searchLine(int tiny_speed, int tiny_angle);
  void goEndPoint();

  bool lr = false;
  bool isTouch = false;
  //float KP = 0.83;
  //float KP = 0.38;
  //float KP = 0.39;
  //float KP = 0.41;
  float KP = 1.0;
  //int8_t pwm = 30;

private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  TouchSensor touchSensor;
  Clock clock;
  Steering steering;
  //const int8_t pwm = 16;
  const int8_t pwm = 15;


  /* キャリブレーションで使用する変数たち */
  int8_t blackValue = 0;
  int8_t whiteValue = 0;

  /* pid 制御で使用する変数たち */
  int target_y = 12;
  const int bias_y = 0;
  const float DELTA_T = 0.004;
  const float KI = 0.00;
  //const float KI = 0.03;
  const float KD = 0.0;
  //const float KD = 0.1;
  //const float KD = 0.04;
  float intergral = 0;
  int diff = 0;
  int oldDiff = 0;

  float calc_pid();
  float calc_prop_value();
};
// end::tracer_h_private[]
