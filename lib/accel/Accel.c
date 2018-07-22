#include Accel.h

uint8_t read_reg_byte(uint8_t reg){
  i2c->beginTransmission(ACCL_ADDR);
  i2c->write(reg);
  i2c->endTransmission();

  i2c->requestFrom(ACCL_ADDR, 1);
  return i2c->read();
}

uint8_t write_reg_byte(uint8_t val, uint8_t reg){
  i2c->beginTransmission(ACCL_ADDR);
  i2c->write(reg);
  i2c->write(val);
  i2c->endTransmission();
}

uint8_t init_accel(){

  //Initialize the I2C communication
  i2c->begin();

  //Wait 10 milliseconds for the accelerometer to turn on and initialize itself
  delay(10);

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
  write_reg_byte(0b10000000, TEMP_CFG_REG)

  return 1;
}

void read_accel(struct Accel* accel){
  //Turn on auto increment by writing 1 to MSb
  i2c->beginTransmission(ACCL_ADDR);
  i2c->write(OUT_X_L | 0x80);
  i2c->endTransmission()

  //Request 6 bytes from the accelerometer. These six bytes are:
  //Lower and upper 8 bits of X axis data
  //Lower and upper 8 bits of y axis data
  //Lower and upper 8 bits of z axis data
  i2c->requestFrom(ACCL_ADDR, 6);

  //Set the accel struct data
  accel->x = i2c->read();
  accel->x |= (((uint_16)i2c->read()) << 8);

  accel->y = i2c->read();
  accel->y |= (((uint_16)i2c->read()) << 8);

  accel->z = i2c->read();
  accel->z |= (((uint_16)i2c->read()) << 8);
}
