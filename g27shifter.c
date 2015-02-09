#include "g27shifter.h"

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

void read_shift_register_buttons(uint8_t *buttons) {
  latch_shift_register();
  for (uint8_t i = 0; i < NUMBER_OF_SHIFT_REGISTER_BUTTONS; i++) {
    buttons[i] = read_button();
  }
}
