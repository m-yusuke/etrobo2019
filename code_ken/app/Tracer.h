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
  void color_run(int color_val);
  void init();
  void terminate();
  void initArm();
  void calibration();
  void LorR();
  void time_run(int8_t time_pwm, uint32_t time);
  void turn(int speed, int32_t angle, char direction);
  void move(int speed, char direction);
  void time_move(int8_t speed, uint32_t time);
  void pause();
  bool searchLine(int tiny_speed, int tiny_angle);
  char whatColor(int colorNum);
  void goEndPoint();


  bool lr = false;
  bool isTouch = false;
  float KP = 1.0;//1.0;
  
private:
  Motor leftWheel;
  Motor rightWheel;
  ColorSensor colorSensor;
  TouchSensor touchSensor;
  Clock clock;
  Steering steering;
  const int8_t pwm = 15;//const30
  int8_t pwm_r = 16;
  int8_t pwm_l = 16;
  const int target = 12;
  const uint32_t duration = 4;

  /* キャリブレーションで使用する変数たち */
  int8_t blackValue = 0;
  int8_t whiteValue = 0;

  /* pid 制御で使用する変数たち */
  int target_y = 12;
  const int bias_y = 0;
  const float DELTA_T = 0.004;
  const float KI = 0.0;//0.01;
  const float KD = 0.06;//0.076;//const0.06
  float intergral = 0;
  int oldDiff = 0;

  float calc_pid(int target);
  float calc_prop_value();
  float calc_diff_value();
  int calc_diff_light(int target);
};
// end::tracer_h_private[]
