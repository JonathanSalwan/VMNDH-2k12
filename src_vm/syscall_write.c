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

void syscall_write(void)
{
  uint16_t  arg1;
  char      *arg2;
  uint16_t  arg3;
  char      txt[] = "[\033[93mSYSCALL output\033[0m]: ";

  arg1 = core.regs.r1;
  arg2 = (char *)(core.memory.vmem + core.regs.r2);
  arg3 = core.regs.r3;

  if (core.flagmode.noverbose == 0)
    write(1, txt, strlen(txt));
  core.regs.r0 = write(arg1, arg2, arg3);
  if (core.flagmode.noverbose == 0)
    write(1, "\n", 1);
}

