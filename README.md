# PTR
Programmable time relay

This project contains software for creating a programmable time relay with several outputs.

The relay operates on the AVR microcontroller ATmega328. The relay has a keyboard for set parameters.

The relay works as follows:
You set the steps, set the time of each step and the state of each output channel.
When the device is started, it executes the configured program and performs each configured step, switching ON (1) and OFF (0) those relays that must be turned on and off at each step of the program.

You do not need to program anything from the computer, everything is configured with 4 buttons and other two - start and pause the program.

The settings are stored in the internal memory of the microcontroller.
