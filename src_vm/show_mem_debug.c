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

void show_mem_debug(void)
{
  uint32_t cpt = 0;
  uint16_t i = 0;
  uint16_t off = 0;
  FILE *f = fopen("/tmp/ndh.core", "w");

  fprintf(stdout, "]=--- Core Dumped -----------------------------\n");
  fprintf(stdout, "\n[%sShow Memory Debug%s]\n\n", BLUE, ENDC);
  fprintf(stdout, "[%sr0%s]: %.4x\t[%sr4%s]: %.4x\n",   RED, ENDC, core.regs.r0, RED, ENDC, core.regs.r4);
  fprintf(stdout, "[%sr1%s]: %.4x\t[%sr5%s]: %.4x\n",   RED, ENDC, core.regs.r1, RED, ENDC, core.regs.r5);
  fprintf(stdout, "[%sr2%s]: %.4x\t[%sr6%s]: %.4x\n",   RED, ENDC, core.regs.r2, RED, ENDC, core.regs.r6);
  fprintf(stdout, "[%sr3%s]: %.4x\t[%sr7%s]: %.4x\n\n",   RED, ENDC, core.regs.r3, RED, ENDC, core.regs.r7);
  fprintf(stdout, "[%sbp%s]: %.4x\t[%szf%s]: %.4x\n",   RED, ENDC, core.memory.bp, RED, ENDC, core.regs.zf);
  fprintf(stdout, "[%ssp%s]: %.4x\t[%saf%s]: %.4x\n",   RED, ENDC, core.memory.sp, RED, ENDC, core.regs.af);
  fprintf(stdout, "[%spc%s]: %.4x\t[%sbf%s]: %.4x\n\n",   RED, ENDC, core.memory.pc, RED, ENDC, core.regs.bf);

  fprintf(stdout, "[%sShow SP%s]\n\n", BLUE, ENDC);
  i = core.memory.sp;
  while (i <= core.memory.sp_aslr_base)
    {
      fprintf(stdout, "%s%.4x%s: %.2x ", RED, core.memory.sp + off, ENDC, *(core.memory.vmem + core.memory.sp + off));
      if (i + 1 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 1 + off));
      if (i + 2 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 2 + off));
      if (i + 3 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 3 + off));
      if (i + 4 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 4 + off));
      if (i + 5 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 5 + off));
      if (i + 6 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 6 + off));
      if (i + 7 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 7 + off));
      if (i + 8 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x ", *(core.memory.vmem + core.memory.sp + 8 + off));
      if (i + 9 <= core.memory.sp_aslr_base)
        fprintf(stdout, "%.2x\n", *(core.memory.vmem + core.memory.sp + 9 + off));
      i += 10;
      off += 10;
    }

  fprintf(stdout, "\n\n[%sShow PC%s]\n\n", BLUE, ENDC);
  i = 0;
  while (i != 80)
    {
      fprintf(stdout, "%s%.4x%s: %.2x ", RED, (core.memory.pc + i), ENDC, (core.memory.pc + core.memory.vmem)[i]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 1]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 2]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 3]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 4]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 5]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 6]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 7]);
      fprintf(stdout, "%.2x ", (core.memory.pc + core.memory.vmem)[i + 8]);
      fprintf(stdout, "%.2x\n", (core.memory.pc + core.memory.vmem)[i + 9]);
      i += 10;
    }

  i = 0;
  while (cpt <= 0xffff)
    {
      fprintf(f, "0x%.4x:  ", cpt);
      while (i != 16)
        {
          fprintf(f, "%.2x ", *(core.memory.vmem + cpt + i));
          i++;
        }
      fprintf(f, "\n");
      i = 0;
      cpt += 16;
    }
  fclose(f);
  fprintf(stdout, "\n[+] Full memory dumped in /tmp/ndh.core\n");

  fprintf(stdout, "\n]=--- Core Dumped -----------------------------\n");
}
