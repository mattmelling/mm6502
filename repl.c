#include <stdlib.h>
#include <stdio.h>
#include "repl.h"
#include "dism.h"
#include "log.h"
#include "mm6502.h"

unsigned int mm6502_breakpoint = 0;

void mm6502_dump_memory(fake6502_context *context, uint16_t start, uint16_t count)
{
  if(count == 0) {
    count = MEM_DUMP_WIDTH;
  }
  if(start > (65536 - MEM_DUMP_WIDTH)) {
    start = 65536 - MEM_DUMP_WIDTH;
  }

  for(int byte = start; byte < (start + count); byte++) {
    if(byte % MEM_DUMP_WIDTH == 0) {
      if(byte > 0) {
        printf("\n");
      }
      printf("%04x:", byte);
    }
    printf(" %02x", fake6502_mem_read(context, byte));
  }
  printf("\n");
}

void mm6502_repl_dism(fake6502_context *context, uint16_t start, uint16_t size)
{
  char buffer[64];
  unsigned int offset = start, tmp;
  for(int i = 0; i < size; i++) {
    tmp = mm6502_dism(context, offset, buffer);
    printf("%04x: %s\n", offset, buffer);
    offset = tmp;
  }
}

void mm6502_repl_print_status(fake6502_context *context)
{
  printf("pc=%04x f=%02x %c%c%c%c%c%c%c a=%02x x=%02x y=%02x s=%04x\n",
         context->cpu.pc,
         context->cpu.flags,
         FLAG_N(context), FLAG_V(context),
         FLAG_B(context), FLAG_D(context),
         FLAG_I(context), FLAG_Z(context),
         FLAG_C(context),
         context->cpu.a, context->cpu.x, context->cpu.y,
         context->cpu.s);
  mm6502_repl_dism(context, context->cpu.pc, 1);
}

void mm6502_repl_print_help()
{
  printf("x                   ; reset\n");
  printf("s <steps>           ; step\n");
  printf("r [to]              ; run\n");
  printf("m <start> <length>  ; memory\n");
  printf("z <start> <length>  ; disassemble\n");
  printf("i <address> <value> ; insert\n");
  printf("p <address>         ; set pc\n");
  printf("?                   ; help\n");
}

void mm6502_repl_process(fake6502_context *context, char *buffer, size_t size)
{
  char command;
  int n;
  unsigned int i1 = 0, i2 = 0;

  n = sscanf(buffer, "%c %x %x", &command, &i1, &i2);

  switch(command) {
  case 'x': // reset
    fake6502_reset(context);
    break;
  case 's': // step
    if(i1 == 0) {
      i1 = 1;
    }
    for(i2 = 0; i2 < i1; i2++) {
      mm6502_step(context);
    }
    break;
  case 'r': // run
    while(1) {
      mm6502_step(context);
      if(context->cpu.pc == mm6502_breakpoint) {
        break;
      }
      if(context->cpu.pc == i1) {
        break;
      }
    }
    break;
  case 'm': // memory
    mm6502_dump_memory(context, i1, i2);
    return;
  case 'z': // disassemble
    if(i1 == 0) {
      i1 = context->cpu.pc;
    }
    if(i2 == 0) {
      i2 = 16;
    }
    mm6502_repl_dism(context, i1, i2);
    return;
  case 'i': // insert
    fake6502_mem_write(context, i1, i2);
    return;
  case 'b':
    if(i1 == 0) {
      i1 = context->cpu.pc;
    }
    mm6502_breakpoint = i1;
    printf("breakpoint at %x\n", mm6502_breakpoint);
    return;
  case 'p': // set pc
    context->cpu.pc = i1;
    break;
  case '?': // help
    mm6502_repl_print_help();
    return;
  default:
    mm6502_repl_process(context, "s", 1);
    return;
  }

  mm6502_repl_print_status(context);
}

void mm6502_repl(fake6502_context *context)
{
  char *buffer;
  size_t bufsize = 32;
  size_t characters;

  buffer = (char *)malloc(bufsize * sizeof(char));

  fake6502_reset(context);
  info("cpu reset");
  mm6502_repl_print_status(context);

  while(1) {
    printf("? ");
    characters = getline(&buffer, &bufsize, stdin);
    mm6502_repl_process(context, buffer, characters);
  }
}
