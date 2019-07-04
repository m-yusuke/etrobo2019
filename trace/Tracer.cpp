#include "Tracer.h"
#include "ev3api.h"
#include "Clock.h"

Tracer::Tracer():
  leftWheel(PORT_C), rightWheel(PORT_B),
  colorSensor(PORT_2) {
}

void Tracer::init() {
  init_f("Tracer");
}

void Tracer::terminate() {
  msg_f("Stopped.", 1);
  leftWheel.stop();
  rightWheel.stop();
  ev3_speaker_stop();
}

// tag::tracer_calc_prop[]
float Tracer::calc_prop_value() {
  const float Kp = 0.83;        // <1>
  const int target = 10;        // <2>
  const int bias = 0;

  int diff = colorSensor.getBrightness() - target; // <3>
  char msgbuf[100];
  int i = 0;
  sprintf(msgbuf,"Brightness: %d", colorSensor.getBrightness());
  ev3_lcd_draw_string(msgbuf, 0, 2);
    //ev3_speaker_play_tone(colorSensor.getBrightness()*15, 100);
  if (colorSensor.getBrightness() > 40){
    ev3_speaker_set_volume(1);
    ev3_speaker_play_tone(NOTE_A4, 100);
  }
  //ev3_speaker_set_volume(50);
  //Clock clock;
  //clock.sleep(colorSensor.getBrightness()*10);
  return (Kp * diff + bias);                       // <4>
}
// end::tracer_calc_prop[]
// tag::tracer_run[]
void Tracer::run() {
  msg_f("running...", 1);
  float turn = calc_prop_value(); // <1>
  int pwm_l = pwm - turn;      // <2>
  int pwm_r = pwm + turn;      // <2>
  leftWheel.setPWM(pwm_l);
  rightWheel.setPWM(pwm_r);
  //leftWheel.setPWM(0);
  //rightWheel.setPWM(0);

}
// end::tracer_run[]
