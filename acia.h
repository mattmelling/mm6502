#include <stdint.h>
#include <fake6502.h>

#define ACIA_REG_BASE    0
#define ACIA_REG_STATUS  1
#define ACIA_REG_COMMAND 2
#define ACIA_REG_CONTROL 3

typedef struct mm6551_registers mm6551_registers;
typedef struct mm6551_acia mm6551_acia;

struct mm6551_registers {
  uint8_t BASE;
  uint8_t STATUS;
  uint8_t COMMAND;
  uint8_t CONTROL;
};

struct mm6551_acia {
  mm6551_registers registers;
  uint16_t address;
};

uint8_t mm6502_acia_read_callback(fake6502_context *context, uint16_t address, void *data);
void mm6502_acia_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data);
void mm6551_step_callback(fake6502_context *context);
