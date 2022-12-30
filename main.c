#include <fake6502.h>
#include <unistd.h>

#include "mm6502.h"
#include "memory.h"
#include "log.h"
#include "repl.h"
#include "via.h"
#include "acia.h"

fake6502_context context;
mm6502_memory memory;
mm6502_memory rom;
mm6522_via via;
mm6551_acia acia;

uint8_t mm6502_fallback_read_callback(fake6502_context *context, uint16_t address, void *data) {
  warn("read from unmapped segment, returning 0: address=%04x", address);
  return 0;
}

void mm6502_fallback_write_callback(fake6502_context *context, uint16_t address, uint8_t value, void *data) {
  warn("write to unmapped segment: address=%04x value=%02x", address, value);
}

mm6502_read_callback mm6502_read_callbacks[] = {
  { .from = 0x0000, .to = 0x3fff, .fn = mm6502_mem_read_callback,  .data = (void *)&memory },
  { .from = 0x4000, .to = 0x5fff, .fn = mm6502_acia_read_callback, .data = (void *)&acia },
  { .from = 0x6000, .to = 0x6003, .fn = mm6502_via_read_callback,  .data = (void *)&via },
  { .from = 0x8000, .to = 0xffff, .fn = mm6502_mem_read_callback,  .data = (void *)&rom },
  { .from = 0x0000, .to = 0x7fff, .fn = mm6502_fallback_read_callback }
};

mm6502_write_callback mm6502_write_callbacks[] = {
  { .from = 0x0000, .to = 0x3fff, .fn = mm6502_mem_write_callback,  .data = (void *)&memory },
  { .from = 0x4000, .to = 0x5fff, .fn = mm6502_acia_write_callback, .data = (void *)&acia },
  { .from = 0x6000, .to = 0x6003, .fn = mm6502_via_write_callback,  .data = (void *)&via },
  { .from = 0x0000, .to = 0x7fff, .fn = mm6502_fallback_write_callback }
};

mm6502_step_callback mm6502_step_callbacks[] = {
  { .data = (void *)&acia, .fn = mm6551_step_callback },
  { .fn = NULL }
};

#define USAGE() \
  fprintf(stderr, "Usage: %s [-v] [-b] [-p address] rom\n", argv[0]); \
  exit(EXIT_FAILURE);

int main(int argc, char *argv[])
{
  via.address = 0x6000;
  acia.address = 0x4000;
  acia.status = ACIA_STATUS_TXE;
  mm6551_init(&acia);
  mm6502_mem_init(&memory, 0x10000, 0x0000);
  mm6502_mem_init(&rom,    0x8000, 0x8000);

  int opt;
  int dobreak = 0, verbose = 0, spc = -1;
  while ((opt = getopt(argc, argv, "vbp:")) != -1) {
    switch (opt) {
    case 'b':
      dobreak = 1;
      break;
    case 'p':
      sscanf(optarg, "%x", (unsigned int *)&spc);
      break;
    case 'v':
      verbose = 1;
      break;
    default:
      USAGE();
    }
  }

  if (optind == 0 || optind >= argc) {
    USAGE();
  }

  char *rompath = argv[optind];
  mm6502_mem_load(&rom, 0x0000, rompath);

  fake6502_reset(&context);
  if(spc > -1) {
    context.cpu.pc = spc;
  }

  if (dobreak) {
    mm6502_repl(&context, verbose);
  } else {
    while (1) {
      fake6502_step(&context);
    }
  }
}
