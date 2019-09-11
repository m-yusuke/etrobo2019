#include "Manager.h"


Manager::Manager():
    touchSensor(PORT_1){
}

void Manager::init() {
    block.initArm();
    tracer.LorR();//追加
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
    }
    else{
        if(tracer.lr) leftJig();
        else rightJig();
    }
  }
}

void Manager::lineTracer() {
    //tracer.KP = 0.6;
    //tracer.time_run(30,2000);
    //tracer.KP = 1.1;
    //sound(NOTE_DS5,200);
    //tracer.time_run(26,2500);
    //tracer.KP = 0.6;    
    //sound(NOTE_DS5,200);
    //tracer.time_run(26,3000);
    //tracer.KP = 1.1;
    //tracer.time_run(26,2000);
    //sound(NOTE_DS5,200);
     //tracer.KP = 1.2;
    //tracer.time_run(26,3000);
     //sound(NOTE_DS5,200);
    //tracer.KP = 1.0;
    //tracer.time_run(28,8000);
    //sound(NOTE_DS5,200);
    //tracer.KP = 0.6;
    //tracer.time_run(28,14000);
    
    tracer.KP = 1.0;
    tracer.time_run(25.5,8000);
    sound(NOTE_DS5,200);
    tracer.KP = 1.1;
    tracer.time_run(25.5,20000);
    sound(NOTE_DS5,200);
    tracer.KP = 1.20;
    tracer.time_run(25,4000);
    
    //tracer.KP = 1.1;
    //tracer.time_run(24,34000);
    tracer.KP = 1.40;
    tracer.time_run(24,4000);
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
