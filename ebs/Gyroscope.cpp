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
  itg_write(itgAddress, DLPF_FS, (DLPF_FS_SEL_0|DLPF_FS_SEL_1|DLPF_CFG_0));

  //Set the sample rate to 100 hz
  itg_write(itgAddress, SMPLRT_DIV, 9);
}

/*
 * Gyroscope::itg_write
 *   DESCRIPTION: writes a value to a register on the gyroscope
 *   INPUTS: i2c_addr -- IC2 address of sensor
 *           reg_addr -- address of register on sensor being written to
 *           data -- value to be written to specified register
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: changes value in sensor register
 */
void Gyroscope::itg_write(char i2c_addr, char reg_addr, char data)
{

}
