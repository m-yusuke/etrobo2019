#include "Tracer.h"

Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2),armWheel(PORT_D),
  touchSensor(PORT_1){
}

void Tracer::init() {
  init_f("Tracer");
  initArm();
}

void Tracer::initArm() { // アームの位置の初期化
  uint32_t b_time = clock.now();
  // アームを一度下げる
  while( (clock.now()-b_time) < 600 ) {
    armWheel.setPWM(-25);
  }
  armWheel.reset();
  // アームの位置を設定する
  while( armWheel.getCount() < 35) {
    armWheel.setPWM(5);
  }
  armWheel.stop();
}

void Tracer::calibration() {
  while(1){
    if(ev3_button_is_pressed(LEFT_BUTTON)){
      msg_f("Black Value", 2);
      blackValue = colorSensor.getBrightness();
      msg_f(blackValue, 3);
    }else if(ev3_button_is_pressed(RIGHT_BUTTON)){
      msg_f("White Value", 4);
      whiteValue = colorSensor.getBrightness();
      msg_f(whiteValue, 5);
    }
    if(ev3_button_is_pressed(ENTER_BUTTON)) {
      mThreshold = (blackValue + whiteValue) / 2;
      msg_f("Waiting for start", 1);
      break;
    }
    clock.wait(4);
  }
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
  isTouch = false;
  leftWheel.stop();
  rightWheel.stop();
}

float Tracer::calc_prop_value() {
  const float Kp = 0.83;
  const int target = 10;
  const int bias = 0;

  int diff = colorSensor.getBrightness() - target;
  return (Kp * diff + bias);
}

// PID制御 - 制御が強すぎる？ 試走会で
float Tracer::calc_pid() {
  int diff = colorSensor.getBrightness() - target_y;
  intergral += (diff + oldDiff) / 2.0 * DELTA_T;

  float p = KP * diff + bias_y;
  float i = KI * intergral;
  float d = KD * (diff - oldDiff) / DELTA_T;

  oldDiff = diff;
  return p+i+d;
}

/*******************************
 * タッチスタート - 走行開始      *
*******************************/
void Tracer::touchStart() {
  if(touchSensor.isPressed()) isTouch = true;
  if(isTouch) run();
}

/*******************************
 * ライントレース - 走行する      *
*******************************/
void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_pid();//calc_prop_value();
  int pwm_l = pwm - turn;
  int pwm_r = pwm + turn;
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}
