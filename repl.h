#include <fake6502.h>

#define MEM_DUMP_WIDTH 16

#define FLAG_CHAR(context, N, hi, lo) (context->cpu.flags & (1 << (N))) ? hi : lo
#define FLAG_C(context) FLAG_CHAR(context, 1, 'c', '-')
#define FLAG_Z(context) FLAG_CHAR(context, 2, 'z', '-')
#define FLAG_I(context) FLAG_CHAR(context, 3, 'i', '-')
#define FLAG_D(context) FLAG_CHAR(context, 4, 'd', '-')
#define FLAG_V(context) FLAG_CHAR(context, 6, 'v', '-')
#define FLAG_N(context) FLAG_CHAR(context, 7, 'n', '-')

void mm6502_repl(fake6502_context *context);
