#include "Tracer.h"


Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2),armWheel(PORT_D),
  touchSensor(PORT_1){
}

void Tracer::init() {
  init_f("Tracer");
  //calibration();
  initArm();
}

void Tracer::LorR() {
  while(1){
    if(ev3_button_is_pressed(LEFT_BUTTON)){
      msg_f("Run LEFT", 2);
      lr = true;
    }else if(ev3_button_is_pressed(RIGHT_BUTTON)){
      msg_f("Run RIGHT", 3);
      lr = false;
    }
    if(ev3_button_is_pressed(ENTER_BUTTON)) {
      break;
    }
    clock.wait(4);
  }
}

void Tracer::initArm() { // アームの位置の初期化
  uint32_t b_time = clock.now();
  // アームを一度下げる
  while( (clock.now()-b_time) < 600 ) {
    armWheel.setPWM(-25);
  }
  armWheel.reset();
  // アームの位置を設定する
  while( armWheel.getCount() < 32) {
    armWheel.setPWM(5);
  }
  armWheel.stop();
}

void Tracer::calibration() {
  msg_clear();
  while(1){
    if(ev3_button_is_pressed(LEFT_BUTTON)){
      msg_f("Black Value", 2);
      blackValue = colorSensor.getBrightness();
      sound(NOTE_DS6,500);
      msg_f(blackValue, 3);
    }else if(ev3_button_is_pressed(RIGHT_BUTTON)){
      msg_f("White Value", 4);
      whiteValue = colorSensor.getBrightness();
      sound(NOTE_DS6,500);
      msg_f(whiteValue, 5);
    }
    if(ev3_button_is_pressed(ENTER_BUTTON)) {
      target_y = (blackValue + whiteValue) / 2;
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
  const float Kp = 1.0;//0.98;
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
  // if(isTouch) run();
  if(isTouch) {
    KP = 0.60;
    vec_run(30,2450);
    sound(NOTE_DS6,200);
    KP = 1.1;
    vec_run(25,2000);
    sound(NOTE_DS6,200);
     KP = 0.6;
    vec_run(30,2200);
    sound(NOTE_DS6,200);
   //KP = 0.99;//R用
    //KP =1.2;//L用
    KP=1.1;
    vec_run(25,6000);
    sound(NOTE_DS6,200);
  KP = 0.8;
    vec_run(27,4000);
    sound(NOTE_DS5,200);
    KP = 0.8;
    vec_run(28,6000);
    sound(NOTE_DS5,200);
    KP = 0.7;
    vec_run(26.5,4000);
    sound(NOTE_DS5,200);
    KP = 1.1;
    vec_run(25.5,7900);
    sound(NOTE_DS5,200);
    KP = 0.6;
    vec_run(30,2000);
    terminate();
   }
}

/*******************************
 * ライントレース - 走行する      *
*******************************/
void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value();// calc_pid();
  int pwm_l, pwm_r;
  if(lr) { // 左コース
    pwm_l = pwm + turn;//正しい
   // pwm_l = pwm - turn;
    pwm_r = pwm - turn;//正しい
    //pwm_r = pwm + turn;
  }else{ // 右コース
    pwm_l = pwm - turn;
    pwm_r = pwm + turn;
  }
   // rightWheel.setPWM(pwm_r);
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}

/*******************************************************************************
 * ライントレース - 指定距離走行する。   vec_pwm = 速度(0~100) , time = 走行時間(msec) *
********************************************************************************/
void Tracer::vec_run(int8_t vec_pwm, uint32_t time) {
  clock.reset();
  while(1){
    if(clock.now() >= time) break;
    float turn = calc_pid();// calc_prop_value();
   // int pwm_l = vec_pwm - turn;//R
    int pwm_l = vec_pwm + turn;//L
 // int pwm_r = vec_pwm + turn;//R
  int pwm_r = vec_pwm - turn;//L
    leftWheel.setPWM(pwm_l);
    rightWheel.setPWM(pwm_r);
  }
}
