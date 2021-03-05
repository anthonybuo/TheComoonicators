# Firmware for Directional Antenna

## Environment

Set your Arduino sketchbook location to this directory. Install the following Arduino libraries:

* LIS3DHTR

Select the "no line ending" option on the Arduino serial monitor.

## Testing

Unfortunately, the Arduio serial monitor can't send raw bytes (as far as I know),
so a terminal emulator like RealTerm can be used to send a byte string to the
Arduino. Note that the terminal emulator should be disconnected from the serial
port while uploading Arduino firmware through the Arduino IDE.

## Pinout

See "Electronics Diagram.drawio" for complete pinout and serial communication format.

