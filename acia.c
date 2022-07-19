#include <stdio.h>
#include "acia.h"
#include "log.h"

#define ACIA_LOG_READ(acia, reg) debug("acia@%04x read %s", acia->address, reg)
#define ACIA_LOG_WRITE(acia, reg, value) debug("acia@%04x write %s = %02x", acia->address, reg, value)

uint8_t mm6502_acia_read_callback(fake6502_context *context, uint16_t address, void *data)
{
  mm6551_acia *acia = (mm6551_acia *)data;
  uint16_t reg = address - acia->address;
  switch(reg) {
  case ACIA_REG_BASE:
    ACIA_LOG_READ(acia, "BASE");
    break;
  case ACIA_REG_STATUS:
    ACIA_LOG_READ(acia, "STATUS");
    break;
  case ACIA_REG_COMMAND:
    ACIA_LOG_READ(acia, "COMMAND");
    break;
  case ACIA_REG_CONTROL:
    ACIA_LOG_READ(acia, "CONTROL");
    break;
  }
  return 0;
}

void mm6502_acia_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data)
{
  mm6551_acia *acia = (mm6551_acia *)data;
  uint16_t reg = address - acia->address;
  switch(reg) {
  case ACIA_REG_BASE:
    ACIA_LOG_WRITE(acia, "BASE", value);
    break;
  case ACIA_REG_STATUS:
    ACIA_LOG_WRITE(acia, "STATUS", value);
    break;
  case ACIA_REG_COMMAND:
    ACIA_LOG_WRITE(acia, "COMMAND", value);
    break;
  case ACIA_REG_CONTROL:
    ACIA_LOG_WRITE(acia, "CONTROL", value);
    break;
  }
}

void mm6551_init(mm6551_acia *acia)
{

}

void mm6551_step_callback(fake6502_context *context)
{
  /* if(!kbhit()) { */
  /*   return; */
  /* } */
  /* int c = getc(stdin); */
  /* if(c == EOF) { */
  /*   return; */
  /* } */
  /* printf(">>> %c\n", c); */
}
