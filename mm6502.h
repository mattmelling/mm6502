#include <stdlib.h>
#include <stdint.h>
#include <fake6502.h>

typedef struct mm6502_read_callback mm6502_read_callback;
typedef struct mm6502_write_callback mm6502_write_callback;
typedef struct mm6502_step_callback mm6502_step_callback;

typedef uint8_t (mm6502_read_callback_fn)(fake6502_context *context, uint16_t address, void *data);
typedef void (mm6502_write_callback_fn)(fake6502_context *context, uint16_t address, uint8_t value, void *data);
typedef void (mm6502_step_callback_fn)(fake6502_context *context, void *data);

struct mm6502_read_callback {
  uint16_t from;
  uint16_t to;
  mm6502_read_callback_fn *fn;
  void *data;
};

struct mm6502_write_callback {
  uint16_t from;
  uint16_t to;
  mm6502_write_callback_fn *fn;
  void *data;
};

struct mm6502_step_callback {
  void *data;
  mm6502_step_callback_fn *fn;
};

extern mm6502_read_callback mm6502_read_callbacks[];
extern mm6502_write_callback mm6502_write_callbacks[];
extern mm6502_step_callback mm6502_step_callbacks[];

void mm6502_step(fake6502_context *context);
