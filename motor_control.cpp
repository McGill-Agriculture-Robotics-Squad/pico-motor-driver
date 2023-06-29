#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <string>
using namespace std;

class TB6612FNG_MD {
    public:
        TB6612FNG_MD(uint PWMA_pin=0, uint AI2_pin=1, uint AI1_pin=2, uint STBY_pin=6, uint BI1_pin =5, uint BI2_pin=4, uint PWMB_pin=3) 
        : pwma_pin(PWMA_pin), ai2_pin(AI2_pin), ai1_pin(AI1_pin), stby_pin(STBY_pin), bi1_pin(BI1_pin), bi2_pin(BI2_pin), pwmb_pin(PWMB_pin){

            // initialize pins
            gpio_init(ai1_pin);
            gpio_init(ai2_pin);
            gpio_init(bi1_pin);
            gpio_init(bi2_pin);
            gpio_init(stby_pin);

            gpio_set_dir(ai1_pin, GPIO_OUT);
            gpio_set_dir(ai2_pin, GPIO_OUT);
            gpio_set_dir(bi1_pin, GPIO_OUT);
            gpio_set_dir(bi2_pin, GPIO_OUT);
            gpio_set_dir(stby_pin, GPIO_OUT);
            
            // gpio_put(stby_pin, true);

            set_standby_mode(false);


            // Setup PWM
            gpio_set_function(pwma_pin, GPIO_FUNC_PWM);
            gpio_set_function(pwmb_pin, GPIO_FUNC_PWM);
            
            pwm_set_wrap( pwm_gpio_to_slice_num(pwma_pin), top);
            pwm_set_wrap( pwm_gpio_to_slice_num(pwmb_pin), top);

            pwm_set_enabled(pwm_gpio_to_slice_num(pwma_pin), true);
            pwm_set_enabled(pwm_gpio_to_slice_num(pwmb_pin), true);
        }

        //set speed [0-100%]
        void set_speed( uint percentage, bool motor=true) {
            // motor==true Motor A, motor==false for Motor B
            uint &pwm_pin = (motor==true) ? pwma_pin : pwmb_pin; 

            pwm_set_chan_level(pwm_gpio_to_slice_num(pwm_pin), 
            pwm_gpio_to_channel(pwm_pin),
            percentage*top/100);
        }

        void set_direction(string operation, bool motor=true){

            uint &dir1_pin = (motor==true) ? ai1_pin : bi1_pin; 
            uint &dir2_pin = (motor==true) ? ai2_pin : bi2_pin;

            if (operation=="ccw"){
                gpio_put(dir1_pin, false);
                gpio_put(dir2_pin, true);
            } else if (operation=="cw"){
                gpio_put(dir1_pin, true);
                gpio_put(dir2_pin, false);
            } else if (operation=="short_brake"){
                gpio_put(dir1_pin, true);
                gpio_put(dir2_pin, true);
            } else if (operation=="stop"){
                gpio_put(dir1_pin, false);
                gpio_put(dir2_pin, false);
            } else {}
        }

        void set_standby_mode(bool active=false){
            stby_state = active;
            gpio_put(stby_pin, !stby_state);
        }

        bool get_standby_state(){
            return stby_state;
        }

    private:
        uint pwma_pin, ai2_pin, ai1_pin, stby_pin, bi1_pin, bi2_pin, pwmb_pin;
        uint top=255; // 8-bit resolution
        bool stby_state;

};

int main(){
    /* Setup motors
    Motors: 
    Front Left(motors_left==true) 
    Back Left(motors_left==false) 
    Front Right(motores_right==true) 
    Back Right(motores_right==false)
    */
    TB6612FNG_MD motors_left(0,1,2,6,5,4,3);
    TB6612FNG_MD motors_right(20,21,22,19,26,27,28);

    while (true){

        motors_left.set_direction("cw", true);
        motors_left.set_direction("cw", false);
        motors_right.set_direction("cw", true);
        motors_right.set_direction("cw", false);

        motors_left.set_speed(50,true);
        motors_left.set_speed(50,false);
        motors_right.set_speed(50,true);
        motors_right.set_speed(50,false);


        sleep_ms(5000);

        motors_left.set_direction("short_brake", true);
        motors_left.set_direction("short_brake", false);
        motors_right.set_direction("short_brake", true);
        motors_right.set_direction("short_brake", false);

        sleep_ms(5000);

        motors_left.set_direction("ccw", true);
        motors_left.set_direction("ccw", false);
        motors_right.set_direction("ccw", true);
        motors_right.set_direction("ccw", false);

        motors_left.set_speed(40, true);
        motors_left.set_speed(40, false);
        motors_right.set_speed(40, true);
        motors_right.set_speed(40, false);


        sleep_ms(5000);

        motors_left.set_direction("stop", true);
        motors_left.set_direction("stop", false);
        motors_right.set_direction("stop", true);
        motors_right.set_direction("stop", false);

        sleep_ms(5000);

        motors_left.set_direction("ccw", true);
        motors_left.set_direction("ccw", false);
        motors_right.set_direction("ccw", true);
        motors_right.set_direction("ccw", false);

        motors_left.set_speed(100, true);
        motors_left.set_speed(100, false);
        motors_right.set_speed(100, true);
        motors_right.set_speed(100, false);

        sleep_ms(5000);

        motors_left.set_direction("cw", true);
        motors_left.set_direction("cw", false);
        motors_right.set_direction("cw", true);
        motors_right.set_direction("cw", false);

        motors_left.set_speed(100,true);
        motors_left.set_speed(100,false);
        motors_right.set_speed(100,true);
        motors_right.set_speed(100,false);

        sleep_ms(5000);
    }
};