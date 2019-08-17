#include "Motor.h"
#include "ColorSensor.h"
#include "TouchSensor.h"
#include "util.h"
#include "Clock.h"

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
  void vec_run(int8_t vec_pwm, uint32_t time);

  bool lr;

private:
  Motor leftWheel;
  Motor rightWheel;
  Motor armWheel;
  ColorSensor colorSensor;
  TouchSensor touchSensor;
  Clock clock;
  int8_t mThreshold = 0; // つかってなーーーい。
  const int8_t pwm = 16; // (Motor::PWM_MAX) / 6;

  bool isTouch = false;

  /* キャリブレーションで使用する変数たち */
  int8_t blackValue = 0;
  int8_t whiteValue = 0;

  /* pid 制御で使用する変数たち */
  int target_y = 12;
  const int bias_y = 0;
  const float DELTA_T = 0.004;
  float KP = 0.83;
  const float KI = 0.0;
  const float KD = 0.2;
  float intergral = 0;
  int oldDiff = 0;

  float calc_prop_value();
  float calc_pid();
};
// end::tracer_h_private[]
