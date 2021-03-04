#ifndef _PACKET_H_
#define _PACKET_H_

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
   uint8_t command;
   uint8_t azimuth_hi;
   uint8_t azimuth_lo;
   uint8_t elevation_hi;
   uint8_t elevation_lo;
   uint8_t speed_hi;
   uint8_t speed_lo;
};

#endif  // _PACKET_H_

