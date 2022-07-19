#include <stdio.h>
#include <stdint.h>

#include "dism.h"

static char* optable_c[256] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |      */
/* 0 */      "brk",  "ora",  "nop",  "slo",  "nop",  "ora",  "asl",  "slo",  "php",  "ora",  "asl",  "nop",  "nop",  "ora",  "asl",  "slo", /* 0 */
/* 1 */      "bpl",  "ora",  "nop",  "slo",  "nop",  "ora",  "asl",  "slo",  "clc",  "ora",  "nop",  "slo",  "nop",  "ora",  "asl",  "slo", /* 1 */
/* 2 */      "jsr",  "and",  "nop",  "rla",  "bit",  "and",  "rol",  "rla",  "plp",  "and",  "rol",  "nop",  "bit",  "and",  "rol",  "rla", /* 2 */
/* 3 */      "bmi",  "and",  "nop",  "rla",  "nop",  "and",  "rol",  "rla",  "sec",  "and",  "nop",  "rla",  "nop",  "and",  "rol",  "rla", /* 3 */
/* 4 */      "rti",  "eor",  "nop",  "sre",  "nop",  "eor",  "lsr",  "sre",  "pha",  "eor",  "lsr",  "nop",  "jmp",  "eor",  "lsr",  "sre", /* 4 */
/* 5 */      "bvc",  "eor",  "nop",  "sre",  "nop",  "eor",  "lsr",  "sre",  "cli",  "eor",  "phy",  "sre",  "nop",  "eor",  "lsr",  "sre", /* 5 */
/* 6 */      "rts",  "adc",  "nop",  "rra",  "nop",  "adc",  "ror",  "rra",  "pla",  "adc",  "ror",  "nop",  "jmp",  "adc",  "ror",  "rra", /* 6 */
/* 7 */      "bvs",  "adc",  "nop",  "rra",  "nop",  "adc",  "ror",  "rra",  "sei",  "adc",  "ply",  "rra",  "nop",  "adc",  "ror",  "rra", /* 7 */
/* 8 */      "nop",  "sta",  "nop",  "sax",  "sty",  "sta",  "stx",  "sax",  "dey",  "nop",  "txa",  "nop",  "sty",  "sta",  "stx",  "sax", /* 8 */
/* 9 */      "bcc",  "sta",  "nop",  "nop",  "sty",  "sta",  "stx",  "sax",  "tya",  "sta",  "txs",  "nop",  "nop",  "sta",  "nop",  "nop", /* 9 */
/* A */      "ldy",  "lda",  "ldx",  "lax",  "ldy",  "lda",  "ldx",  "lax",  "tay",  "lda",  "tax",  "nop",  "ldy",  "lda",  "ldx",  "lax", /* A */
/* B */      "bcs",  "lda",  "nop",  "lax",  "ldy",  "lda",  "ldx",  "lax",  "clv",  "lda",  "tsx",  "lax",  "ldy",  "lda",  "ldx",  "lax", /* B */
/* C */      "cpy",  "cmp",  "nop",  "dcp",  "cpy",  "cmp",  "dec",  "dcp",  "iny",  "cmp",  "dex",  "nop",  "cpy",  "cmp",  "dec",  "dcp", /* C */
/* D */      "bne",  "cmp",  "nop",  "dcp",  "nop",  "cmp",  "dec",  "dcp",  "cld",  "cmp",  "phx",  "dcp",  "nop",  "cmp",  "dec",  "dcp", /* D */
/* E */      "cpx",  "sbc",  "nop",  "isb",  "cpx",  "sbc",  "inc",  "isb",  "inx",  "sbc",  "nop",  "sbc",  "cpx",  "sbc",  "inc",  "isb", /* E */
/* F */      "beq",  "sbc",  "nop",  "isb",  "nop",  "sbc",  "inc",  "isb",  "sed",  "sbc",  "plx",  "isb",  "nop",  "sbc",  "inc",  "isb"  /* F */
};

#define imp  (uint8_t) 1
#define rel  (uint8_t) 2
#define abso (uint8_t) 3
#define indx (uint8_t) 5
#define indy (uint8_t) 6
#define absx (uint8_t) 7
#define zp   (uint8_t) 8
#define zpx  (uint8_t) 9
#define zpy  (uint8_t) 10
#define ind  (uint8_t) 11
#define absy (uint8_t) 12
#define acc  (uint8_t) 13
#define imm  (uint8_t) 14
#define isb  (uint8_t) 15

static uint8_t addrtable[256] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |     */
/* 0 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 0 */
/* 1 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 1 */
/* 2 */    abso, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 2 */
/* 3 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 3 */
/* 4 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm, abso, abso, abso, abso, /* 4 */
/* 5 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 5 */
/* 6 */     imp, indx,  imp, indx,   zp,   zp,   zp,   zp,  imp,  imm,  acc,  imm,  ind, abso, abso, abso, /* 6 */
/* 7 */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* 7 */
/* 8 */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* 8 */
/* 9 */     rel, indy,  imp, indy,  zpx,  zpx,  zpy,  zpy,  imp, absy,  imp, absy, absx, absx, absy, absy, /* 9 */
/* A */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* A */
/* B */     rel, indy,  imp, indy,  zpx,  zpx,  zpy,  zpy,  imp, absy,  imp, absy, absx, absx, absy, absy, /* B */
/* C */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* C */
/* D */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx, /* D */
/* E */     imm, indx,  imm, indx,   zp,   zp,   zp,   zp,  imp,  imm,  imp,  imm, abso, abso, abso, abso, /* E */
/* F */     rel, indy,  imp, indy,  zpx,  zpx,  zpx,  zpx,  imp, absy,  imp, absy, absx, absx, absx, absx  /* F */
};

uint16_t mm6502_dism(fake6502_context *context, uint16_t offset, char buffer[64]) {
  uint8_t i = fake6502_mem_read(context, offset); // instruction
  char* i_c = optable_c[i]; // instruction string
  uint8_t a = addrtable[i]; // addressing mode
  uint8_t pc1, pc2;
  switch(a) {
  case imp:
    sprintf(buffer, "%s          ; %02x", i_c, i);
    break;
  case rel:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %02x       ; %02x %02x", i_c, pc1, i, pc1);
    break;
  case abso:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %04x     ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case indx:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s (%04x,x) ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case indy:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s (%04x),y ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case absx:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %04x,x   ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case zp:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %02x     ; %02x %02x", i_c, pc1, i, pc1);
    break;
  case zpx:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %02x,x   ; %02x %02x", i_c, pc1, i, pc1);
    break;
  case zpy:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %02x,y   ; %02x %02x", i_c, pc1, i, pc1);
    break;
  case ind:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s (%04x)   ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case absy:
    pc1 = fake6502_mem_read(context, ++offset);
    pc2 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %04x,y   ; %02x %02x %02x", i_c, (pc2 << 8) + pc1, i, pc1, pc2);
    break;
  case acc:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s %02x     ; %02x %02x", i_c, pc1, i, pc1);
    break;
  case imm:
    pc1 = fake6502_mem_read(context, ++offset);
    sprintf(buffer, "%s #%02x      ; %02x %02x", i_c, pc1, i, pc1);
    break;
  }
  return ++offset;
}
