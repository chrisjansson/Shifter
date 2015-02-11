#include "g27shifter.h"

void g27_initialize_io() {
  BUTTON_IO = (1 << BUTTON_SHIFT_REGISTER_MODE_PIN) | (1 << BUTTON_CLOCK_PIN);
  BUTTON_PORT = BUTTON_PORT & ~(1 << BUTTON_DATA_PIN);

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz
  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  ADC_IO = 0;
}

static inline void latch_shift_register() {
  BUTTON_PORT = BUTTON_PORT & ~(1 << BUTTON_SHIFT_REGISTER_MODE_PIN);
  _delay_us(BUTTON_MODE_AND_CLOCK_WAIT);
  BUTTON_PORT = BUTTON_PORT | (1 << BUTTON_SHIFT_REGISTER_MODE_PIN);
}

static inline uint8_t read_button() {
  BUTTON_PORT = BUTTON_PORT & ~(1 << BUTTON_CLOCK_PIN);
  _delay_us(BUTTON_MODE_AND_CLOCK_WAIT);

  uint8_t button = (BUTTON_PIN >> BUTTON_DATA_PIN) & 0x01;

  BUTTON_PORT = BUTTON_PORT | (1 << BUTTON_CLOCK_PIN);
  _delay_us(BUTTON_MODE_AND_CLOCK_WAIT);
  return button;
}

static inline void read_shift_register_buttons(uint8_t *buttons) {
  latch_shift_register();
  for (uint8_t i = 0; i < NUMBER_OF_SHIFT_REGISTER_BUTTONS; i++) {
    buttons[i] = read_button();
  }
}

uint16_t read_buttons() {
  uint16_t buttonResult = 0;
  uint8_t buttons[NUMBER_OF_SHIFT_REGISTER_BUTTONS];

  read_shift_register_buttons(buttons);
  for (uint8_t i = 0; i < NUMBER_OF_SHIFT_REGISTER_BUTTONS; i++) {
      buttonResult |= (buttons[i] << i);
  }

  return buttonResult;
}

static inline uint16_t read_adc(uint8_t mux) {
  ADMUX = ((ADMUX) & ~7) | (mux & 7);
  ADCSRA |= (1 << ADEN);  // Enable ADC
  ADCSRA |= (1 << ADSC);  // Start A2D Conversions
  while(ADCSRA & (1 << ADSC)) {}

  int result = ADCL | (ADCH << 8); //ADCL must be read first
  ADCSRA = ADCSRA & ~(1 << ADEN);  // Disable ADC, ADC must be disabled to change MUX
  return result;
}

static inline g27coordinates read_shift_stick_coordinates() {
  g27coordinates c;
  c.x = read_adc(STICK_X_ADC);
  c.y = read_adc(STICK_Y_ADC);
  return c;
}

static inline uint8_t decode_shifter(g27coordinates c, bool isStickDown) {
      const uint8_t neutral = 0;
      const uint8_t first = (1 << 0);
      const uint8_t second = (1 << 1);
      const uint8_t third = (1 << 2);
      const uint8_t fourth = (1 << 3);
      const uint8_t fifth = (1 << 4);
      const uint8_t sixth = (1 << 5);
      const uint8_t reverse = (1 << 6);

      if(c.x < STICK_X_12) {
        if(c.y > STICK_Y_135){
          return first;
        }
        else if(c.y < STICK_Y_246R){
          return second;
        }
      }

      if(c.x > STICK_X_56R){
        if(c.y > STICK_Y_135){
          return fifth;
        }
        else if(c.y < STICK_Y_246R){
          if(isStickDown){
            return reverse;
          }
          return sixth;
        }
      }

      if(c.y > STICK_Y_135){
        return third;
      }

      if(c.y < STICK_Y_246R){
        return fourth;
      }

      return neutral;
}

uint8_t read_selected_gear(bool isStickDown){
  g27coordinates c = read_shift_stick_coordinates();
  uint8_t selectedGear = decode_shifter(c, isStickDown);
  return selectedGear;
}
