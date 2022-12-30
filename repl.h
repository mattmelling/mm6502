#include <stdbool.h>
#include <fake6502.h>

#define MEM_DUMP_WIDTH 16

#define FLAG_CHAR(context, N, hi, lo) ((context->cpu.flags & N) > 0) ? hi : lo
#define FLAG_N(context) FLAG_CHAR(context, FAKE6502_SIGN_FLAG,      'N', 'n')
#define FLAG_V(context) FLAG_CHAR(context, FAKE6502_OVERFLOW_FLAG,  'V', 'v')
#define FLAG_B(context) FLAG_CHAR(context, FAKE6502_BREAK_FLAG,     'B', 'b')
#define FLAG_D(context) FLAG_CHAR(context, FAKE6502_DECIMAL_FLAG,   'D', 'd')
#define FLAG_I(context) FLAG_CHAR(context, FAKE6502_INTERRUPT_FLAG, 'I', 'i')
#define FLAG_Z(context) FLAG_CHAR(context, FAKE6502_ZERO_FLAG,      'Z', 'z')
#define FLAG_C(context) FLAG_CHAR(context, FAKE6502_CARRY_FLAG,     'C', 'c')

typedef struct mm6502_repl_opts mm6502_repl_opts;

struct mm6502_repl_opts {
  fake6502_context *context;
  bool run;
  bool verbose;
};

void mm6502_repl(mm6502_repl_opts *opts);
