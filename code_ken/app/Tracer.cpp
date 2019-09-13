#include "Tracer.h"
#include <string.h> 

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
float Tracer::calc_pid(int target) {
  int diff = colorSensor.getBrightness() - target;
  intergral += (diff + oldDiff) / 2.0 * DELTA_T;

  float p = KP * diff + bias_y;
  float i = KI * intergral;
  float d = KD * (diff - oldDiff) / DELTA_T;

  oldDiff = diff;
  return p+i+d;
}

/*********************
 * 動作
 * ・p_control runで使用。
 * *******************/
float Tracer::calc_prop_value() {
  const float Kp = 1.0;        // <1>        // <2>
  const int bias = 0;

  int new_target = colorSensor.getBrightness() - target; // <3>反射光が強ければ(おそらく白?)diff>0,反射光が弱ければ(おそらく黒?)diff<0
  return (Kp * (new_target) + bias);                       // <4>
}

/*********************
 *動作
 *・d_control runで使用
 * *******************/

float Tracer::calc_diff_value(){
  const float Kd = 0.05;
  int dt = 0.04;
  //const int target = 12;
  float diff = colorSensor.getBrightness() - target;
  float d = Kd*target / dt;
  float oldDiff = diff;
  return d;
}

/*
calc_diff_light
 */


/*******************************
 * 動作
 * ・ライントレース - 通常走行
 * ******************************/
void Tracer::run() {
  msg_f("running...", 1);
  //float turn_p = calc_prop_value();//反射光が強い(白?)とturn>0,反射光が弱い(黒?)とturn<0
  //float turn_d = calc_diff_value();
  float turn_pid = calc_pid(target);
  //int pwm_l, pwm_r;
  if(lr) { // 左コース
    pwm_l = pwm + turn_pid;
    pwm_r = pwm - turn_pid;
  }else { // 右コース
    pwm_l = pwm - turn_pid;
    pwm_r = pwm + turn_pid;
  }
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}


/*
  color_val ...色の値
 */

void Tracer::color_run(int color_val) {
  //int diff = colorSensor.getBrightness() - target;
  switch(color_val)
    {
    case 4:
      int target_y = 77;//100 - 23 (白+黄色の平均を100から引いた値)
      int turn_y = calc_pid(target_y);
      int pwm_l,pwm_r;
      pwm_l = pwm - turn_y;
      pwm_r = pwm + turn_y;
      leftWheel.setPWM(pwm_l);
      rightWheel.setPWM(pwm_r);
    }
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
    float turn = calc_pid(target);
    int pwm_l = time_pwm - turn; // time_pwm + turn
    int pwm_r = time_pwm + turn; // time_pwm - turn
    leftWheel.setPWM(pwm_l);
    rightWheel.setPWM(pwm_r);
  }
}


//adding hiroto sorry no ref
void Tracer::time_move(int8_t speed,uint32_t time) {
  clock.reset();
  while(1){
    if(clock.now() >= time) break;
    move(20,'f');
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
    if(clock.now() >= 5000) break;
      leftWheel.stop();
      rightWheel.stop();
  }
}

/***********************
 * 動作
 * ・首を振ってラインを探す。
 * 引数
 * ・int tiny_speed:モータの速度
 * ・int tiny_angle:首を振る角度を指定。
 * *********************/
bool Tracer::searchLine(int tiny_speed, int tiny_angle){
  bool isEnd = true;
  int color = 0;
  leftWheel.reset();
  rightWheel.reset();
  while(1) {
    color = colorSensor.getColorNumber();
    if(color == 0 || color == 1) isEnd = false;
    clock.sleep(duration);
    if(leftWheel.getCount() <= (-1 * tiny_angle / 2) && rightWheel.getCount() >= tiny_angle / 2) break;
    leftWheel.setPWM(-1 * tiny_speed);
    rightWheel.setPWM(tiny_speed);
  }
  leftWheel.reset();
  rightWheel.reset();
  while(1) {
    color = colorSensor.getColorNumber();
    if(color == 0 || color == 1) isEnd = false;
    clock.sleep(duration);
    if(leftWheel.getCount() >= tiny_angle && rightWheel.getCount() <= (-1 * tiny_angle)) break;
    leftWheel.setPWM(tiny_speed);
    rightWheel.setPWM(-1 * tiny_speed);
  }
  leftWheel.reset();
  rightWheel.reset();
  while(1) {
    color = colorSensor.getColorNumber();
    if(color == 0 || color == 1) isEnd = false;
    clock.sleep(duration);
    if(leftWheel.getCount() <= (-1 * tiny_angle / 2) && rightWheel.getCount() >= tiny_angle / 2) break;
    leftWheel.setPWM(-1 * tiny_speed);
    rightWheel.setPWM(tiny_speed);
  }
  if (isEnd) sound(NOTE_D6,500);
  return isEnd;
}

char Tracer::whatColor(int colorNum){
  char color;
  switch (colorNum) {
  case 0:
    color = 'v';
    break;
  case 1:
    color = 'v';
    break;
  case 2:
    color = 'b';
    break;
  case 3:
    color = 'g';
    break;
  case 4:
    color = 'y';
    break;
  case 5:
    color = 'r';
    break;
  case 6:
    color = 'w';
    break;
  case 7:
    color = 'w';
    break;
  default:
    color = 'e';
    break;
  }
  return color;
}

/****************************
 * 動作
 * ・ラインを探しながら進んでいく。
 * ラインがない場合は反転。
 * **************************/
void Tracer::goEndPoint() {
  int speed = 10;
  int tiny_speed = 5;
  int32_t angle = 275;
  int32_t tiny_angle = 34;
  int32_t forward_angle = 110;
  bool end = true;
  int color = 0;
  char color_char = 'e';
  char chars[100] = "";
  char oldChars[100] = "";
  FILE *file;
  while(1){
    end = true;
    leftWheel.reset();
    rightWheel.reset();
    while(1) {
      run();
      color = colorSensor.getColorNumber();
      if(color != 0 && color != 1 && color != 6 && color != 7){ 
	color_char = whatColor(color);
	sprintf(chars, "%s%c", oldChars, color_char);
	sprintf(oldChars, "%s", chars);
	break;
      }
      clock.sleep(duration);
    }
    sound(NOTE_D6,500);
    leftWheel.reset();
    rightWheel.reset();
    while(1){
      if(leftWheel.getCount() >= forward_angle && rightWheel.getCount() >= forward_angle) break;
      leftWheel.setPWM(speed);
      rightWheel.setPWM(speed);
    }
    end = searchLine(tiny_speed, tiny_angle);
    while(end){
      leftWheel.reset();
      rightWheel.reset();
      angle = 137;
      while (1) {
	if (strcmp(chars, "yygg") != 0) speed = 0;
	if(leftWheel.getCount() <= (-1 * angle) || rightWheel.getCount() >= (1 * angle)) break;
	leftWheel.setPWM(-1 * speed);
	rightWheel.setPWM(speed);
	return ;
      }
      leftWheel.reset();
      rightWheel.reset();
      while(1){
	msg_f("end line", 2);
	if(leftWheel.getCount() >= tiny_angle && rightWheel.getCount() >= tiny_angle) break;
	leftWheel.setPWM(speed);
	rightWheel.setPWM(speed);
      }
      sound(NOTE_D6,500);
      file = fopen("/log.txt","a");
      fprintf(file , "%s\n" , chars);
      fclose(file);
      clock.sleep(duration);
      goEndPoint();
    }
  }
}