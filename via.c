#include "via.h"
#include "log.h"

#define VIA_LOG_READ(via, reg) debug("via@%04x read %s", via->address, reg)
#define VIA_LOG_WRITE(via, reg, value) debug("via@%04x write %s = %02x", via->address, reg, value)

uint8_t mm6502_via_read_callback(fake6502_context *context, uint16_t address, void *data)
{
  mm6522_via *via = (mm6522_via *)data;
  uint16_t reg = address - via->address;
  uint8_t ret = 0;
  switch(reg) {
  case VIA_REG_RB:
    VIA_LOG_READ(via, "IRB");
    ret = via->registers.RB & via->registers.DDRB;
    break;
  case VIA_REG_RA:
    VIA_LOG_READ(via, "IRA");
    ret = via->registers.RA & via->registers.DDRA;
    break;
  case VIA_REG_DDRB:
    VIA_LOG_READ(via, "DDRB");
    ret = via->registers.DDRB;
  case VIA_REG_DDRA:
    VIA_LOG_READ(via, "DDRA");
    ret = via->registers.DDRA;
    break;
  default:
    warn("via@%04x register write %02x not implemented", via->address, address);
    break;
  }
  return ret;
}

void mm6502_via_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data)
{
  mm6522_via *via = (mm6522_via *)data;
  uint16_t reg = address - via->address;
  switch(reg) {
  case VIA_REG_RB:
    VIA_LOG_WRITE(via, "ORB", value);
    via->registers.RB = value & via->registers.DDRB;
    break;
  case VIA_REG_RA:
    VIA_LOG_WRITE(via, "ORA", value);
    via->registers.RA = value & via->registers.DDRA;
    break;
  case VIA_REG_DDRB:
    VIA_LOG_WRITE(via, "DDRB", value);
    via->registers.DDRB = value;
    break;
  case VIA_REG_DDRA:
    VIA_LOG_WRITE(via, "DDRA", value);
    via->registers.DDRA = value;
    break;
  default:
    warn("via@%04x register write %02x not implemented", via->address, address);
    break;
  }
}
