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

uint16_t *get_reg(char reg)
{
  uint16_t *p;

  if (reg == REG_0)
    p = &core.regs.r0;
  else if (reg == REG_1)
    p = &core.regs.r1;
  else if (reg == REG_2)
    p = &core.regs.r2;
  else if (reg == REG_3)
    p = &core.regs.r3;
  else if (reg == REG_4)
    p = &core.regs.r4;
  else if (reg == REG_5)
    p = &core.regs.r5;
  else if (reg == REG_6)
    p = &core.regs.r6;
  else if (reg == REG_7)
    p = &core.regs.r7;
  else if (reg == REG_SP)
    p = &core.memory.sp;
  else if (reg == REG_BP)
    p = &core.memory.bp;
  else
    segfault(3); /* register unknow */

  return (p);
}
