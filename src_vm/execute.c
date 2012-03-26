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

static t_instruction tabOP[] =
{
    {OP_PUSH,     &op_push},
    {OP_NOP,      &op_nop},
    {OP_POP,      &op_pop},
    {OP_INC,      &op_inc},
    {OP_DEC,      &op_dec},
    {OP_XOR,      &op_xor},
    {OP_OR,       &op_or},
    {OP_AND,      &op_and},
    {OP_NOT,      &op_not},
    {OP_ADD,      &op_add},
    {OP_SUB,      &op_sub},
    {OP_MUL,      &op_mul},
    {OP_DIV,      &op_div},
    {OP_CALL,     &op_call},
    {OP_RET,      &op_ret},
    {OP_JMPS,     &op_jmps},
    {OP_JMPL,     &op_jmpl},
    {OP_TEST,     &op_test},
    {OP_CMP,      &op_cmp},
    {OP_JZ,       &op_jz},
    {OP_JNZ,      &op_jnz},
    {OP_MOV,      &op_mov},
    {OP_END,      &op_end},
    {OP_XCHG,     &op_xchg},
    {OP_JA,       &op_ja},
    {OP_JB,       &op_jb},
    {OP_SYSCALL,  &op_syscall},
    {0, NULL}
};

static t_cmd_console tabCMD[] =
{
    {"run",      &console_run},
    {"r",        &console_run},
    {"bp",       &console_bp},
    {"info bp",  &console_info_bp},
    {"info reg", &console_inforeg},
    {"show pc",  &console_showpc},
    {"show sp",  &console_showsp},
    {"dis",      &console_dis},
    {"x/s",      &console_xs},
    {"x/x",      &console_xx},
    {"set",      &console_set},
    {"syscall",  &console_syscall},
    {"help",     &console_help},
    {"?",        &console_help},
    {"quit",     &console_quit},
    {NULL, NULL}
};

static void check_pc(uint16_t end_text)
{
  if (core.memory.pc > end_text)
    segfault(4); /* out of range .text */

  /* If NX enable */
  if (core.flagmode.nx == 1)
    if (core.memory.pc < core.memory.pc_pie_base)
      segfault(5); /* stack not exec */

  /* check if sp in .text section */
  if (core.memory.sp >= core.memory.pc_pie_base)
    segfault(6);
}

static void check_bp(void)
{
  t_bp *tmp_bp;

  tmp_bp = core.flagmode.linkbp;
  if (core.flagmode.bp == 1 && core.flagmode.debug != 1)
    {
      while (tmp_bp != NULL)
        {
          if (tmp_bp->addr == core.memory.pc)
            {
              fprintf(stdout, "[%sBreakPoint %d - 0x%.4x%s]\n", RED, tmp_bp->bp_number, tmp_bp->addr, ENDC);
              core.flagmode.debug = 1;
              return ;
            }
          tmp_bp = tmp_bp->next;
        }
    }
}

static void debug_mode(void)
{
  char cmd[32];
  int i, found;

  if (core.flagmode.debug == 0)
    return ;

  bcl:
  i = found = 0;
  memset(cmd, 0x00, 32);
  write(1, "\033[94m[Console]#>\033[0m ", 22);
  read(0, cmd, 28);
  while (tabCMD[i].cmd != NULL)
    {
      if (!strncmp(tabCMD[i].cmd, cmd, strlen(tabCMD[i].cmd)))
        found = tabCMD[i].ptrCMD(cmd);
      i++;
    }
  if (found == 0 && cmd[0] != '\n')
    {
      fprintf(stderr, "command not found\n");
      goto bcl;
    }
  if (found == 1)
    goto bcl;
}

void execute(void)
{
  int i = 0;
  int found;
  unsigned char OP;
  uint16_t end_text;

  end_text = core.memory.pc + core.infobin.size_text;
  while ((core.memory.pc != end_text))
    {
      found = 0;
      debug_mode();
      check_pc(end_text);
      check_bp();
      OP = *(core.memory.vmem + core.memory.pc);
      while (tabOP[i].ptrOP != NULL)
        {
          if (tabOP[i].OP == OP)
            {
              found = 1;
              tabOP[i].ptrOP();
            }
          i++;
        }
      i = 0;
      if (found == 0)
        segfault(0); /* opcode not found */
    }
}
