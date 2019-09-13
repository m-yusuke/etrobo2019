#include "Manager.h"


Manager::Manager():
  leftWheel(PORT_C), rightWheel(PORT_B),
  touchSensor(PORT_1),colorSensor(PORT_2){
}

void Manager::init() {
    block.initArm();
    tracer.init();
}

void Manager::finish() {
    tracer.terminate();
}

void Manager::touchStart() {
  if(touchSensor.isPressed()) tracer.isTouch = true;
  if(tracer.isTouch) {
    lineTracer();
    bool isColor = block.colorStop();
    if(!isColor){
        tracer.run();
	msg_f(colorSensor.getBrightness(),4);
    }
    
    else{
      
      /*
      if(tracer.lr){
        leftJig();
      }
      else{ 
        rightJig();
      }
      */



      /*
	rightコースのブロックビンゴエリアの4番サークルの左上の交点を走行体が上を向いているときにゴールできるはう。
       */
      tracer.pause();
      //tracer.time_move(10,5270);//ブロックビンゴエリア侵入からゴール,ワンチャン
      tracer.turn(10,280,'r');
      tracer.time_move(10,1810);
      tracer.turn(10,270,'r');
      //決まった時間進む処理
      tracer.time_move(10,2500);
      finish();
      
      
    }
  }
}



void Manager::lineTracer() {
  /*
    tracer.KP = 0.65;
    tracer.time_run(23,7000);
    tracer.KP = 0.98;
    tracer.time_run(20,21000);
    tracer.KP = 0.38;
    tracer.time_run(25,3700);
    tracer.KP = 0.98;
    tracer.time_run(20,15100);
  */
  //if(touchSensor.isPressed()) isTouch = true;
  // if(isTouch) run();

  /*
  tracer.KP = 1.1;
  tracer.time_run(24,34000);
  tracer.KP = 1.4;
  tracer.time_run(24,18000);
  sound(NOTE_DS5,200);
  tracer.time_run(20,15100);
  */
  
  //自習室用のスピード
  //tracer.KP=1.0;
  //tracer.run();
  int color = colorSensor.getColorNumber();
  tracer.color_run(color);
  //if((color != 0 or color != 1) and (color != 6 or color != 7)){
  if(color == 2){
    tracer.pause();
  }else{
    //tracer.KP=0.5;
    tracer.run();
  }
  //tracer.terminate();
}




void Manager::leftJig() {
    tracer.pause();
    tracer.turn(10,90,'b');
    tracer.pause();
    tracer.turn(10,100,'r');
    tracer.pause();
    reLine('r');
    tracer.lr = false;
}

void Manager::rightJig() {
    tracer.pause();
    tracer.turn(10,90,'b');
    tracer.pause();
    tracer.turn(10,100,'l');
    tracer.pause();
    reLine('l');
    tracer.lr = true;
}


/**********************
 * 動作
 * ・ラインに戻る
 * 引数
 * ・char direction:方向を指定する。 l 左回転, r 右回転
 * *********************/
void Manager::reLine(char direction) {
    bool Incomp = true;
    while(Incomp) {
        char color = block.WeiBschwarz();
        switch (color)
        {
            case 'w':
                tracer.move(10,'f');
                break;
        
            case 'b':
                tracer.pause();
                Incomp = false;
                break;
        }
    }
    Incomp = true;
    while(Incomp) {
        char color = block.WeiBschwarz();
        switch (color)
        {
            case 'w':
                tracer.pause();
                Incomp = false;
                break;
        
            case 'b':
                // tracer.move(10,'f');
                tracer.turn(10,60,'f');
                break;
        }
    }
    Incomp = true;
    while(Incomp) {
        char color = block.WeiBschwarz();
        switch (color)
        {
            case 'w':
                tracer.move(15,direction);
                break;
        
            case 'b':
                tracer.pause();
                Incomp = false;
                break;
        }
    }
}
