#include "Parking.h"

Parking::Parking():
  armWheel(PORT_D),
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2),
  touchSensor(PORT_1),steering(leftWheel,rightWheel){
}

void Parking::init() {
  uint32_t b_time = clock.now();
  // アームを一度下げる
  while( (clock.now()-b_time) <600 ) {
    armWheel.setPWM(-25);
  }
  armWheel.reset();
  // アームの位置を設定する
  while( armWheel.getCount() < 32) {
    armWheel.setPWM(5);
  }
  armWheel.stop();
}

/****
 stop machine
 ***/
void Parking::stop(){
  leftWheel.stop();
  rightWheel.stop();
}

char Parking::color_search(){
  color = colorSensor.getColorNumber();
  return color;
}

void Parking::turn(int speed, int32_t angle, char direction){
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


void Parking::start(){
  init();
  linetracer();
  if(color_search() == 2){
    nolinetracer();
  }
}

float Parking::calc_prop_value() {
  const float Kp = 1.0;
  const int target = 12;
  const int bias = 0;
  int new_target = colorSensor.getBrightness() - target; 
  return (Kp * (new_target) + bias);                     
}


void Parking::run(){
  float turn_p=calc_prop_value();
  int pwm_l = pwm - turn_p;
  int pwm_r = pwm + turn_p;
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
}

void Parking::linetracer(){
  int color_bw = color_search();
  if(color_bw != 0 && color_bw != 1 && color_bw != 6 && color_bw != 7 ){
    run();
  }
}

void Parking::nolinetracer(){
  int turn_number = 0;
  while(1){
    if (turn_number == 0){
      if(clock.now() <= 1000){
	leftWheel.setPWM(20);
	rightWheel.setPWM(20);
      }else{
	turn(10,260,'r');
	turn_number += 1;
	clock.reset();
      }

    }else if(turn_number == 1){
      if(clock.now() <= 1000){
	leftWheel.setPWM(22);
	rightWheel.setPWM(22); 
      }else{
	turn(10,268,'r');
	turn_number += 1;
	clock.reset();
      }

    }else if(turn_number ==  2){
      if(clock.now() <= 2000){
        leftWheel.setPWM(24);
        rightWheel.setPWM(24);
      }else{
	turn_number += 1;
	clock.reset();
      }
    }else{
      stop();
      msg_f(turn_number,2);
    }
  }
}
