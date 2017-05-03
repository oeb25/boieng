#include <Wire.h>
#include "prog.h"

const auto FORWARD = HIGH;
const auto BAKWARD = LOW;

// t is 0..1, where 0 is always off and 1 is always on
bool decide_pwm(float t) {
  auto time = millis();
  bool on = time % 10 < (t * 10);

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
/*
float signed_sqr(float t) {
  return t < 0 ? -(t * t) : (t * t);
}

int clamp_float_to_1024(float t) {
  int f = (int)(t * 1023);
  return f > 1023 ? 1023 : f < 0 ? 0 : f;
}*/

void wheel(int f, int b, float n) {
  Serial.println(n);
  if (n < 0.0) {
    analogWrite(f, 0);
    analogWrite(b, n > -216 ? round(-n-40) : 255);
  } else {
    analogWrite(f, n < 216 ? round(n+40) : 255);
    analogWrite(b, 0);
  }
}

void loop() {
  // read inputs

  float roll = read_gyro();

  if (millis() % 1000 < 100) {
    Serial.println("Clearing buffer");
    clear_gyro_wire();
  }

  roll = tanh(roll / 20) * 200; // Shake
  //roll = roll * 10; // Shake
  //roll = roll * 5; // Shake
  //roll = roll*7;

  wheel(R_F, R_B, roll);
  wheel(L_F, L_B, roll);
  // act on inputs

  /*OutputState state = {
    right: abs(roll) < 1 ? 0 : signed_sqr(roll / 10.0) * 5,
    left: abs(roll) < 1 ? 0 : signed_sqr(roll / 10.0) * 5,
  };


  // write output state

  String state_log = state_to_string(&state);

  // Serial.println(state_log);

  analogWrite(R_F, clamp_float_to_1024(state.right));
  analogWrite(R_B, clamp_float_to_1024(-state.right));

  analogWrite(L_F, clamp_float_to_1024(state.left));
  analogWrite(L_B, clamp_float_to_1024(-state.left));*/

  //delay(1);
}
