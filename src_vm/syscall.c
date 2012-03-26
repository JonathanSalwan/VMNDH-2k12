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

static t_sys_call_table syscalltable[] =
{
    {SYS_EXIT,   "exit",   &syscall_exit},
    {SYS_WRITE,  "write",  &syscall_write},
    {SYS_CLOSE,  "close",  &syscall_close},
    {SYS_OPEN,   "open",   &syscall_open},
    {SYS_READ,   "read",   &syscall_read},
    {SYS_SETUID, "setuid", &syscall_setuid},
    {SYS_SETGID, "setgid", &syscall_setgid},
    {SYS_DUP2,   "dup2",   &syscall_dup2},
    {SYS_SEND,   "send",   &syscall_send},
    {SYS_RECV,   "recv",   &syscall_recv},
    {SYS_SOCKET, "socket", &syscall_socket},
    {SYS_BIND,   "bind",   &syscall_bind},
    {SYS_LISTEN, "listen", &syscall_listen},
    {SYS_ACCEPT, "accept", &syscall_accept},
    {SYS_CHDIR,  "chdir",  &syscall_chdir},
    {SYS_CHMOD,  "chmod",  &syscall_chmod},
    {SYS_LSEEK,  "lseek",  &syscall_lseek},
    {SYS_GETPID, "getpid", &syscall_getpid},
    {SYS_GETUID, "getuid", &syscall_getuid},
    {SYS_PAUSE,  "pause",  &syscall_pause},

    {0, NULL, NULL}
};


static char *namesyscall(char syscall_number)
{
  int i = 0;

  while(syscalltable[i].name != NULL)
    {
      if (syscalltable[i].OP == syscall_number)
        return (syscalltable[i].name);
      i++;
    }
  return ("unknown");
}

void op_syscall(void)
{
  int i;
  int found;
  char syscall_number;

  i = 0;
  found = 0;
  syscall_number = core.regs.r0;
  if (core.flagmode.noverbose == 0)
    fprintf(stdout, "%s0x%4.x%s > syscall (r0 = 0x%.4x - %s)\n", RED, core.memory.pc, ENDC, syscall_number, namesyscall(syscall_number));
  while (syscalltable[i].ptrOP != NULL)
    {
      if (syscalltable[i].OP == syscall_number)
        {
          found = 1;
          syscalltable[i].ptrOP();
        }
      i++;
    }
  if (found == 0)
    segfault(7); /* syscall_number not found */

  core.memory.pc += 1;
}
