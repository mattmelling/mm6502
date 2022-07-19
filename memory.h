#ifndef MEMORY_H

#include <stdint.h>
#include <fake6502.h>

#include "log.h"

typedef struct mm6502_memory mm6502_memory;

struct mm6502_memory {
  uint8_t *data;
  uint16_t offset;
  size_t size;
};

void mm6502_mem_init(mm6502_memory *memory, size_t size, uint16_t offset);
void mm6502_mem_load(mm6502_memory *memory, uint16_t start, char *filename);
uint8_t mm6502_mem_read_callback(fake6502_context *context, uint16_t address, void *data);
void mm6502_mem_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data);

#define MEMORY_H
#endif
