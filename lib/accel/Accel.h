
/*
  Accel - A lightweight library for communicating with the LIS3DH
  acclerometer over with SoftI2cMaster for maximum RAM savings.
  Created by Grant Haack, July 21st 2018
  Released under GPL V3
    Join us now and share the Software
    You'll be free, Hackers, you'll be free
*/

#ifndef ACCEL_H_
#define ACCEL_H_

#include "Arduino.h"

//The accelerometer is hard wired with device address 0x19
#define ACCL_ADDR 0x19

//Register values
#define WHO_AM_I      0x0F
#define CTRL_REG1     0x20
#define CTRL_REG3     0x22
#define CTRL_REG4     0x23
#define TEMP_CFG_REG  0x1F
#define OUT_X_L       0x28
#define OUT_X_H       0x29
#define OUT_Y_L       0x2A
#define OUT_Y_H       0x2B
#define OUT_Z_L       0x2C
#define OUT_Z_H       0x2D

//The data structure that will hold the accelerometer data
struct Accel{
  int16_t x;
  int16_t y;
  int16_t z;
};
//Initialize the accelerometer. Returns 1 on success and 0 on failure
uint8_t init_accel();

//Read values from the accelerometer, store into the Accel struct passed to the
//function
void read_accel(struct Accel* accel);

//Read a byte from the accelerometer via I2C
uint8_t read_reg_byte(uint8_t reg);

//Write a byte from the acclerometer via I2C
uint8_t write_reg_byte(uint8_t val, uint8_t reg);

#endif
