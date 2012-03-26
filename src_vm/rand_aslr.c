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

uint16_t rand_aslr(void)
{
  uint32_t aslr;

  #ifdef __x86_64__
    __asm__("mov %fs:0x28, %eax");
  #else
    __asm__("mov %gs:0x14, %eax");
  #endif
  __asm__("shr $0x08, %eax");
  __asm__("mov %%eax, %0" : "=m"(aslr));
  aslr = aslr % 0x3000 + 0x4ffe;

  return((uint16_t)aslr);
}
