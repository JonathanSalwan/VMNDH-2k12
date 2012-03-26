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

/*
** <opcode> <FLAG> <REG | REG_INDIRECT> <REG | REG_INDIRECT | DIR8 | DIR16>
*/

static void op_mov_reg_reg(void)
{
  char reg1, reg2;
  uint16_t *p1, *p2;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  reg2 = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > mov r%d, r%d\n", RED, core.memory.pc, ENDC, reg1, reg2);

  p1 = get_reg(reg1);
  p2 = get_reg(reg2);

  *p1 = *p2;

  core.memory.pc += 4;
}

static void op_mov_reg_dir08(void)
{
  char reg1;
  uint8_t operande;
  uint16_t *p1;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > movb r%d, #%d\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);
  *p1 = operande;

  core.memory.pc += 4;
}

static void op_mov_reg_dir16(void)
{
  char reg1;
  uint16_t *p1;
  uint16_t operande;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(uint16_t *)(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > movl r%d, #0x%x\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);
  *p1 = operande;

  core.memory.pc += 5;
}

static void op_mov_regindirect_reg(void)
{
  char reg1, reg2;
  uint16_t *p1, *p2;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  reg2 = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > mov [r%d], r%d\n", RED, core.memory.pc, ENDC, reg1, reg2);

  p1 = get_reg(reg1);
  p2 = get_reg(reg2);

  if (*p1 >= (core.memory.pc_pie_base))
    segfault(8);

  *(core.memory.vmem + *p1) = *p2;

  core.memory.pc += 4;
}

static void op_mov_regindirect_dir08(void)
{
  char reg1;
  uint8_t operande;
  uint16_t *p1;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > movb [r%d], #%d\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);

  if (*p1 >= (core.memory.pc_pie_base))
    segfault(8);

  *(core.memory.vmem + *p1) = operande;

  core.memory.pc += 4;
}

static void op_mov_regindirect_dir16(void)
{
  char reg1;
  uint16_t operande;
  uint16_t *p1;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(uint16_t *)(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > movl [r%d], #0x%x\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);

  if (*p1 >= (core.memory.pc_pie_base))
    segfault(8);

  *(uint16_t *)(core.memory.vmem + *p1) = operande;

  core.memory.pc += 5;
}

static void op_mov_regindirect_regindirect(void)
{
  char reg1, reg2;
  uint16_t *p1, *p2;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  reg2 = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > movl [r%d], [r%d]\n", RED, core.memory.pc, ENDC, reg1, reg2);

  p1 = get_reg(reg1);
  p2 = get_reg(reg2);

  /*
  if (*p1 > (core.memory.pc_pie_base) || *p2 > (core.memory.pc_pie_base))
    segfault(8);
  */
  if (*p1 >= core.memory.pc_pie_base)
    segfault(8);

  *(core.memory.vmem + *p1) = *(core.memory.vmem + *p2);

  core.memory.pc += 4;
}

static void op_mov_reg_regindirect(void)
{
  char reg1, reg2;
  uint16_t *p1, *p2;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  reg2 = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > mov r%d, [r%d]\n", RED, core.memory.pc, ENDC, reg1, reg2);

  p1 = get_reg(reg1);
  p2 = get_reg(reg2);

  /*if (*p2 > (core.memory.pc_pie_base))
    segfault(8);*/

  *p1 = *(core.memory.vmem + *p2);

  core.memory.pc += 4;
}

void op_mov(void)
{
  char flag;

  flag = *(core.memory.vmem + core.memory.pc + 1);
  if (flag == OP_FLAG_REG_REG)
    op_mov_reg_reg();
  else if (flag == OP_FLAG_REG_DIRECT08)
    op_mov_reg_dir08();
  else if (flag == OP_FLAG_REG_DIRECT16)
    op_mov_reg_dir16();
  else if (flag == OP_FLAG_REGINDIRECT_REG)
    op_mov_regindirect_reg();
  else if (flag == OP_FLAG_REGINDIRECT_DIRECT08)
    op_mov_regindirect_dir08();
  else if (flag == OP_FLAG_REGINDIRECT_DIRECT16)
    op_mov_regindirect_dir16();
  else if (flag == OP_FLAG_REGINDIRECT_REGINDIRECT)
    op_mov_regindirect_regindirect();
  else if (flag == OP_FLAG_REG_REGINDIRECT)
    op_mov_reg_regindirect();
  else
    segfault(2); /* flag unknow */
}
