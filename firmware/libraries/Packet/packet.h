#ifndef _PACKET_H_
#define _PACKET_H_

#include "Arduino.h"

// Represents a serial packet sent by the MCU to the computer.
class PacketOut {
 public:
  // Serializes this classes data in a byte stream.
  uint8_t* serialize() {
    serialized[0] = start;
    serialized[1] = elevation_hi;
    serialized[2] = elevation_lo;
    serialized[3] = azimuth_hi;
    serialized[4] = azimuth_lo;
    serialized[5] = sample_rate;
    serialized[6] = limit_switch_status;

    // if a payload byte is the same as the start byte, decrement it, then flag it in the stop byte
    stop = 0;
    for (int i = 0; i < PACKET_SIZE - 1; i++) {
      if (serialized[i + 1] == 255) {
        serialized[i + 1] -= 1;
        stop |= (1 << i);
      }
    }
    serialized[7] = stop;

    return serialized;
  }

  // Total packet size
  static constexpr uint8_t PACKET_SIZE = 8;

  // Serialized representation
  uint8_t serialized[PACKET_SIZE];

  // Packet contents
  const uint8_t start = 255;
  uint8_t elevation_hi = 0;
  uint8_t elevation_lo = 0;
  uint8_t azimuth_hi = 0;
  uint8_t azimuth_lo = 0;
  uint8_t sample_rate = 0;
  uint8_t limit_switch_status = 0;
  uint8_t stop = 0;
};

// Represents a serial packet sent from the computer to the MCU.
class PacketIn {
 public:

   // Dump the packet to serial port marked with 0xFFFF at the start.
   void debug_print() {
       Serial.write(0xFF);
       Serial.write(0xFF);
       Serial.write(command);
       Serial.write(azimuth_hi);
       Serial.write(azimuth_lo);
       Serial.write(elevation_hi);
       Serial.write(elevation_lo);
       Serial.write(speed_hi);
       Serial.write(speed_lo);
       Serial.write(stop);
   }

   // Reads a byte into one of this packet's data bytes.
   void read_byte(uint8_t byte, uint8_t idx) {
       switch(idx) {
           case 0:
               // Start byte (not needed)
               break;
           case 1:
               command = byte;
               break;
           case 2:
               azimuth_hi = byte;
               break;
           case 3:
               azimuth_lo = byte;
               break;
           case 4:
               elevation_hi = byte;
               break;
           case 5:
               elevation_lo = byte;
               break;
           case 6:
               speed_hi = byte;
               break;
           case 7:
               speed_lo = byte;
               break;
           case 8: {
               stop = byte;
               // Bits in the stop byte indicate that the corresponding field
               // should be incremented from 254 to 255
               for (int i = 1; i < 8; i++) {
                   if ((stop & (1 << (i - 1))) != 0) {
                       read_byte(255, i);
                   }
               }
               break;
           }
           default:
               break;
       }
   }

   uint8_t command = 0;
   uint8_t azimuth_hi = 0;
   uint8_t azimuth_lo = 0;
   uint8_t elevation_hi = 0;
   uint8_t elevation_lo = 0;
   uint8_t speed_hi = 0;
   uint8_t speed_lo = 0;
   uint8_t stop = 0;
   static constexpr uint8_t PACKET_SIZE = 9;
};

#endif  // _PACKET_H_

