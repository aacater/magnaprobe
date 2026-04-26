#include "sdcard.h"

MagnaSD sd;

void setup() {
  sd.SDset();
}

void loop() {
  sd.record();
}
