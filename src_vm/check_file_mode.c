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

void check_file_mode(char **argv)
{
  int i = 0;

  while (argv[i] != NULL)
    {
      if (!strcmp(argv[i], "-file"))
        {
          if (argv[i + 1] != NULL && argv[i + 1][0] != '\0')
            {
              check_arg_mode(argv);
              init_vmem();
              save_binary(argv[i + 1]);
              parse_binary();
              execute();
              free_all();
              exit(EXIT_SUCCESS);   /* end */
            }
          else
            {
              fprintf(stderr, "%sSyntax%s: -file <binary>\n", RED, ENDC);
              exit(EXIT_FAILURE);
            }
        }
      i++;
    }
}
