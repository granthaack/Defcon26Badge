#include "Accel.h"

//Set up software serial
#define SCL_PIN 5
#define SCL_PORT PORTC
#define SDA_PIN 4
#define SDA_PORT PORTC
#define I2C_HARDWARE 1
#include <SoftI2CMaster.h>

uint8_t read_reg_byte(uint8_t reg){
  //Start I2C communication with the acceleroeter
  //If it can't start, it's probably busy
  if (!i2c_start((ACCL_ADDR<<1)|I2C_WRITE)) {
    Serial.println("ERROR: I2C bus busy");
    return 0;
  }
  //Send the register that we want to write to
  i2c_write(reg);
  //Attempt to establish communication
  i2c_rep_start((ACCL_ADDR<<1)|I2C_READ);
  //Read back the byte and send a NAK when done
  uint8_t val = i2c_read(true);
  //Stop I2C communication
  i2c_stop();
  //Return the recieved byte
  return val;
}

uint8_t write_reg_byte(uint8_t val, uint8_t reg){
  //Start I2C communication with the acceleroeter
  //If it can't start, it's probably busy
  if (!i2c_start((ACCL_ADDR<<1)|I2C_WRITE)) {
    Serial.println("ERROR: I2C bus busy");
    return 0;
  }
  //Send the register we want to write to
  i2c_write(reg);
  //Send the byte we want to write
  i2c_write(val);
  //Halt communication
  i2c_stop();
}

uint8_t init_accel(){
  //Initialize I2C
  i2c_init();

  //Wait 10 milliseconds for the accelerometer to turn on and initialize itself
  delay(10);

  //Try to talk to the accelerometer
  if(read_reg_byte(WHO_AM_I) != 0x33){
    Serial.println("ERROR: Accelerometer could not be initialized");
    return 0;
  }
  //Write to CTRL_REG1 (0x20) with this configuration:
  //ODR[0:3] = 0b0111: 400 Hz data rate
  //LPen = 0b0: Normal resolution mode
  //Zen = 0b1: Z axis enable
  //Yen = 0b1: Y axis enable
  //Xen = 0b1: X axis enable
  write_reg_byte(0b01110111, CTRL_REG1);

  //Write to CTRL_REG4 (0x23) with this configuration:
  //BDU = 0b1: Output registers not updated
  //BLE = 0b0: Little endian
  //FS[1:0] = 0b00: +-2G
  //HR = 0b1: High resolution enabled
  //ST[1:0] = 0b00: Self test disabled
  //SIM = 0b0: 4 wire SPI interface, default. We're using I2C anyway
  write_reg_byte(0b10001000, CTRL_REG4);

  //Write to CTRL_REG3 (0x22) with this configuration:
  //I1_ZYXDA = 0b1: Enable ZYXDA interrupt on INT1
  //All other interrupts disabled
  write_reg_byte(0b00010000, CTRL_REG3);

  //Write to TEMP_CFG_REG (0x1F) with this configuration:
  //ADC_EN: Enable the ADCs
  write_reg_byte(0b10000000, TEMP_CFG_REG);

  return 1;
}

void read_accel(struct Accel* accel){
  //Set the accel struct data
  accel->x = read_reg_byte(OUT_X_L);
  accel->x |= ((read_reg_byte(OUT_X_H)) << 8);

  accel->y = read_reg_byte(OUT_Y_L);
  accel->y |= ((read_reg_byte(OUT_Y_H)) << 8);

  accel->z = read_reg_byte(OUT_Z_L);
  accel->z |= ((read_reg_byte(OUT_Z_H)) << 8);
}
