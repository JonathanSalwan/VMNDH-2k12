##
## vmndh - Release v0.1
## Jonathan Salwan - http://twitter.com/JonathanSalwan
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##

DEBUG   = no
RM      = rm -f
INCLUDE = ./includes
SRC_DIR = ./src_vm
NAME    = vmndh

ifeq ($(DEBUG),yes)
    CFLAGS   	= -c -ggdb3 -Wextra -Wall -D _BSD_SOURCE -I$(INCLUDE)
    LDFLAGS     =
    CC 		= clang
else
    CFLAGS    	= -c -W -Wall -ansi -Werror -pedantic -D _BSD_SOURCE -I$(INCLUDE)
    LDFLAGS     =
    CC 		= gcc
endif

SRC     = 	$(SRC_DIR)/main.c \
          	$(SRC_DIR)/syntax.c \
		$(SRC_DIR)/check_file_mode.c \
		$(SRC_DIR)/check_aslr_mode.c \
		$(SRC_DIR)/check_nx_mode.c \
		$(SRC_DIR)/check_pie_mode.c \
		$(SRC_DIR)/check_arg_mode.c \
		$(SRC_DIR)/check_debug_mode.c \
		$(SRC_DIR)/check_core_mode.c \
		$(SRC_DIR)/xfunc.c \
                $(SRC_DIR)/error_ndh_format.c \
                $(SRC_DIR)/free_all.c \
                $(SRC_DIR)/init_vmem.c \
                $(SRC_DIR)/parse_binary.c \
		$(SRC_DIR)/rand_aslr.c \
		$(SRC_DIR)/rand_pie.c \
                $(SRC_DIR)/save_binary.c \
		$(SRC_DIR)/show_mem_debug.c \
		$(SRC_DIR)/get_reg.c \
		$(SRC_DIR)/execute.c \
		$(SRC_DIR)/disass.c \
		$(SRC_DIR)/segfault.c \
		$(SRC_DIR)/console_functions.c \
		$(SRC_DIR)/op_push.c \
		$(SRC_DIR)/op_pop.c \
		$(SRC_DIR)/op_nop.c \
		$(SRC_DIR)/op_inc.c \
		$(SRC_DIR)/op_dec.c \
		$(SRC_DIR)/op_xor.c \
		$(SRC_DIR)/op_or.c \
                $(SRC_DIR)/op_and.c \
                $(SRC_DIR)/op_not.c \
                $(SRC_DIR)/op_add.c \
		$(SRC_DIR)/op_sub.c \
                $(SRC_DIR)/op_mul.c \
                $(SRC_DIR)/op_div.c \
		$(SRC_DIR)/op_call.c \
		$(SRC_DIR)/op_ret.c \
		$(SRC_DIR)/op_jmpl.c \
		$(SRC_DIR)/op_jmps.c \
		$(SRC_DIR)/op_test.c \
		$(SRC_DIR)/op_cmp.c \
		$(SRC_DIR)/op_jz.c \
		$(SRC_DIR)/op_ja.c \
		$(SRC_DIR)/op_jb.c \
		$(SRC_DIR)/op_jnz.c \
		$(SRC_DIR)/op_mov.c \
		$(SRC_DIR)/op_end.c \
		$(SRC_DIR)/op_xchg.c \
		$(SRC_DIR)/syscall.c \
		$(SRC_DIR)/syscall_exit.c \
		$(SRC_DIR)/syscall_write.c \
		$(SRC_DIR)/syscall_close.c \
		$(SRC_DIR)/syscall_open.c \
		$(SRC_DIR)/syscall_read.c \
		$(SRC_DIR)/syscall_setuid.c \
		$(SRC_DIR)/syscall_setgid.c \
		$(SRC_DIR)/syscall_dup2.c \
		$(SRC_DIR)/syscall_send.c \
		$(SRC_DIR)/syscall_recv.c \
		$(SRC_DIR)/syscall_socket.c \
		$(SRC_DIR)/syscall_bind.c \
		$(SRC_DIR)/syscall_listen.c \
		$(SRC_DIR)/syscall_accept.c \
		$(SRC_DIR)/syscall_chdir.c \
                $(SRC_DIR)/syscall_chmod.c \
                $(SRC_DIR)/syscall_lseek.c \
                $(SRC_DIR)/syscall_getpid.c \
                $(SRC_DIR)/syscall_getuid.c \
                $(SRC_DIR)/syscall_pause.c


OBJ      = $(SRC:.c=.o)

all:     $(NAME)

$(NAME): $(OBJ)
	 $(CC) $(LDFLAGS) $(OBJ) -o $@

install:
	 install -D -m 755 ./$(NAME) /usr/bin/$(NAME)

clean:
	 $(RM) $(OBJ)

fclean:  clean
	 $(RM) $(NAME)

re:	 fclean all

.c.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY:  re fclean clean install all
