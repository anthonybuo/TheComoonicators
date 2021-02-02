# Firmware for Directional Antenna

## Environment

Set your Arduino sketchbook location to this directory. Install the following Arduino libraries:

* LIS3DHTR

Select the "no line ending" option on the Arduino serial monitor.

## Pinout

### Arduino -> Motor Driver Board

GPIO 4  -> M1A input

GPIO 5  -> M1B input

GPIO 6  -> M2A input

GPIO 7  -> M2B input

GND     -> GND


### Motor Driver Board -> Stepper Motor

M1A output -> yellow

M1B output -> orange

M2A output -> brown

M2B output -> black

VB+ output -> red

VB+ output -> green


### Power Supply -> Motor Driver Board

\+ -> VB+

\- -> VB-

### Limit Switch -> Arduino

C  -> GND

NC -> GPIO 2


