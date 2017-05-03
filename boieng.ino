#define JOE 1
#define MIKE !JOE

#include <Wire.h>
#include "prog.h"

const auto FORWARD = HIGH;
const auto BAKWARD = LOW;

void setup() {
  set_wheel_pins();
  begin_gyro();

  Serial.begin(9600);
}

void wheel(int f, int b, float n) {
  if (n < 0.0) {
    analogWrite(f, 0);
    analogWrite(b, n > -216 ? round(-n-40) : 255);
  } else {
    analogWrite(f, n < 216 ? round(n+40) : 255);
    analogWrite(b, 0);
  }
}

bool did_clear = false;

void loop() {
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
