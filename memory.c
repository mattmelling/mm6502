#include <stdlib.h>

#include "memory.h"

void mm6502_mem_init(mm6502_memory *memory, size_t size, uint16_t offset)
{
  info("init memory segment %04x-%04x", (uint16_t)size, (uint16_t)(offset + (size - 1)));
  memory->size = size;
  memory->offset = offset;
  memory->data = (uint8_t *)malloc(sizeof(uint8_t) * size);
}

uint8_t mm6502_mem_read_callback(fake6502_context *context, uint16_t address, void *data) {
  mm6502_memory *memory = (mm6502_memory *)data;
  uint8_t val = memory->data[address - memory->offset];
  trace("mm6502_mem_read_callback(%04x) = %02x", address, val);
  return val;
}

void mm6502_mem_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data) {
  trace("mm6502_mem_write_callback(%04x, %02x)", address, value);
  mm6502_memory *memory = (mm6502_memory *)data;
  memory->data[address - memory->offset] = value;
}

void mm6502_mem_load(mm6502_memory *memory, uint16_t start, char *filename)
{
  info("loading %s to %04x", filename, start + memory->offset);
  FILE *f = fopen(filename, "r");
  int count;
  uint8_t *m = memory->data + start;
  size_t max = memory->size - start;
  while((count = fread(m, 1, max, f)) > 0) {
    m += count;
    max -= count;
  }
  fclose(f);
}
