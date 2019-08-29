#include "app.h"
#include "Manager.h"

using namespace ev3api;

Manager manager;

void tracer_cyc(intptr_t exinf) {
    act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t exinf) {
  if (ev3_button_is_pressed(LEFT_BUTTON)) {
    wup_tsk(MAIN_TASK);  // 左ボタン押下でメインを起こす
  } else {
    manager.touchStart();  // タッチスタート
  }
  ext_tsk();
}

void main_task(intptr_t unused) {
  manager.init();
  ev3_sta_cyc(TRACER_CYC);
  slp_tsk();  // 起きたら、走行をやめる
  ev3_stp_cyc(TRACER_CYC);
  manager.finish();
  ext_tsk();
}
