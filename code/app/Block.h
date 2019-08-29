#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"
#include "Clock.h"

using namespace ev3api;

class Block {
public:
  Block();
  void initArm();
  bool colorStop();
  char WeiBschwarz();
  int getColorNumber();
  void checkColor();

  int positions[4][4] = {{2,2,5,5},
                         {2,2,5,5},
                         {3,3,4,4},
                         {3,3,4,4},};

private:
  ColorSensor colorSensor;
  Motor armWheel;
  Clock clock;

};
// end::tracer_h_private[]
