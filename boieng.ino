#include <Wire.h>
#include "prog.h"

const auto FORWARD = HIGH;
const auto BAKWARD = LOW;

// t is 0..1, where 0 is always off and 1 is always on
bool decide_pwm(float t) {
  auto time = millis();
  bool on = time % 100 < (t * 100);

  return on;
}

void pin_pwm(int pin, float t) {
  digitalWrite(pin, decide_pwm(t));
}

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
  digitalWrite(R_B, state.right <= 0 ? FORWARD : BAKWARD);

  digitalWrite(L_F, state.left > 0 ? FORWARD : BAKWARD);
  digitalWrite(L_B, state.left <= 0 ? FORWARD : BAKWARD);

  delay(10);
}
