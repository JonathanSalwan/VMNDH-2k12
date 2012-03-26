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
** <opcode> <FLAG> <REG | DIR08 | DIR16>
*/

static void op_push_reg(void)
{
  char operande;
  uint16_t *reg;

  operande = *(core.memory.vmem + core.memory.pc + 2);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > push r%d\n", RED, core.memory.pc, ENDC, operande);

  core.memory.sp -= 2; /* sub sp */
  reg = get_reg(operande);
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = *reg;

  core.memory.pc += 3;
}

static void op_push_dir08(void)
{
  char operande;

  operande = *(core.memory.vmem + core.memory.pc + 2);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > pushb #0x%x\n", RED, core.memory.pc, ENDC, operande);

  core.memory.sp -= 1; /* sub sp */
  *(core.memory.vmem + core.memory.sp) = operande;

  core.memory.pc += 3;
}

static void op_push_dir16(void)
{
  uint16_t operande;

  operande = *(uint16_t *)(core.memory.vmem + core.memory.pc + 2);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > pushl #0x%x\n", RED, core.memory.pc, ENDC, operande);

  core.memory.sp -= 2; /* sub sp */
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = operande;

  core.memory.pc += 4;
}

void op_push(void)
{
  char flag;

  flag = *(core.memory.vmem + core.memory.pc + 1);
  if (flag == OP_FLAG_REG)
    op_push_reg();
  else if (flag == OP_FLAG_DIRECT08)
    op_push_dir08();
  else if (flag == OP_FLAG_DIRECT16)
    op_push_dir16();
  else
    segfault(2); /* flag unknow */
}
