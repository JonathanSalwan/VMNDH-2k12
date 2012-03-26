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

t_regptr tabREG[] = {
  {"r0", &core.regs.r0},
  {"r1", &core.regs.r1},
  {"r2", &core.regs.r2},
  {"r3", &core.regs.r3},
  {"r4", &core.regs.r4},
  {"r5", &core.regs.r5},
  {"r6", &core.regs.r6},
  {"r7", &core.regs.r7},
  {"sp", &core.memory.sp},
  {"bp", &core.memory.bp},
  {"pc", &core.memory.pc},
  {NULL, NULL}
};

int console_inforeg(__attribute__ ((unused))char *cmd)
{
  fprintf(stdout, "[%sr0%s]: %.4x\t[%sr4%s]: %.4x\n",   RED, ENDC, core.regs.r0, RED, ENDC, core.regs.r4);
  fprintf(stdout, "[%sr1%s]: %.4x\t[%sr5%s]: %.4x\n",   RED, ENDC, core.regs.r1, RED, ENDC, core.regs.r5);
  fprintf(stdout, "[%sr2%s]: %.4x\t[%sr6%s]: %.4x\n",   RED, ENDC, core.regs.r2, RED, ENDC, core.regs.r6);
  fprintf(stdout, "[%sr3%s]: %.4x\t[%sr7%s]: %.4x\n\n",   RED, ENDC, core.regs.r3, RED, ENDC, core.regs.r7);
  fprintf(stdout, "[%sbp%s]: %.4x\t[%szf%s]: %.4x\n",   RED, ENDC, core.memory.bp, RED, ENDC, core.regs.zf);
  fprintf(stdout, "[%ssp%s]: %.4x\t[%saf%s]: %.4x\n",   RED, ENDC, core.memory.sp, RED, ENDC, core.regs.af);
  fprintf(stdout, "[%spc%s]: %.4x\t[%sbf%s]: %.4x\n",   RED, ENDC, core.memory.pc, RED, ENDC, core.regs.bf);
  return (1);
}

int console_showsp(__attribute__ ((unused))char *cmd)
{
  uint16_t i = 0;
  uint16_t off = 0;

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
  fprintf(stdout, "\n");
  return (1);
}

int console_showpc(__attribute__ ((unused))char *cmd)
{
  int i = 0;

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
  return (1);
}

int console_help(__attribute__ ((unused))char *cmd)
{
  fprintf(stdout, "%s<enter>%s              Execute next instruction\n", RED, ENDC);
  fprintf(stdout, "%srun%s                  Run program\n", RED, ENDC);
  fprintf(stdout, "%sbp <addr>%s            Set breakpoint\n", RED, ENDC);
  fprintf(stdout, "%sinfo bp%s              Display info breakpoint\n", RED, ENDC);
  fprintf(stdout, "%sinfo reg%s             Display registers\n", RED, ENDC);
  fprintf(stdout, "%sshow sp%s              Display SP memory\n", RED, ENDC);
  fprintf(stdout, "%sshow pc%s              Display PC memory\n", RED, ENDC);
  fprintf(stdout, "%sdis <addr>:<size>%s    Disassembly X bytes from ADDR\n", RED, ENDC);
  fprintf(stdout, "%sx/x <addr>:<size>%s    Print X bytes from ADDR\n", RED, ENDC);
  fprintf(stdout, "%sx/s <addr>%s           Print string addr\n", RED, ENDC);
  fprintf(stdout, "%sset reg=value%s        Set value in register\n", RED, ENDC);
  fprintf(stdout, "%ssyscall%s              Execute 'syscall' instruction\n", RED, ENDC);
  fprintf(stdout, "%shelp%s                 Display this help\n", RED, ENDC);
  fprintf(stdout, "%squit%s                 Quit console debugging\n", RED, ENDC);
  return (1);
}

int console_run(__attribute__ ((unused))char *cmd)
{
  core.flagmode.debug = 0;
  return (2);
}

static t_bp *add_elem(t_bp *old_elem, char *addrstr, uint32_t cpt)
{
  t_bp *elem;

  elem = xmalloc(sizeof(t_bp));
  elem->addr = strtol(addrstr, NULL, 16);
  elem->bp_number = cpt;
  elem->next = old_elem;
  return (elem);
}

static int howmanybp(t_bp *elem)
{
  int cpt = 0;

  while (elem != NULL)
    {
      cpt++;
      elem = elem->next;
    }
  return (cpt);
}

int console_bp(char *cmd)
{
  int cpt;


  if (strtol(cmd+3, NULL, 16) == 0)
    {
      fprintf(stderr, "Bad address\n");
      return (1);
    }
  cpt = howmanybp(core.flagmode.linkbp);
  core.flagmode.bp = 1;
  core.flagmode.linkbp = add_elem(core.flagmode.linkbp, cmd+3, cpt+1);
  fprintf(stdout, "Breakpoint set in 0x%.4x\n", (uint16_t)strtol(cmd+3, NULL, 16));

  return (1);
}

int console_info_bp(__attribute__ ((unused))char *cmd)
{
  t_bp *tmpbp;

  tmpbp = core.flagmode.linkbp;
  if (tmpbp == NULL)
    fprintf(stdout, "No breakpoint\n");
  else
    while (tmpbp != NULL)
      {
        fprintf(stdout, "Breakpoint %d in 0x%.4x\n", tmpbp->bp_number, tmpbp->addr);
        tmpbp = tmpbp->next;
      }

  return (1);
}

int console_xs(char *cmd)
{
  uint16_t addr;
  unsigned char *real;

  addr = strtol(cmd+3, NULL, 16);
  real = (core.memory.vmem + addr);
  if (addr == 0)
    {
      fprintf(stderr, "Bad address\n");
      return (1);
    }
  fprintf(stdout, "%s0x%.4x%s: %s\n", RED, addr, ENDC, real);

  return (1);
}

static uint16_t console_xx_getaddr(char *cmd)
{
  int i = 0;

  while (cmd[i] != '\0' && cmd[i] != ':')
    i++;

  if (cmd[i] == ':')
    cmd[i] = '\0';
  else
    return (0);

  return ((uint16_t)strtol(cmd, NULL, 16));
}

static char *console_xx_getsize(char *cmd)
{
  int i = 0;

  while (cmd[i] != '\0' && cmd[i] != ':')
    i++;

  if (cmd[i] == ':')
    return (cmd + i + 1);
  else
    return (cmd + i);
}


int console_xx(char *cmd)
{
  int i = 0;
  uint32_t cpt;
  uint16_t addr;
  uint16_t size;
  char *cmd_size;

  cmd = cmd + 4;
  cmd_size = console_xx_getsize(cmd);
  addr = console_xx_getaddr(cmd);
  size = (uint16_t)strtol(cmd_size, NULL, 16);
  cpt = 1;
  while (cpt <= size)
    {
      i = 0;
      fprintf(stdout, "%s0x%.4x%s: ", RED, addr, ENDC);
      while (i != 16 && cpt <= size)
        {
          fprintf(stdout, "%.2x ", *(core.memory.vmem + addr));
          i++;
          addr++;
          cpt++;
        }
      fprintf(stdout, "\n");
    }

  return (1);
}

static char *console_set_getreg(char *cmd)
{
  int i =0;

  while (cmd[i] != '\0' && cmd[i] != '=')
    i++;

  if (cmd[i] == '=')
    cmd[i] = '\0';
  else
    return (NULL);

  return (cmd);
}

static uint16_t *getregptr(char *reg)
{
  int i = 0;

  while (tabREG[i].name != NULL)
    {
      if (!strcmp(tabREG[i].name, reg))
        return (tabREG[i].reg);
      i++;
    }
  return (0);
}

int console_set(char *cmd)
{
  char *reg;
  char *value;
  uint16_t *ptr;

  cmd = cmd + 4; /* ptr on first args */
  reg = console_set_getreg(cmd);
  if (reg == NULL)
    {
      fprintf(stderr, "%sError syntax%s: exemple: set r1=0883\n", RED, ENDC);
      return (1);
    }
  cmd = cmd + strlen(cmd) + 1;
  value = cmd;
  if (*value == '\0' || *value == '\n')
    {
      fprintf(stderr, "%sError syntax%s: exemple: set r1=0883\n", RED, ENDC);
      return (1);
    }
  ptr = getregptr(reg);
  if (ptr == 0)
    {
      fprintf(stderr, "%sError%s: register '%s' not found\n", RED, ENDC, reg);
      return (1);
    }
  *ptr = strtol(value, NULL, 16);
  printf("%s = 0x%.4x\n", reg, *ptr);

  return (1);
}

static uint16_t console_dis_getaddr(char *cmd)
{
  int i = 0;

  while (cmd[i] != '\0' && cmd[i] != ':')
    i++;

  if (cmd[i] == ':')
    cmd[i] = '\0';
  else
    return (0);

  return ((uint16_t)strtol(cmd, NULL, 16));
}

static char *console_dis_getsize(char *cmd)
{
  int i = 0;

  while (cmd[i] != '\0' && cmd[i] != ':')
    i++;

  if (cmd[i] == ':')
    return (cmd + i + 1);
  else
    return (cmd + i);
}

int console_dis(char *cmd)
{
  uint16_t addr;
  uint16_t size;
  char *cmd_size;

  cmd = cmd + 4;
  cmd_size = console_dis_getsize(cmd);
  addr = console_dis_getaddr(cmd);
  if (addr < core.memory.pc_pie_base || addr > core.memory.pc_pie_base + core.infobin.size_text)
    {
      fprintf(stderr, "%sError%s: Set addr between 0x%.4x and 0x%.4x\n", RED, ENDC,
                                                                         core.memory.pc_pie_base,
                                                                         core.memory.pc_pie_base + core.infobin.size_text);
      return (1);
    }
  size = (uint16_t)strtol(cmd_size, NULL, 16);
  if (size > core.infobin.size_text)
    {
      fprintf(stderr, "%sError%s: Max sax 0x%.4x\n", RED, ENDC, core.infobin.size_text);
      return (1);
    }

  disass(addr, size);

  return (1);
}


int console_syscall(__attribute__ ((unused))char *cmd)
{
  op_syscall();
  return (1);
}

int console_quit(__attribute__ ((unused))char *cmd)
{
  free_all();
  exit(EXIT_SUCCESS);
}
