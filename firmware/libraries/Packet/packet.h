#ifndef _PACKET_H_
#define _PACKET_H_

#include "Arduino.h"

// Represents a serial packet sent by the MCU to the computer.
class PacketOut {
 public:
  // Error codes
  enum Error {
    ELEVATION_COMMAND_OOB = 0,
    AZIMUTH_COMMAND_OOB,
    SPEED_COMMAND_OOB,
    ACCELEROMETER_READING_OOB,
    ACCELEROMETER_NO_COMM,
  };

  // Serializes this classes data in a byte stream.
  uint8_t* serialize();

  // Bitwise set an error in the error byte
  void set_error(Error error_code) {
    error |= (1 << error_code);
  }

  // Bitwise clear an error in the error byte
  void clear_error(Error error_code) {
    error &= ~(1 << error_code);
  }

  // Clear limit switch status
  void clear_limit_switch() {
    limit_switch_status = 0;
  }

  // Packet contents
  const uint8_t start = 255;
  uint8_t elevation_hi = 0;
  uint8_t elevation_lo = 0;
  uint8_t azimuth_hi = 0;
  uint8_t azimuth_lo = 0;
  uint8_t sample_rate = 0;
  uint8_t limit_switch_status = 0;
  uint8_t error = 0;
  uint8_t stop = 0;
  static constexpr uint8_t PACKET_SIZE = 9;

  // Serialized representation
  uint8_t serialized[PACKET_SIZE];
};

// Represents a serial packet sent from the computer to the MCU.
class PacketIn {
 public:
   // Commands
   enum Command {
     GOTO_POS_SPEED = 1,
     GOTO_HOME,
     PARK,
     UNPARK,
     MUSIC,
     GOTO_ELEVATION,
     GOTO_AZIMUTH,
     EMERGENCY_STOP,
     SET_AZIMUTH,
   };

   // Reads a byte into one of this packet's data bytes.
   void read_byte(uint8_t byte, uint8_t idx);

   uint8_t command = 0;
   uint8_t elevation_hi = 0;
   uint8_t elevation_lo = 0;
   uint8_t azimuth_hi = 0;
   uint8_t azimuth_lo = 0;
   uint8_t speed_hi = 0;
   uint8_t speed_lo = 0;
   uint8_t stop = 0;
   static constexpr uint8_t PACKET_SIZE = 9;
};

#endif  // _PACKET_H_

