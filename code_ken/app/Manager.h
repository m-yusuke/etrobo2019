#include "Tracer.h"
#include "Block.h"
#include "TouchSensor.h"
#include "ColorSensor.h"
#include "Motor.h"
#include "Clock.h"
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
  void stop();

private:
  TouchSensor touchSensor;
  ColorSensor colorSensor;
  Motor rightWheel;
  Motor leftWheel;
  Clock clock;
};
// end::tracer_h_private[]
