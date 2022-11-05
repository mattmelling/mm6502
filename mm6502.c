#include <stdio.h>
#include <fake6502.h>
#include "mm6502.h"
#include "log.h"

uint8_t fake6502_mem_read(fake6502_context *context, uint16_t address)
{
  trace("fake6502_mem_read(%p, %04x)", (void *)context, address);
  mm6502_read_callback *callback = mm6502_read_callbacks;
  while(!(address >= callback->from && address <= callback->to)) {
    callback++;
  }
  return callback->fn(context, address, callback->data);
}

void fake6502_mem_write(fake6502_context *context, uint16_t address, uint8_t value)
{
  trace("fake6502_mem_write(%p, %04x, %02x)", (void *)context, address, value);
  mm6502_write_callback *callback = mm6502_write_callbacks;
  while(!(address >= callback->from && address <= callback->to)) {
      callback++;
  }
  callback->fn(context, address, value, callback->data);
}

void mm6502_step(fake6502_context *context)
{
  fake6502_step(context);
  mm6502_step_callback *callback = mm6502_step_callbacks;
  while(callback->fn != NULL) {
    callback->fn(context, callback->data);
    callback++;
  }
}
