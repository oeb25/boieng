#include <Wire.h>
#include "prog.h"

const auto FORWARD = LOW;
const auto BAKWARD = HIGH;

void setup() {
  set_wheel_pins();
  begin_gyro();

  Serial.begin(9600);
}

void loop() {
  // read inputs

  float roll = read_gyro();

  if (digitalRead(3) > 0) {
    if (roll > 0) {
      offset--;
    } else if (roll < 0) {
      offset++;
    }
  }


  // act on inputs

  OutputState state = {
    right: roll > 0,
    left: roll > 0,
  };


  // write output state

  String state_log = state_to_string(&state);

  Serial.println(state_log);

  digitalWrite(R_F, state.right > 0 ? FORWARD : BAKWARD);
  digitalWrite(R_B, state.right < 0 ? FORWARD : BAKWARD);

  digitalWrite(L_F, state.left > 0 ? FORWARD : BAKWARD);
  digitalWrite(L_B, state.left < 0 ? FORWARD : BAKWARD);

  delay(100);
}
