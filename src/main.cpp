#include "mbed.h"

#define t_1 3
#define t_2 10

DigitalOut leds[] = {(p6), (p8), (p7)};
InterruptIn but(p5);
Timeout time_led;
Timeout time_pressed;

enum states{RED, GREEN, YEL};
float times[] = {10.0, 20.0, 4.0};
int state = RED;
int last_state = state;
bool pressed;

void but_rise();
void but_fall();
void prox_led();
void t_off();
void t_on();
void alerta();
void pisca();

int main(){
    leds[state] = 1;
    but.rise(&but_rise);
    but.fall(&but_fall);
    time_led.attach(&prox_led, times[state]);
}

void but_rise(){
    pressed = true;
    if(state != 4) time_pressed.attach(&alerta, t_1);
    else time_pressed.attach(&t_on, t_2);
}

void but_fall(){
    pressed = false;
    if(state == GREEN) prox_led();
}

void prox_led(){
    leds[state] = 0;
    if(state<2) state++;
    else state = 0;
    leds[state] = 1;
    time_led.attach(&prox_led, times[state]);
    last_state = state;
}

void alerta(){
    if(pressed){
        if(state !=3){
            state = 3;
            for(int i=0; i<3; i++) leds[i]=0;
            time_led.attach(&pisca, 0.5);
        }
        else{
            for(int i=0; i<3; i++) leds[i]=0;
            state = last_state - 1;
            prox_led();
        }
        time_pressed.attach(&t_off, t_2 - t_1);
    }    
}

void pisca(){
    leds[YEL] = !leds[YEL];
    time_led.attach(&pisca, 0.5);
}

void t_off(){
    if(pressed){
        for(int i=0; i<3; i++) leds[i]=0;
        state = 4;
        time_led.detach();
        time_pressed.attach(&t_on, t_2);
    }
}

void t_on(){
    if(pressed){
        prox_led();
        time_pressed.attach(&t_on, t_2);
    }
}
