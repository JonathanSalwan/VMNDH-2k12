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
** <opcode> <REG>
*/

void op_pop(void)
{
  char operande;
  uint16_t *reg;

  operande = *(core.memory.vmem + core.memory.pc + 1);
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > pop r%d\n", RED, core.memory.pc, ENDC, operande);

  reg = get_reg(operande);
  *reg = *(uint16_t *)(core.memory.vmem + core.memory.sp);

  core.memory.sp += 2; /* add sp */
  core.memory.pc += 2;
}
