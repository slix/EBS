/*
 * UserIO.h
 *
 * Author:
 * Matt Potok
 */

#ifndef USERIO_H
#define USERIO_H

#include <Arduino.h>

/*
 * Class for the user I/O module
 */
class UserIO
{
  public:
    UserIO(int batery_led, int status_led, int up_shift_btn, 
        int down_shift_btn);

  private:
    int battery_led_;
    int status_led_;
    int up_shift_btn_;
    int down_shift_btn:
}

#endif /* USERIO_H */
