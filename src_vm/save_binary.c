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

void save_binary(char *file_name)
{
  struct stat filestat;
  int fd;
  int size;

  stat(file_name, &filestat);
  size = filestat.st_size;
  fd = xopen(file_name, O_RDONLY, 0644);
  core.infobin.binary = xmalloc(size * sizeof(char));
  core.infobin.binary_size = size;
  xread(fd, core.infobin.binary, size);
  xclose(fd);
}
