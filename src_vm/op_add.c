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
** <opcode> <FLAG> <REG> <REG | DIR8 | DIR16>
*/

static void op_add_reg_reg(void)
{
  char reg1, reg2;
  uint16_t *p1, *p2;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  reg2 = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > add r%d, r%d\n", RED, core.memory.pc, ENDC, reg1, reg2);

  p1 = get_reg(reg1);
  p2 = get_reg(reg2);

  *p1 = *p1 + *p2;
  if (*p1 == 0)
    core.regs.zf = 1;
  else
    core.regs.zf = 0;

  core.memory.pc += 4;
}

static void op_add_reg_dir08(void)
{
  char reg1;
  uint8_t operande;
  uint16_t *p1;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > addb r%d, #%d\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);
  *p1 = *p1 + operande;
  if (*p1 == 0)
    core.regs.zf = 1;
  else
    core.regs.zf = 0;

  core.memory.pc += 4;
}

static void op_add_reg_dir16(void)
{
  char reg1;
  uint16_t *p1;
  uint16_t operande;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  operande = *(uint16_t *)(core.memory.vmem + core.memory.pc + 3);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > addl r%d, #0x%x\n", RED, core.memory.pc, ENDC, reg1, operande);

  p1 = get_reg(reg1);
  *p1 = *p1 + operande;
  if (*p1 == 0)
    core.regs.zf = 1;
  else
    core.regs.zf = 0;

  core.memory.pc += 5;
}

void op_add(void)
{
  char flag;

  flag = *(core.memory.vmem + core.memory.pc + 1);
  if (flag == OP_FLAG_REG_REG)
    op_add_reg_reg();
  else if (flag == OP_FLAG_REG_DIRECT08)
    op_add_reg_dir08();
  else if (flag == OP_FLAG_REG_DIRECT16)
    op_add_reg_dir16();
  else
    segfault(2); /* flag unknow */
}
