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
 *  \brief LUFA Custom Board Joystick Hardware Driver (Template)
 *
 *  This is a stub driver header file, for implementing custom board
 *  layout hardware with compatible LUFA board specific drivers. If
 *  the library is configured to use the BOARD_USER board mode, this
 *  driver file should be completed and copied into the "/Board/" folder
 *  inside the application's folder.
 *
 *  This stub is for the board-specific component of the LUFA Joystick
 *  driver, for a digital four-way (plus button) joystick.
*/

#include <avr/io.h>

#ifndef __JOYSTICK_USER_H__
#define __JOYSTICK_USER_H__

	/* Includes: */
		// TODO: Add any required includes here


	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_JOYSTICK_H)
			#error Do not include this file directly. Include LUFA/Drivers/Board/Joystick.h instead.
		#endif

		typedef struct {
			uint16_t x;
			uint16_t y;
		} coordinates;

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Mask for the joystick being pushed in the left direction. */
			#define JOY_LEFT 0                 // TODO: Add mask to indicate joystick left position here

			/** Mask for the joystick being pushed in the right direction. */
			#define JOY_RIGHT 0                // TODO: Add mask to indicate joystick right position here

			/** Mask for the joystick being pushed in the upward direction. */
			#define JOY_UP 0                   // TODO: Add mask to indicate joystick up position here

			/** Mask for the joystick being pushed in the downward direction. */
			#define JOY_DOWN 0                 // TODO: Add mask to indicate joystick down position here

			/** Mask for the joystick being pushed inward. */
			#define JOY_PRESS 0                // TODO: Add mask to indicate joystick pressed position here

/*			• Bit 3:0 – ADTS3:0: ADC Auto Trigger Source
			If ADATE in ADCSRA is written to one, the value of these bits selects which source will trigger an ADC
			conversion. If ADATE is cleared, the ADTS3:0 settings will have no effect. A conversion will be triggered by the
			rising edge of the selected interrupt flag. Note that switching from a trigger source that is cleared to a trigger
			source that is set, will generate a positive edge on the trigger signal. If ADEN in ADCSRA is set, this will start a
			conversion. Switching to Free Running mode (ADTS[3:0]=0) will not cause a trigger event, even if the ADC
			Interrupt Flag is set.*/

		/* Inline Functions: */
		#if !defined(__DOXYGEN__)
			static inline void Joystick_Init(void)
			{
				ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

				ADMUX |= (1 << REFS0); // Set ADC reference to AVCC

				DDRB = 1;
				DDRF = 0;
			}

			static inline void Joystick_Disable(void)
			{
				// TODO: Clear the joystick pins as high impedance inputs here
			}

			uint16_t read_adc(uint8_t mux){
				ADMUX = ((ADMUX) & ~7) | (mux & 7);
				ADCSRA |= (1 << ADEN);  // Enable ADC
				ADCSRA |= (1 << ADSC);  // Start A2D Conversions
				while(ADCSRA & (1 << ADSC)){

				}

				int l = ADCL;
				int result = (ADCH << 8) | l;
				if(result < 200){
					PORTB = 1;
				}
				else{
					PORTB = 0;
				}
				ADCSRA = ADCSRA & ~(1 << ADEN);  // Enable ADC
				return result;
			}

			static inline coordinates Joystick_GetStatus(void) ATTR_WARN_UNUSED_RESULT;
			static inline coordinates Joystick_GetStatus(void)
			{
				coordinates c;
				c.x = read_adc(7); //A0
				c.y = read_adc(6); //A1
				return c;
			}
		#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif
