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

#ifndef	 VMNDH_H
#define	 VMNDH_H

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
#include <sys/socket.h>
#include <arpa/inet.h>
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

/* memory struct */
typedef struct s_memory
{
  uint16_t sp;
  uint16_t sp_aslr_base;
  uint16_t bp;
  uint16_t pc;
  uint16_t pc_pie_base;
  unsigned char *vmem;
} t_memory;

/* regs struct */
typedef struct s_regs
{
  uint16_t r0;
  uint16_t r1;
  uint16_t r2;
  uint16_t r3;
  uint16_t r4;
  uint16_t r5;
  uint16_t r6;
  uint16_t r7;
  uint8_t  zf;
  uint8_t  af;
  uint8_t  bf;
} t_regs;

typedef struct s_infobin
{
/*
** [MAGIC] [SIZE TEXT] [TEXT]
*/
  unsigned char   *binary;
  uint16_t        binary_size;
  uint32_t        magic;     /* 4 bytes */
  uint16_t        size_text;
  unsigned char   *text;
  char            *arg;
} t_infobin;

/* linked list for breakpoint mode */
typedef struct s_bp
{
  uint16_t addr;
  uint32_t bp_number;
  struct s_bp *next;
} t_bp;

/* flag struct */
typedef struct s_flagmode
{
  int aslr;
  int debug;
  int noverbose;
  int coredump;
  int nx;
  int pie;
  int arg;
  int bp;
  t_bp *linkbp;
} t_flagmode;

/* core struct */
typedef struct s_core
{
  struct s_infobin  infobin;
  struct s_memory   memory;
  struct s_regs     regs;
  struct s_flagmode flagmode;
} t_core;

/* tab struct for disass */
typedef struct s_disass
{
  char *name;
  char OP;
  uint16_t (*ptrOP)(uint16_t);
} t_disass;

/* tab struct for instructions */
typedef struct s_instruction
{
  char OP;
  void (*ptrOP)(void);
} t_instruction;

/* tab struct console debug */
typedef struct s_cmd_console
{
  char *cmd;
  int (*ptrCMD)(char *);
} t_cmd_console;

typedef struct s_regptr
{
  char *name;
  uint16_t *reg;
} t_regptr;

/* tab struct for syscalls */
typedef struct s_sys_call_table
{
  char OP;
  char *name;
  void (*ptrOP)(void);
} t_sys_call_table;

t_core                  core; /* core is only one global variable */

void           		syntax(char *);
void                    *xmalloc(size_t);
int                     xopen(const char *, int, mode_t);
void                    *xmmap(void *, size_t, int, int, int, off_t);
ssize_t                 xread(int, void *, size_t);
int                     xclose(int);
void                    check_file_mode(char **);
void                    check_aslr_mode(char **);
void                    check_nx_mode(char **);
void                    check_pie_mode(char **);
void                    check_debug_mode(char **);
void                    check_arg_mode(char **);
void                    check_core_mode(char **);
void                    error_ndh_format(int);
void                    free_all(void);
void                    init_vmem(void);
void                    parse_binary(void);
void                    save_binary(char *);
uint16_t                rand_aslr(void);
uint16_t                rand_pie(void);
void                    show_mem_debug(void);
int                     console_showsp(char *);
int                     console_showpc(char *);
int                     console_inforeg(char *);
int                     console_quit(char *);
int                     console_help(char *);
int                     console_run(char *);
int                     console_bp(char *);
int                     console_info_bp(char *);
int                     console_xs(char *);
int                     console_xx(char *);
int                     console_set(char *);
int                     console_syscall(char *);
int                     console_dis(char *);
uint16_t                *get_reg(char);
void			execute(void);
void                    disass(uint16_t, uint16_t);
void                    segfault(int);
void			op_push(void);
void                    op_pop(void);
void                    op_nop(void);
void                    op_inc(void);
void                    op_dec(void);
void                    op_xor(void);
void                    op_or(void);
void                    op_and(void);
void                    op_not(void);
void                    op_add(void);
void                    op_sub(void);
void                    op_mul(void);
void                    op_div(void);
void                    op_call(void);
void                    op_ret(void);
void                    op_jmpl(void);
void                    op_jmps(void);
void                    op_test(void);
void                    op_cmp(void);
void                    op_jz(void);
void                    op_ja(void);
void                    op_jb(void);
void                    op_jnz(void);
void                    op_mov(void);
void                    op_end(void);
void                    op_xchg(void);
void                    op_syscall(void);
void                    syscall_exit(void);
void                    syscall_write(void);
void                    syscall_close(void);
void                    syscall_open(void);
void                    syscall_read(void);
void                    syscall_setuid(void);
void                    syscall_setgid(void);
void                    syscall_dup2(void);
void                    syscall_send(void);
void                    syscall_recv(void);
void                    syscall_socket(void);
void                    syscall_bind(void);
void                    syscall_listen(void);
void                    syscall_accept(void);
void                    syscall_chdir(void);
void                    syscall_chmod(void);
void                    syscall_lseek(void);
void                    syscall_getpid(void);
void                    syscall_getuid(void);
void                    syscall_pause(void);

#endif

