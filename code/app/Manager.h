#include "Tracer.h"
#include "Block.h"
#include "TouchSensor.h"

using namespace ev3api;

class Manager {
public:
  Manager();
  Tracer tracer;
  Block block;
  void init();
  void finish();
  void touchStart();
  void reLine(char direction);
  void leftJig();
  void rightJig();
  void lineTracer();
  //void moveParking();
  const int8_t pwm = (Motor::PWM_MAX) / 6;

private:
  TouchSensor touchSensor;

};
// end::tracer_h_private[]