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

void error_ndh_format(int error)
{
  switch (error)
    {
      case 0:
              fprintf(stderr, "%sError%s: Magic NDH\n", RED, ENDC);
              break;
      case 1:
              fprintf(stderr, "%sError%s: size text\n", RED, ENDC);
              break;
      case 2:
              fprintf(stderr, "%sError%s: Size binary too shurt\n", RED, ENDC);
              break;
      case 3:
              fprintf(stderr, "%sError%s: Size binary too long\n", RED, ENDC);
              break;
    }

  exit(EXIT_FAILURE);
}
