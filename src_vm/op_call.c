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
** <opcode> <FLAG> <REG | DIR16>
*/

static void op_call_reg(void)
{
  char reg1;
  uint16_t *p1;

  reg1 = *(core.memory.vmem + core.memory.pc + 2);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > call r%d\n", RED, core.memory.pc, ENDC, reg1);

  p1 = get_reg(reg1);
  core.memory.sp -= 2; /* sub sp */
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = core.memory.pc + 3;
  core.memory.pc = *p1;
}

static void op_call_dir16(void)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + core.memory.pc + 2);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > call 0x%.2x\n", RED, core.memory.pc, ENDC, operande);

  core.memory.sp -= 2; /* sub sp */
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = core.memory.pc + 4;
  core.memory.pc = core.memory.pc + operande + 4;
}

void op_call(void)
{
  char flag;

  flag = *(core.memory.vmem + core.memory.pc + 1);
  if (flag == OP_FLAG_REG)
    op_call_reg();
  else if (flag == OP_FLAG_DIRECT16)
    op_call_dir16();
  else
    segfault(2); /* flag unknow */
}
