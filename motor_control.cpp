#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <string>
using namespace std;

class Motor {
    public:
        Motor(uint PWM_pin, uint forward_pin, uint reverse_pin, uint stby_pin) 
        : pwm_pin(PWM_pin), dir_pin1(forward_pin), dir_pin2(reverse_pin), stby_pin(stby_pin){

            // initialize pins
            gpio_init(dir_pin1);
            gpio_init(dir_pin2);
            gpio_init(stby_pin);
            gpio_set_dir(dir_pin1, GPIO_OUT);
            gpio_set_dir(dir_pin2, GPIO_OUT);
            gpio_set_dir(stby_pin, GPIO_OUT);
            gpio_put(stby_pin, true);


            // Setup PWM
            gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
            uint slice_num = pwm_gpio_to_slice_num(pwm_pin);
            pwm_set_wrap(slice_num, top);
            pwm_set_enabled(slice_num, true);
        }

        //set speed [0-100%]
        void set_speed(uint percentage) {
            pwm_set_chan_level(pwm_gpio_to_slice_num(pwm_pin),
            pwm_gpio_to_channel(gpio),
            percentage*top/100
            );
        }

        void set_direction(string operation){

            if (operation=="ccw"){
                gpio_put(dir_pin1, false);
                gpio_put(dir_pin2, true);
            } else if (operation=="cw"){
                gpio_put(dir_pin1, true);
                gpio_put(dir_pin2, false);
            } else if (operation=="short_brake"){
                gpio_put(dir_pin1, true);
                gpio_put(dir_pin2, true);
            } else if (operation=="stop"){
                gpio_put(dir_pin1, false);
                gpio_put(dir_pin2, false);
            } else {
                break;
            }
        }

    private:
        uint pwm_pin, dir_pin1, dir_pin2, stby_pin;
        uint top =255; // 8-bit resolution

};

int main(){

    Motor test_motor(0,1,2,6);

    test_motor.set_direction("cw");
    test_motor.set_speed(50);
    
    sleep_ms(5000);

    test_motor.set_direction("short_brake");
    test_motor.set_speed(10);

    sleep_ms(5000);

    test_motor.set_direction("ccw");
    test_motor.set_speed(10);

    sleep_ms(5000);

    test_motor.set_direction("stop");
    test_motor.set_speed(85);

    sleep_ms(5000);

    test_motor.set_direction("cw");
    test_motor.set_speed(85);

    sleep_ms(5000);

    test_motor.set_direction("ccw");
    test_motor.set_speed(0);

    sleep_ms(5000);
};