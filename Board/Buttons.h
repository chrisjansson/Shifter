/*
             LUFA Library
     Copyright (C) Dean Camera, 2014.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2014  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief LUFA Custom Board Button Hardware Driver (Template)
 *
 *  This is a stub driver header file, for implementing custom board
 *  layout hardware with compatible LUFA board specific drivers. If
 *  the library is configured to use the BOARD_USER board mode, this
 *  driver file should be completed and copied into the "/Board/" folder
 *  inside the application's folder.
 *
 *  This stub is for the board-specific component of the LUFA Buttons driver,
 *  for the control of physical board-mounted GPIO pushbuttons.
 */

#ifndef __BUTTONS_USER_H__
#define __BUTTONS_USER_H__

#define BUTTON_SHIFT_REGISTER_MODE_PIN 0
#define BUTTON_DATA_PIN 4
#define BUTTON_CLOCK_PIN 1
#define BUTTON_MODE_AND_CLOCK_WAIT 10
#define G25_BUTTONS 16

#include <util/delay.h>

void read_buttons(uint8_t *buttons) {
	PORTD = PORTD & ~(1 << BUTTON_SHIFT_REGISTER_MODE_PIN);
	_delay_us(BUTTON_MODE_AND_CLOCK_WAIT);
	PORTD = PORTD | (1 << BUTTON_SHIFT_REGISTER_MODE_PIN);

	for (uint8_t i = 0; i < G25_BUTTONS; i++) {
		PORTD = PORTD & ~(1 << BUTTON_CLOCK_PIN);
		_delay_us(BUTTON_MODE_AND_CLOCK_WAIT);

		buttons[i] = (PIND >> BUTTON_DATA_PIN) & 0x01;

		PORTD = PORTD | (1 << BUTTON_CLOCK_PIN);
		_delay_us(BUTTON_MODE_AND_CLOCK_WAIT);
	}
}

	/* Includes: */
		// TODO: Add any required includes here

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_BUTTONS_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/Buttons.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Button mask for the first button on the board. */
			#define BUTTONS_BUTTON1 0xFF         // TODO: Add mask for first board button here

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Buttons_Init(void)
			{
				DDRD = (1 << BUTTON_SHIFT_REGISTER_MODE_PIN) | (1 << BUTTON_CLOCK_PIN);
				PORTD = 0;
				// TODO: Initialize the appropriate port pins as an inputs here, with pull-ups
			}

			static inline void Buttons_Disable(void)
			{
				// TODO: Clear the appropriate port pins as high impedance inputs here
			}

			static inline uint16_t Buttons_GetStatus(void) ATTR_WARN_UNUSED_RESULT;
			static inline uint16_t Buttons_GetStatus(void)
			{
				uint16_t buttonResult = 0;

				uint8_t buttons[16];
				read_buttons(buttons);
				for (uint8_t i = 0; i < G25_BUTTONS; i++) {
						buttonResult |= (buttons[i] << i);
				}
				return buttonResult;
			}
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif
