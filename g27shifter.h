#ifndef __G27_SHIFTER_H__
#define __G27_SHIFTER_H__

#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>

#define BUTTON_SHIFT_REGISTER_MODE_PIN 0
#define BUTTON_DATA_PIN 4
#define BUTTON_CLOCK_PIN 1
#define BUTTON_MODE_AND_CLOCK_WAIT 10
#define NUMBER_OF_SHIFT_REGISTER_BUTTONS 16

#define BUTTON_PORT PORTD
#define BUTTON_PIN PIND

#define STICK_X_ADC 7
#define STICK_Y_ADC 6

typedef struct {
  uint16_t x;
  uint16_t y;
} g27coordinates;

void read_shift_register_buttons(uint8_t* buttons);

uint8_t read_selected_gear(bool isStickDown);


#endif
