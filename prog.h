#include <math.h>

const int MPU_addr = 0x68;  // I2C address of the MPU-6050

const int L_F = 9;
const int L_B = 12;
const int R_F = 11;
const int R_B = 10;

const int GYRO_PIN = 3;

int t = 0;
int offset = -90;

void set_wheel_pins() {
  pinMode(R_F, OUTPUT);
  pinMode(R_B, OUTPUT);
  pinMode(L_F, OUTPUT);
  pinMode(L_B, OUTPUT);
}

void begin_gyro() {
  pinMode(GYRO_PIN, INPUT);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
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
  
  float roll = (atan2(-AcX, AcZ) * 180 / 3.14) + offset;

  return roll;
}

struct OutputState {
  int right;
  int left;
};
