#include <stdint.h>
#include <poll.h>
#include <fake6502.h>

#define ACIA_REG_BASE    0
#define ACIA_REG_STATUS  1
#define ACIA_REG_COMMAND 2
#define ACIA_REG_CONTROL 3

#define ACIA_STATUS_IRQ 0x80
#define ACIA_STATUS_DSR 0x40
#define ACIA_STATUS_DCD 0x20
#define ACIA_STATUS_TXE 0x10
#define ACIA_STATUS_RXF 0x08
#define ACIA_STATUS_OVR 0x04
#define ACIA_STATUS_FRM 0x02
#define ACIA_STATUS_PAR 0x01

#define ACIA_COMMAND_PMCH 0x80
#define ACIA_COMMAND_PMCL 0x40
#define ACIA_COMMAND_PME  0x20
#define ACIA_COMMAND_REM  0x10
#define ACIA_COMMAND_TICH 0x08
#define ACIA_COMMAND_TICL 0x04
#define ACIA_COMMAND_IRD  0x02
#define ACIA_COMMAND_DTR  0x01

typedef struct mm6551_acia mm6551_acia;

struct mm6551_acia {
  uint16_t address;
  uint8_t status;
  uint8_t command;
  uint8_t control;
  uint8_t tx;
  uint8_t rx;
  struct pollfd fd;
};

uint8_t mm6502_acia_read_callback(fake6502_context *context, uint16_t address, void *data);
void mm6502_acia_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data);
void mm6551_step_callback(fake6502_context *context, void *data);

void mm6551_init(mm6551_acia *acia);
