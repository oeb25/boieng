#include "Arduino.h"

#define JOE 1
#define MIKE !JOE

bool IS_CONTROLLER = true;

#include <Wire.h>
#include "prog.h"
#include "controller.h"

const auto FORWARD = HIGH;
const auto BAKWARD = LOW;

void setup() {
  set_wheel_pins();
  begin_gyro();

  Serial.begin(9600);
}

bool did_clear = false;

void loop() {
  if (IS_CONTROLLER) {
    do_contoller_rotine();

    delay(10);

    return;
  }

  // read inputs

  Gyro gyro;

  read_gyro(&gyro);

  float roll = get_roll(&gyro);

  if (millis() % 1000 < 100) {
    if (!did_clear) {
      did_clear = true;
      clear_gyro_wire();
    }
  } else {
    did_clear = false;
  }


  // act on inputs

  long wheel_speed = tanh(roll / 20) * 200; // Shake


  // write output state

  wheel(R_F, R_B, wheel_speed);
  wheel(L_F, L_B, wheel_speed);
}
