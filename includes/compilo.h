/*
** Compilo NDH - Release v0.1
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

#ifndef	 COMPILO_H
#define	 COMPILO_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <wait.h>
#include <fcntl.h>
#include <elf.h>
#include <time.h>
#include "op.h"

/* colors */
#define BLUE        "\033[94m"
#define GREEN       "\033[92m"
#define YELLOW      "\033[93m"
#define RED         "\033[91m"
#define ENDC        "\033[0m"

#define MAGIC_NDH         0x48444e2e /* .NDH */
#define TRUE              0
#define FALSE             1

typedef struct s_infobin
{
  char *src_name;
  char *src_file;
} t_infobin;

typedef struct s_parsing
{
  char *instruction;
  char *operande;
  unsigned nb_line;       /* line number in source file */
  unsigned size;          /* size of instruction */
  struct s_parsing *next;
  struct s_parsing *back;
} t_parsing;

typedef struct s_compile
{
  uint32_t magic;
  uint16_t size;
  char *text;
  unsigned int offset;
} t_compile;

/* tab struct for instructions */
typedef struct s_instruction
{
  char *OP;
  void (*ptrOP)(void);
} t_instruction;

typedef struct s_size_instruction
{
  char *OP;
  size_t size;
} t_size_instruction;

typedef struct s_core
{
  struct s_infobin infobin;
  struct s_compile compile;
  t_parsing *parsing;
} t_core;

t_core                  core;

void                    *xmalloc(size_t);
int                     xopen(const char *, int, mode_t);
void                    *xmmap(void *, size_t, int, int, int, off_t);
ssize_t                 xread(int, void *, size_t);
int                     xclose(int);
void                    check_file_mode(char **);
void                    save_src_file(char *);
void                    free_all(void);
void                    compile(void);
void                    parsing(void);
void                    set_size(void);

#endif

