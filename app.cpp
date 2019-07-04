#include "app.h"

#include "Tracer.h"

using namespace ev3api;

Tracer tracer;

void tracer_cyc(intptr_t exinf) {
    act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {
  if (ev3_button_is_pressed(LEFT_BUTTON)) {
    wup_tsk(MAIN_TASK);  // 左ボタン押下でメインを起こす
  } else {
    tracer.run();  // ;走行
  }
  ext_tsk();
}

void main_task(intptr_t unused) {
  tracer.init();
  char msgbuf[100];
  int mode = 0;
  while (1) {
      msg_f("waiting...",1);
      msg_f("press ENTER BUTTON",2);
      if (ev3_button_is_pressed(ENTER_BUTTON)) {
          break;
      }else if(ev3_button_is_pressed(RIGHT_BUTTON)){
        mode = 1;
        break;
      }
      tslp_tsk(10);
  }
  if (mode == 0){
      ev3_sta_cyc(TRACER_CYC);
      slp_tsk();  // 起きたら、走行をやめる
      ev3_stp_cyc(TRACER_CYC);
      tracer.terminate();
      ext_tsk();
  } else if (mode == 1){}
}
