/*
 * Gyroscope.cpp
 *
 * Author:
 * Matt Potok
 */

#include "Gyroscope.h"

/*
 * Gyroscope::Gyroscope
 *   DESCRIPTION: constructor
 *   INPUTS: none
 */
Gyroscope::Gyroscope()
{
  // Initialize IC2 communication
  Wire.begin();
   
  // Read WHO_AM_I register
  id_ = itg_read(WHO_AM_I, 0x00);

  // Configure gyroscope settings
  itg_write(ITG_ADDR, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));

  //Set the sample rate to 100 hz
  itg_write(ITG_ADDR, SMPLRT_DIV, 9);
}

/*
 * Gyroscope::itg_write
 *   DESCRIPTION: writes a value to a register on the gyroscope
 *   INPUTS: i2c_addr -- IC2 address of sensor
 *           reg_addr -- address of register on sensor being written to
 *           data -- value to be written to specified register
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: changes register value in gyroscope
 */
void Gyroscope::itg_write(char i2c_addr, char reg_addr, char data)
{
  //Initiate a communication sequence with the desired i2c device
  Wire.beginTransmission(i2c_addr);

  //Tell the I2C address which register we are writing to
  Wire.write(reg_addr);

  //Send the value to write to the specified register
  Wire.write(data);

  //End the communication sequence
  Wire.endTransmission();
}

/*
 * Gyroscope::itg_read
 *   DESCRIPTION: read data from a register on the gyroscope
 *   INPUTS: i2c_addr -- IC2 address of sensor
 *           reg_addr -- address of register on sensor being read from
 *   OUTPUTS: none
 *   RETURN VALUE: register data
 *   SIDE EFFECTS: none
 */unsigned char Gyroscope::itg_read(char i2c_addr, char reg_addr)
{
  //This variable will hold the contents read from the i2c device.
  unsigned char data=0;
  
  //Send the register address to be read.
  Wire.beginTransmission(i2c_addr);
  
  //Send the Register Address
  Wire.write(reg_addr);

  //End the communication sequence.
  Wire.endTransmission();
  
  //Ask the I2C device for data
  Wire.beginTransmission(i2c_addr);
  Wire.requestFrom(i2c_addr, 1);
  
  //Wait for a response from the I2C device
  if(Wire.available()){
    //Save the data sent from the I2C device
    data = Wire.read();
  }
  
  //End the communication sequence.
  Wire.endTransmission();
  
  //Return the data read during the operation
  return data;
}

/*
 * Gyroscope::read_x_axis()
 *   DESCRIPTION: read x-axis rate of the gyroscope
 *   INPUTS: 
 *   OUTPUTS: none
 *   RETURN VALUE: x-axis ADC value from gyroscope
 *   SIDE EFFECTS: none
 */
int Gyroscope::read_x_axis()
{
  int data=0;
  data = itgRead(ITG_ADDR, GYRO_XOUT_H)<<8;
  data |= itgRead(ITG_ADDR, GYRO_XOUT_L);  
  
  return data;
}

/*
 * Gyroscope::read_y_axis()
 *   DESCRIPTION: read y-axis rate of the gyroscope
 *   INPUTS: 
 *   OUTPUTS: none
 *   RETURN VALUE: y-axis ADC value from gyroscope
 *   SIDE EFFECTS: none
 */
int Gyroscope::read_y_axis()
{
  int data=0;
  data = itgRead(ITG_ADDR, GYRO_YOUT_H)<<8;
  data |= itgRead(ITG_ADDR, GYRO_YOUT_L);  
  
  return data;
}

/*
 * Gyroscope::read_z_axis()
 *   DESCRIPTION: read z-axis rate of the gyroscope
 *   INPUTS: 
 *   OUTPUTS: none
 *   RETURN VALUE: z-axis ADC value from gyroscope
 *   SIDE EFFECTS: none
 */
int Gyroscope::read_z_axis()
{
  int data=0;
  data = itgRead(ITG_ADDR, GYRO_ZOUT_H)<<8;
  data |= itgRead(ITG_ADDR, GYRO_ZOUT_L);  
  
  return data;
}
