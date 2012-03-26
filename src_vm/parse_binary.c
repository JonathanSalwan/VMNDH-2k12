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

void parse_binary(void)
{
  int i = 0;

  if (core.infobin.binary_size < 7)       /* size binary too shurt */
    error_ndh_format(2);
  if (core.infobin.binary_size > 0x7ffe)  /* size binary too long */
    error_ndh_format(3);

  core.infobin.magic        = *(uint32_t *)core.infobin.binary;
  if (core.infobin.magic != MAGIC_NDH)
    error_ndh_format(0);

  core.infobin.size_text    = *(uint16_t *)(core.infobin.binary + 4);
  if ((core.infobin.size_text + 6) != core.infobin.binary_size)
    error_ndh_format(1);

  core.infobin.text = xmalloc(core.infobin.size_text);
  while (i != core.infobin.size_text)
    {
      core.infobin.text[i] = (core.infobin.binary + 6)[i];
      i++;
    }

  /* set section text on vmem */
  i = 0;
  while (i != core.infobin.size_text)
    {
      (core.memory.vmem + core.memory.pc)[i] = core.infobin.text[i];
      i++;
    }
}
