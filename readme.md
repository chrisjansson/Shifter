Logitech G25/27 Shifter to AVR  USB
==
This project is for using a G25/27 shifter connected directly to your computer as a HID device without the need for the main G25/27 unit.

## Setup ##

This is the pinout of the DSUB9 coming out of the shifter (female)

PIN   | Purpose                              | Wire Color
------|--------------                        |------
1     | Clock                                | Purple
2     | Serial Button Data                   | Gray
3     | Shift Register Mode Parallel/Serial  | Yellow
4     | X Axis                               | Orange
5     | Ground LEDs                          | White
6     | Ground Logic                         | Black
7     | +5V Short                            | Red
8     | Y Axis                               | Green
9     | +5V Short                            | Red

The first step is to wire the shfiter I/O.

The inputs used by the project are defined in **g27shifter.h**. The button I/Os are assumed to all be on the same AVR port, these are defined by the **BUTTON_PIN**,** BUTTON_PORT** and **BUTTON_IO**.

Next up is defining which pin **BUTTON_SHIFT_REGISTER_MODE_PIN**, **BUTTON_DATA_PIN**, and **BUTTON_CLOCK_PIN** is on.

After that is connecting the X and Y pins to an ADC port each, these are defined by **STICK_X_ADC** and **STICK_Y_ADC**.

Next is powering the logic circuitry, connect **pin 7** (or 9 for that matter since they're connected internally) to +5V or VCC and **pin 6** to ground. I recommend not connecting **pin 5** since that will power the leds and in turn reduce the voltage over the X and Y pots when powering from USB, doing this with an external power supply should work better.

## Build
Run the make file and flash the hex onto your AVR.

I've tested this with win-avr on a atmega32u4. avr-gcc and Atmels tool chain should work just as well.

## Gear selection is working poorly
You might need to adjust the ADC values for the X and Y axis used used for calculating the selected gear, these are also defined in **g27shifter.h**. Do this by wiring everything up then writing down the voltage on the X and Y pin for each gear in turn with everyting connected to USB.

For example:
Voltage in first might be 1.3 for x.

Now we need to find the ADC value 1.3 and 3.7 corresponds to, since the ADC runs in 10bit mode using VCCREF so measure the voltage from VCC to ground, for example 5V.

The ADC value is then 1.3 / 5 \* 1024. This should be the value for **STICK_X_12**.

Repeat for the other STICK_\* values.

## Differences between G27 and G25
To make this compatible with a G25 shifter switch pin 1 and 7 going into the DSUB9, that is clock on pin 7 and +5V on pin 1.

Since the G27 doesn't have the sequential mode that is not implemented, but as seen in the schematic value 4 from the shift register tells wether the shifter is in H or sequential mode.

##Hardware operation
The X and Y pins are simply potentiometers that tells how the stick is positioned. The buttons work through parallel in serial out shift registers. Pulling the mode pin low then holding it high latches the current input pins so that they can be sequentially read through the serial output. The shift register advances to the next value on the falling edge on the clock pin.

##Schematics
### G25 circuitry ###
![image](http://i.imgur.com/W0HSzhh.png?1)

### G25 and G27 pinouts ###
Showing that 1 and 9 are shorted in a G25

![image](http://i.imgur.com/csH44Uz.jpg?1)
