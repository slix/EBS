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

  private:
    char id_;

    // Reigster list
    const char WHO_AM_I = 0x00;
    const char SMPLRT_DIV= 0x15;
    const char DLPF_FS = 0x16;
    const char GYRO_XOUT_H = 0x1D;
    const char GYRO_XOUT_L = 0x1E;
    const char GYRO_YOUT_H = 0x1F;
    const char GYRO_YOUT_L = 0x20;
    const char GYRO_ZOUT_H = 0x21;
    const char GYRO_ZOUT_L = 0x22;

    // Setting list to be loaded into registers
    const char DLPF_CFG_0 = (1<<0);
    const char DLPF_CFG_1 = (1<<1);
    const char DLPF_CFG_2 = (1<<2);
    const char DLPF_FS_SEL_0 = (1<<3);
    const char DLPF_FS_SEL_1 = (1<<4);

    // IC2 address
    const char itgAddress = 0x69;
};

#endif /* GYROSCO_PE_H */
