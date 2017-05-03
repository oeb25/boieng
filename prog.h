#include <math.h>

struct OutputState {
  float right;
  float left;
};

struct Gyro {
  int GyX;
  int GyY;
  int GyZ;
  int Tmp;
  int AcX;
  int AcY;
  int AcZ;
};

const int MPU_addr = 0x68;  // I2C address of the MPU-6050

const int L_F = JOE ? 11 : 11;
const int L_B = JOE ?  6 : 10;
const int R_F = JOE ?  9 :  5;
const int R_B = JOE ? 10 :  6;

void set_wheel_pins() {
  pinMode(R_F, OUTPUT);
  pinMode(R_B, OUTPUT);
  pinMode(L_F, OUTPUT);
  pinMode(L_B, OUTPUT);
}

void begin_gyro() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

String state_to_string(OutputState *state) {
  String out = "OutputState {\n  right: ";
  out += state->right;
  out += ",\n  left: ";
  out += state->left;
  out += "\n}";

  return out;
}

String state_to_string(Gyro *state) {
  String out = "Gyro {"
  out += "\n  GyX: ";
  out += state->GyX;
  out += ",\n  GyY: ";
  out += state->GyY;
  out += ",\n  GyZ: ";
  out += state->GyZ;
  out += ",\n  Tmp: ";
  out += state->Tmp;
  out +=  ",\n  AcX: ";
  out += state->AcX;
  out += ",\n  AcY: ";
  out += state->AcY;
  out += ",\n  AcZ: ";
  out += state->AcZ;
  out += "\n}";

  return out;
}

void read_gyro(Gyro * gyro) {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // request a total of 14 registers
  gyro->GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyro->GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyro->GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  gyro->Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gyro->AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  gyro->AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  gyro->AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
}

int initial_roll_offset = 0;

float get_roll(Gyro * gyro) {
  float roll = (atan2(-gyro->GyZ, gyro->GyY) * 180 / 3.14);

  if (initial_roll_offset == 0) {
    initial_roll_offset = -roll;
    return 0;
  }

  return roll + initial_roll_offset;
}

void clear_gyro_wire() {
  while (Wire.available()) {
    Wire.read();
  }
}
