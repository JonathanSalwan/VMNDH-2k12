/*
** vmndh - v0.1
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

void syntax(char *str)
{
  fprintf(stderr, "%sSyntax%s:  %s [OPTION] <binary> [FLAG]\n\n", RED, ENDC, str);
  fprintf(stderr, "%sOPTION%s:\n", RED, ENDC);
  fprintf(stderr, "             -file         Load binary\n");
  fprintf(stderr, "             -arg          Binary argument (optional)\n");
  fprintf(stderr, "%sFLAG%s:\n", RED, ENDC);
  fprintf(stderr, "             -aslr         Enable ASLR\n");
  fprintf(stderr, "             -nx           Enable NX bit\n");
  fprintf(stderr, "             -pie          Enable PIE\n");
  fprintf(stderr, "             -debug        Debug console\n");
  fprintf(stderr, "             -core         Generates a core dump when segfault\n");
  exit(EXIT_FAILURE);
}
