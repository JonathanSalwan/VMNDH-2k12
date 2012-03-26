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

static void set_arg_in_memory(void)
{
  size_t size_arg;
  uint16_t tmp;

  size_arg = strlen(core.infobin.arg);
  if (size_arg > 0xff)
    {
      fprintf(stderr, "%sError%s: arg too long\n", RED, ENDC);
      exit(EXIT_FAILURE);
    }
  core.memory.sp -= size_arg;
  core.memory.bp = core.memory.sp;
  strcpy((char *)(core.memory.vmem + core.memory.sp), core.infobin.arg);
  tmp = core.memory.sp;
  core.memory.sp -= 32; /* allocate */
  core.memory.bp -= 32;
  /* push arg @ on stack */
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = tmp;
  core.memory.sp -= 2;
  core.memory.bp -= 2;

  /* push argco on stack */
  *(uint16_t *)(core.memory.vmem + core.memory.sp) = 0x01;
}

void init_vmem(void)
{
  core.memory.vmem = xmalloc(0xffff);
  memset(core.memory.vmem, 0x00, 0xffff);

  /* if ASLR */
  if (core.flagmode.aslr == 0)
    {
      core.memory.sp  = 0x7ffe;
      core.memory.bp  = 0x7ffe;
      core.memory.sp_aslr_base  = 0x7fff;
    }
  else
    {
      core.memory.sp = rand_aslr();
      core.memory.bp = core.memory.sp;
      core.memory.sp_aslr_base = core.memory.sp + 1;
    }

  /* if PIE */
  if (core.flagmode.pie == 0)
    {
      core.memory.pc  = 0x8000;
      core.memory.pc_pie_base = 0x8000;
    }
  else
    {
      core.memory.pc = rand_pie();
      core.memory.pc_pie_base = core.memory.pc;
    }

  /* if arg */
  if (core.flagmode.arg == 1)
    set_arg_in_memory();
  else
    {
      core.memory.sp -= 4;
      core.memory.bp = core.memory.sp;
      *(uint16_t *)(core.memory.vmem + core.memory.sp) = 0x00; /* push argc (argc = 0)*/
      *(uint16_t *)(core.memory.vmem + core.memory.sp) = 0x00; /* push argc (argv = NULL)*/
    }


  core.regs.r0 = 0;
  core.regs.r1 = 0;
  core.regs.r2 = 0;
  core.regs.r3 = 0;
  core.regs.r4 = 0;
  core.regs.r5 = 0;
  core.regs.r6 = 0;
  core.regs.r7 = 0;
  core.regs.zf = 0;
  core.regs.af = 0;
  core.regs.bf = 0;

  core.flagmode.linkbp = NULL;
}
