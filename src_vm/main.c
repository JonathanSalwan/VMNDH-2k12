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

int main(__attribute__ ((unused))int argc, char **argv)
{
  core.flagmode.noverbose = 1;
  check_aslr_mode(argv);
  check_nx_mode(argv);
  check_pie_mode(argv);
  check_debug_mode(argv);
  check_core_mode(argv);
  check_file_mode(argv);

  syntax(argv[0]);
  return(0);
}
