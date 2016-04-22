/*
 * Gyroscope.h
 *
 * Original Author:
 * Basic Example Sketch for the ITG-3200 (http://www.sparkfun.com/products/9801)
 * SparkFun Electronics 2011
 * Ryan Owens
 * 3/16/11
 *
 * Updated for Arduino 1.0 and beyond:
 * Joel Bartlett 
 * SparkFun Electronics
 * 10/16/13
 *
 * Modified for EBS: 
 * Matt Potok
 * Peter Kowalczyk
 */

#ifndef GYROSCOPE_H
#define GYROSCOPE_H

#include <Arduino.h>
#include <Wire.h>

/*
 * Class for the ITG-3200 gyroscope
 */
class Gyroscope
{
  public:
    Gyroscope();

    // Read z axis rotation and return in degrees per second
    // Prints an error to the serial console and returns 0.0 if the gyroscope is in a bugged state
    float read_z();

  private:
    char id_;

    int raw_read_x_axis();
    int raw_read_y_axis();
    int raw_read_z_axis();

    // Register list
    static const char WHO_AM_I = 0x00;
    static const char SMPLRT_DIV= 0x15;
    static const char DLPF_FS = 0x16;
    static const char GYRO_XOUT_H = 0x1D;
    static const char GYRO_XOUT_L = 0x1E;
    static const char GYRO_YOUT_H = 0x1F;
    static const char GYRO_YOUT_L = 0x20;
    static const char GYRO_ZOUT_H = 0x21;
    static const char GYRO_ZOUT_L = 0x22;

    // Setting list to be loaded into registers
    static const char DLPF_CFG_0 = (1<<0);
    static const char DLPF_CFG_1 = (1<<1);
    static const char DLPF_CFG_2 = (1<<2);
    static const char DLPF_FS_SEL_0 = (1<<3);
    static const char DLPF_FS_SEL_1 = (1<<4);

    // IC2 address
    static const char ITG_ADDR = 0x69;

    void itg_write(char i2c_addr, char reg_addr, char data);
    unsigned char itg_read(char i2c_addr, char reg_addr);
};

#endif /* GYROSCO_PE_H */
