/*
** vmndh - Release v0.1
** Jonathan Salwan - http://twitter.com/JonathanSalwan
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "vmndh.h"

static uint16_t dis_push(uint16_t);
static uint16_t dis_pop(uint16_t);
static uint16_t dis_mov(uint16_t);
static uint16_t dis_add(uint16_t);
static uint16_t dis_sub(uint16_t);
static uint16_t dis_mul(uint16_t);
static uint16_t dis_div(uint16_t);
static uint16_t dis_inc(uint16_t);
static uint16_t dis_dec(uint16_t);
static uint16_t dis_or(uint16_t);
static uint16_t dis_and(uint16_t);
static uint16_t dis_xor(uint16_t);
static uint16_t dis_not(uint16_t);
static uint16_t dis_jz(uint16_t);
static uint16_t dis_jnz(uint16_t);
static uint16_t dis_jmps(uint16_t);
static uint16_t dis_test(uint16_t);
static uint16_t dis_cmp(uint16_t);
static uint16_t dis_call(uint16_t);
static uint16_t dis_ret(uint16_t);
static uint16_t dis_jmpl(uint16_t);
static uint16_t dis_end(uint16_t);
static uint16_t dis_xchg(uint16_t);
static uint16_t dis_ja(uint16_t);
static uint16_t dis_jb(uint16_t);
static uint16_t dis_syscall(uint16_t);
static uint16_t dis_nop(uint16_t);

t_disass tabDIS[] = {
      {"push",    OP_PUSH,     &dis_push},
      {"pop",     OP_POP,      &dis_pop},
      {"mov",     OP_MOV,      &dis_mov},
      {"add",     OP_ADD,      &dis_add},
      {"sub",     OP_SUB,      &dis_sub},
      {"mul",     OP_MUL,      &dis_mul},
      {"div",     OP_DIV,      &dis_div},
      {"inc",     OP_INC,      &dis_inc},
      {"dec",     OP_DEC,      &dis_dec},
      {"or",      OP_OR,       &dis_or},
      {"and",     OP_AND,      &dis_and},
      {"xor",     OP_XOR,      &dis_xor},
      {"not",     OP_NOT,      &dis_not},
      {"jz",      OP_JZ,       &dis_jz},
      {"jnz",     OP_JNZ,      &dis_jnz},
      {"jmps",    OP_JMPS,     &dis_jmps},
      {"test",    OP_TEST,     &dis_test},
      {"cmp",     OP_CMP,      &dis_cmp},
      {"call",    OP_CALL,     &dis_call},
      {"ret",     OP_RET,      &dis_ret},
      {"jmpl",    OP_JMPL,     &dis_jmpl},
      {"end",     OP_END,      &dis_end},
      {"xchg",    OP_XCHG,     &dis_xchg},
      {"ja",      OP_JA,       &dis_ja},
      {"jb",      OP_JB,       &dis_jb},
      {"syscall", OP_SYSCALL,  &dis_syscall},
      {"nop",     OP_NOP,      &dis_nop},
      {NULL, 0, NULL}
};

static uint16_t dis_push(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        fprintf(stdout, "%s0x%.4x%s: push r%d\n", RED, addr, ENDC, reg);
        return (addr + 4);

      case OP_FLAG_DIRECT08:
        dir8 = *(core.memory.vmem + addr + 2);
        fprintf(stdout, "%s0x%.4x%s: pushb #%.2x\n", RED, addr, ENDC, dir8);
        return (addr + 4);

      case OP_FLAG_DIRECT16:
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 2);
        fprintf(stdout, "%s0x%.4x%s: pushl #%.4x\n", RED, addr, ENDC, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (push error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_pop(uint16_t addr)
{
  uint8_t operande;

  operande = *(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: pop r%d\n", RED, addr, ENDC, operande);

  return (addr + 2);
}

static uint16_t dis_mov(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: movb r%d, #0x%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg  = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: movl r%d, #0x%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      case OP_FLAG_REGINDIRECT_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov [r%d], r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REGINDIRECT_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov [r%d], #0x%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REGINDIRECT_DIRECT16:
        reg  = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov [r%d], #0x%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      case OP_FLAG_REGINDIRECT_REGINDIRECT:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov [r%d], [r%d]\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_REGINDIRECT:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mov r%d, [r%d]\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      default:
        fprintf(stdout, "%s0x%.4x%s: (mov error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_add(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: add r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: addb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: addl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (add error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_sub(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: sub r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: subb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: subl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (sub error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_mul(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mul r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mulb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: mull r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (mul error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_div(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: div r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: divb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: divl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (div error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_inc(uint16_t addr)
{
  uint8_t operande;

  operande = *(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: inc r%d\n", RED, addr, ENDC, operande);

  return (addr + 2);
}

static uint16_t dis_dec(uint16_t addr)
{
  uint8_t operande;

  operande = *(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: dec r%d\n", RED, addr, ENDC, operande);

  return (addr + 2);
}

static uint16_t dis_or(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: or r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: orb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: orl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (or error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_and(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: and r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: andb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: andl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (and error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_xor(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: xor r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: xorb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: xorl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (xor error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_not(uint16_t addr)
{
  uint8_t operande;

  operande = *(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: not r%d\n", RED, addr, ENDC, operande);

  return (addr + 2);
}

static uint16_t dis_jz(uint16_t addr)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: jz 0x%.4x\n", RED, addr, ENDC, operande);

  return (addr + 3);
}

static uint16_t dis_jnz(uint16_t addr)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: jnz 0x%.4x\n", RED, addr, ENDC, operande);

  return (addr + 3);
}

static uint16_t dis_jmps(uint16_t addr)
{
  int8_t operande1;

  operande1 = *(uint8_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: jmps 0x%.2x\n", RED, addr, ENDC, (uint8_t)operande1);

  return (addr + 2);
}

static uint16_t dis_test(uint16_t addr)
{
  uint8_t operande1;
  uint8_t operande2;

  operande1 = *(uint8_t *)(core.memory.vmem + addr + 1);
  operande2 = *(uint8_t *)(core.memory.vmem + addr + 2);
  fprintf(stdout, "%s0x%.4x%s: test r%d, r%d\n", RED, addr, ENDC, operande1, operande2);

  return (addr + 3);
}

static uint16_t dis_cmp(uint16_t addr)
{
  uint16_t  dir16;
  uint8_t   dir8;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG_REG:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: cmp r%d, r%d\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT08:
        reg  = *(core.memory.vmem + addr + 2);
        dir8 = *(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: cmpb r%d, #%.2x\n", RED, addr, ENDC, reg, dir8);
        return (addr + 4);

      case OP_FLAG_REG_DIRECT16:
        reg = *(core.memory.vmem + addr + 2);
        dir16 = *(uint16_t *)(core.memory.vmem + addr + 3);
        fprintf(stdout, "%s0x%.4x%s: cmpl r%d, #%.4x\n", RED, addr, ENDC, reg, dir16);
        return (addr + 5);

      default:
        fprintf(stdout, "%s0x%.4x%s: (cmp error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_call(uint16_t addr)
{
  uint16_t  operande;
  uint8_t   reg;
  char      flag;

  flag = *(core.memory.vmem + addr + 1);
  switch (flag)
    {
      case OP_FLAG_REG:
        reg = *(core.memory.vmem + addr + 2);
        fprintf(stdout, "%s0x%.4x%s: call r%d\n", RED, addr, ENDC, reg);
        return (addr + 3);

      case OP_FLAG_DIRECT16:
        operande = *(uint16_t *)(core.memory.vmem + addr + 2);
        fprintf(stdout, "%s0x%.4x%s: call 0x%.4x\n", RED, addr, ENDC, operande);
        return (addr + 4);

      default:
        fprintf(stdout, "%s0x%.4x%s: (call error flag)\n", RED, addr, ENDC);
        return (addr + 4);
    }
}

static uint16_t dis_ret(uint16_t addr)
{
  fprintf(stdout, "%s0x%.4x%s: ret\n", RED, addr, ENDC);
  return (addr + 1);
}

static uint16_t dis_jmpl(uint16_t addr)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: jmpl 0x%.4x\n", RED, addr, ENDC, operande);

  return (addr + 3);
}

static uint16_t dis_end(uint16_t addr)
{
  fprintf(stdout, "%s0x%.4x%s: end\n", RED, addr, ENDC);
  return (addr + 1);
}

static uint16_t dis_xchg(uint16_t addr)
{
  uint8_t operande1;
  uint8_t operande2;

  operande1 = *(uint8_t *)(core.memory.vmem + addr + 1);
  operande2 = *(uint8_t *)(core.memory.vmem + addr + 2);
  fprintf(stdout, "%s0x%.4x%s: xchg r%d, r%d\n", RED, addr, ENDC, operande1, operande2);

  return (addr + 3);
}

static uint16_t dis_ja(uint16_t addr)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: ja 0x%.4x\n", RED, addr, ENDC, operande);

  return (addr + 3);
}

static uint16_t dis_jb(uint16_t addr)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + addr + 1);
  fprintf(stdout, "%s0x%.4x%s: jb 0x%.4x\n", RED, addr, ENDC, operande);

  return (addr + 3);
}

static uint16_t dis_syscall(uint16_t addr)
{
  fprintf(stdout, "%s0x%.4x%s: syscall\n", RED, addr, ENDC);
  return (addr + 1);
}

static uint16_t dis_nop(uint16_t addr)
{
  fprintf(stdout, "%s0x%.4x%s: nop\n", RED, addr, ENDC);
  return (addr + 1);
}

void disass(uint16_t addr, uint16_t size)
{
  int i, found;
  uint16_t addr_end;
  uint16_t save_addr;
  char OP;

  addr_end = core.memory.pc_pie_base + core.infobin.size_text;
  save_addr = addr;
  while (addr < addr_end && (addr - save_addr) < size)
    {
      i = found = 0;
      OP = *(core.memory.vmem + addr);
      while (tabDIS[i].name != NULL)
        {
          if (tabDIS[i].OP == OP)
            {
              found = 1;
              addr = tabDIS[i].ptrOP(addr);
            }
          i++;
        }
      if (found == 0)
        {
          if (OP >= 20 && OP <= 126)
            fprintf(stdout, "%s0x%.4x%s: .byte 0x%.2x (%c)\n", RED, addr, ENDC, (unsigned char)OP, OP);
          else
            fprintf(stdout, "%s0x%.4x%s: .byte 0x%.2x\n", RED, addr, ENDC, (unsigned char)OP);
          addr++;
        }
    }
}
