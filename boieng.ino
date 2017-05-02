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

float abs_sqrt(float t) {
  return t < 0 ? -sqrt(-t) : sqrt(t);
}

void loop() {
  // read inputs

  float roll = read_gyro();
  Serial.print("Roll: ");
  Serial.println(roll);

  if (millis() % 1000 < 100) {
    Serial.println("Clearing buffer");
    clear_gyro_wire();
  }

  if (digitalRead(3) > 0) {
    if (roll > 0) {
      offset--;
    } else if (roll < 0) {
      offset++;
    }
  }


  // act on inputs

  OutputState state = {
    right: abs_sqrt(roll / 60.0),
    left: abs_sqrt(roll / 60.0),
  };


  // write output state

  String state_log = state_to_string(&state);

  //Serial.println(state_log);

  pin_pwm(R_F, state.right > 0 ? state.right : 0);
  pin_pwm(R_B, state.right < 0 ? -state.right : 0);

  pin_pwm(L_F, state.left > 0 ? state.left : 0);
  pin_pwm(L_B, state.left < 0 ? -state.left : 0);

  delay(10);
}











int read_gyro() {
  int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  
  if (offset == 0) {
    offset = -atan2(-AcZ, AcY) * 180 / 3.14;
  }

  Serial.println(AcX);
  Serial.println(AcY);
  Serial.println(AcZ);
  Serial.println(Tmp);
  Serial.println(GyX);
  Serial.println(GyY);
  Serial.println(GyZ);

  float roll = (atan2(-AcZ, AcY) * 180 / 3.14) + offset;

  return roll;
}