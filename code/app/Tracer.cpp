#include "Tracer.h"


Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2),
  touchSensor(PORT_1),steering(leftWheel,rightWheel){
}

void Tracer::init() {
  init_f("Tracer");
  // calibration();
}

/*************
 * 動作
 * ・LコースかRコースの選択
 * ***********/
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

/**********************
 * 動作
 * ・キャリブレーション
 * ********************/
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

/**********************
 * 動作
 * ・ストップする。
 * ********************/
void Tracer::terminate() {
  msg_f("Stopped.", 1);
  isTouch = false;
  leftWheel.stop();
  rightWheel.stop();
}

/**********************
 * 動作
 * ・pid計算部分 time_runで使用。
 * ********************/
float Tracer::calc_pid() {
  int diff = colorSensor.getBrightness() - target_y;
  intergral += (diff + oldDiff) / 2.0 * DELTA_T;

  float p = KP * diff + bias_y;
  float i = KI * intergral;
  float d = KD * (diff - oldDiff) / DELTA_T;

  oldDiff = diff;
  return p+i+d;
}

/*********************
 * 動作
 * ・pid計算部分 runで使用。
 * *******************/
float Tracer::calc_prop_value() {
  const float Kp = 0.85;        // <1>
  const int target = 12;        // <2>
  const int bias = 0;

  int diff = colorSensor.getBrightness() - target; // <3>
  return (Kp * diff + bias);                       // <4>
}

/*******************************
 * 動作
 * ・ライントレース - 通常走行
 * ******************************/
void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value();
  int pwm_l, pwm_r;
  if(lr) { // 左コース
    pwm_l = pwm + turn;
    pwm_r = pwm - turn;
  }else { // 右コース
    pwm_l = pwm - turn;
    pwm_r = pwm + turn;
  }
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}

/*******************************************************************************
 * 動作
 * ・ライントレース - 指定時間走行
 * 引数
 * ・int8_t time_pwm = 速度(0~100)
 * ・uint32_t time = 走行時間(msec) 1秒 = 1000
 * *****************************************************************************/
void Tracer::time_run(int8_t time_pwm, uint32_t time) {
  clock.reset();
  while(1){
    if(clock.now() >= time) break;
    float turn = calc_pid();
    int pwm_l = time_pwm - turn; // time_pwm + turn
    int pwm_r = time_pwm + turn; // time_pwm - turn
    leftWheel.setPWM(pwm_l);
    rightWheel.setPWM(pwm_r);
  }
}

/**************************************************************
 * 動作
 * ・ターンする(前進, 後進あり)。 
 * 引数 
 * ・int speed:モータの速度
 * ・int32_t angle:モータの各位置, 268 - 45度, 536 - 180度, 1072 - 360度
 * ・char direction:走行パターン。 l 左回転, r 右回転, f 前進, b 後進 
 * *************************************************************/
void Tracer::turn(int speed, int32_t angle, char direction) {
  switch (direction)
  {
  case 'l': // Left turn
    leftWheel.reset();
    while(1) {
      if(leftWheel.getCount() >= angle) break;
      steering.setPower(speed,50);
    }
    break;
  
  case 'r': // Right turn
    rightWheel.reset();
    while(1) {
      if(rightWheel.getCount() >= angle) break;
      steering.setPower(speed,-50);
    }
    break;

  case 'f': // Forward
    leftWheel.reset();
    rightWheel.reset();
    while(1) {
      if(leftWheel.getCount() >= angle && rightWheel.getCount() >= angle) break;
      steering.setPower(speed,0);
    }
    break;

  case 'b': // Back
    leftWheel.reset();
    rightWheel.reset();
    while(1) {
      if(leftWheel.getCount() <= -angle && rightWheel.getCount() <= -angle) break;
      steering.setPower(-speed,0);
    }
    break;
  }
}

/***********************
 * 動作
 * ・トレースしないで動く。
 * 引数
 * ・int speed:モータの速度
 * ・char direction:走行パターン。 l 左回転, r 右回転, f 前進, b 後進
 * *********************/
void Tracer::move(int speed, char direction){
  switch (direction)
  {
  case 'l': // Left turn
    steering.setPower(speed,100);
    break;
  
  case 'r': // Right turn
    steering.setPower(speed,-100);
    break;

  case 'f': // Foward
    steering.setPower(speed,0);
    break;
  
  case 'b': // Back
    steering.setPower(-speed,0);
    break;
  }
}

/****************************
 * 動作
 * ・0.5秒その場で一時停止する。
 * **************************/
void Tracer::pause(){
  clock.reset();
  while(1) {
    if(clock.now() >= 500) break;
      leftWheel.stop();
      rightWheel.stop();
  }
}