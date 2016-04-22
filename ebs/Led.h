#ifndef LED_H
#define LED_H


class Led {
  public:
    // Initialize led that is in pin
    Led(int pin);

    // Turn on LED if true, turn off LED if false
    void set_state(bool on);

  private:
    int pin;
};


#endif //LED_H
