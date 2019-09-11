#include "Block.h"

Block::Block():
  colorSensor(PORT_2),armWheel(PORT_D){
}

/***********
 * 動作
 * ・アームの初期化
 * *********/
void Block::initArm() { // アームの位置の初期化
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

bool Block::colorStop() {
  int color = colorSensor.getColorNumber();
  if(color != 0 && color != 1 && color != 6 && color != 7 ) return true;
  return false;
}

char Block::WeiBschwarz() {
  int colorNum = colorSensor.getColorNumber();
  char color;
  if(colorNum == 6) color = 'w';
  else if(colorNum == 0 || colorNum == 1) color = 'b';
  return color;
}

int Block::getColorNumber() {
  int colorNum = colorSensor.getColorNumber();
  msg_f(colorNum,2);
  return colorNum;
}

void Block::checkColor() {
  uint32_t b_time = clock.now();
  int colorNum = -1;
  // アームを一度下げる
  while( (clock.now()-b_time) < 600 ) {
    armWheel.setPWM(25);
  }
  b_time = clock.now();
  while( (clock.now()-b_time) < 10000 ) {
    getColorNumber();
    clock.wait(4);
  }
  initArm();
}