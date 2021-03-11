#include "packet.h"

void PacketIn::read_byte(uint8_t byte, uint8_t idx) {
  switch(idx) {
    case 0:
      // Start byte (not needed)
      break;
    case 1:
      command = byte;
      break;
    case 2:
      elevation_hi = byte;
      break;
    case 3:
      elevation_lo = byte;
      break;
    case 4:
      azimuth_hi = byte;
      break;
    case 5:
      azimuth_lo = byte;
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

uint8_t* PacketOut::serialize() {
  serialized[0] = start;
  serialized[1] = elevation_hi;
  serialized[2] = elevation_lo;
  serialized[3] = azimuth_hi;
  serialized[4] = azimuth_lo;
  serialized[5] = sample_rate;
  serialized[6] = limit_switch_status;
  serialized[7] = error;

  // if a payload byte is the same as the start byte, decrement it, then flag it in the stop byte
  stop = 0;
  for (int i = 0; i < PACKET_SIZE - 1; i++) {
    if (serialized[i + 1] == 255) {
      serialized[i + 1] -= 1;
      stop |= (1 << i);
    }
  }
  serialized[8] = stop;

  return serialized;
}
