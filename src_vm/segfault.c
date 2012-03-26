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

void segfault(int error)
{
  switch (error)
    {
      case 0:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (opcode unknown)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 1:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (operande unknown)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 2:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (flag unknown)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 3:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (register unknown)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 4:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (out of range)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 5:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (NX bit)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 6:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (SP out of range)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 7:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (syscall unknown)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
      case 8:
              if (core.flagmode.noverbose == 0)
                fprintf(stderr, "\n");
              fprintf(stderr, "%s[!] Segfault 0x%.4x (write out of range)%s\n", RED, core.memory.pc, ENDC);
              if (core.flagmode.noverbose == 0 || core.flagmode.coredump == 1)
                show_mem_debug();
              free_all();
              break;
    }

  exit(EXIT_FAILURE);
}

