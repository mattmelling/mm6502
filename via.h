#include <stdint.h>
#include <fake6502.h>

typedef struct mm6522_via mm6522_via;
typedef struct mm6522_registers mm6522_registers;

typedef uint8_t (mm6522_via_read_callback_fn)(mm6522_via *via, uint8_t reg);
typedef void (mm6522_via_write_callback_fn)(mm6522_via *via, uint8_t reg, uint8_t value);

#define VIA_REG_RB   0
#define VIA_REG_RA   1
#define VIA_REG_DDRB 2
#define VIA_REG_DDRA 3

struct mm6522_registers {
  uint8_t RB;
  uint8_t RA;
  uint8_t DDRB;
  uint8_t DDRA;
};

struct mm6522_via {
  mm6522_registers registers;
  uint16_t address;
  mm6522_via_read_callback_fn *read_callback;
  mm6522_via_write_callback_fn *write_callback;
};

uint8_t mm6502_via_read_callback(fake6502_context *context, uint16_t address, void *data);
void mm6502_via_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data);
