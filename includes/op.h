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

#ifndef   __OP_H__
#define   __OP_H__

/****************************************
 *
 *
 * ------------------------------- Mapping memory:
 *
 *  STACK   [0000 - 7FFF]  (default ASLR is disable. Possibility to set ASLR with -aslr)
 *  Program [8000 - FFFE]  (default NX & PIE is disable. Possibility to set NX & PIE with -nx -pie)
 *
 *  ASLR genered with:
 *                    __asm__("mov %gs:0x14, %eax");
 *                    __asm__("shr $0x08, %eax");
 *                    __asm__("mov %%eax, %0" : "=m"(aslr));
 *                    aslr = aslr % 0x3000 + 0x4ffe;
 *
 *  PIE genered with:
 *                    __asm__("mov %gs:0x14, %eax");
 *                    __asm__("shr $0x08, %eax");
 *                    __asm__("mov %%eax, %0" : "=m"(pie));
 *                    pie = pie % 0x3000 + 0x8000;
 *
 *
 *
 *   ^   0000>+-----------------+
 *   |        |                 |             The size max of binary is 0x7ffe.
 *   |        |     STACK    ^^ |
 *   |        |              || |             Before the program is executed atgv and argc is pushed on the stack.
 *   |        |                 |             If a arguments is set with (-arg), argc = 1 and argv points to the string.
 *   |        +-----------------+< SP & BP
 *   6        |     ARG         |             If you don't set a arguments argc and argv is pushed with value 0x00.
 *   4   8000>+-----------------+< PC
 *   K        |                 |
 *   |        |              || |             Exemple1: ./vmndh -file ./binary
 *   |        |     CODE     vv |
 *   |        |                 |                       [SP] 0x00 0x00 0x00 0x00 0x00 0x00
 *   |        |                 |                            <--argc-> <-argv-->
 *   |        |                 |
 *   v   ffff>+-----------------+             Exemple2: ./vmndh -file ./binary -arg "abcd"
 *
 *  -------------------------------                     [SP] 0x01 0x00 0xac 0x7f 0x00 0x00
 *                                                           <--argc-> <-argv-->
 *
 *
 *  ------------------------------- File format (fichier .ndh)
 *
 *  [MAGIC][size .text][.text content]
 *
 *  MAGIC: ".NDH"
 *  SIZE:  size of section TEXT
 *  CODE:  our instructions
 *  -------------------------------
 *
 *
 *  ------------------------------------- Instruction encoding:
 *
 *  [OPCODE] [OP_FLAGS | !] [OPERAND #1] [OPERAND #2]
 *
 *
 *  [ADD]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = add
 *          - dir8  = addb
 *          - dir16 = addl
 *
 *  [AND]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = and
 *          - dir8  = andb
 *          - dir16 = andl
 *
 *  [CALL]  <opcode> <FLAG> <REG | DIR16> (size = 3 or 4 bytes)
 *
 *  [CMP]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = cmp
 *          - dir8  = cmpb
 *          - dir16 = cmpl
 *
 *  [DEC]   <opcode> <REG> (size = 2)
 *
 *  [DIV]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = div
 *          - dir8  = divb
 *          - dir16 = divl
 *
 *  [END]   <opcode> (size = 1 byte)
 *
 *  [INC]   <opcode> <REG> (size = 2 bytes)
 *
 *  [JMPL]  <opcode> <DIR16> (size = 3 bytes)
 *
 *  [JMPS]  <opcode> <DIR8> (size = 2 bytes)
 *
 *  [JNZ]   <opcode> <DIR16> (size = 3 bytes)
 *
 *  [JZ]    <opcode> <DIR16> (size = 3 bytes)
 *
 *  [JA]    <opcode> <DIR16> (size = 3 bytes)
 *
 *  [JB]    <opcode> <DIR16> (size = 3 bytes)
 *
 *  [MOV]   <opcode> <FLAG> <REG | REG_INDIRECT> <REG | REG_INDIRECT | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = mov
 *          - dir8  = movb
 *          - dir16 = movl
 *          - indir = mov [rX]
 *
 *  [MUL]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = mul
 *          - dir8  = mulb
 *          - dir16 = mull
 *
 *  [NOP]   <opcode> (size = 1 byte)
 *
 *  [NOT]   <opcode> <REG> (size = 2 bytes)
 *
 *  [OR]    <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = or
 *          - dir8  = orb
 *          - dir16 = orl
 *
 *  [POP]   <opcode> <REG> (size = 2 bytes)
 *
 *  [PUSH]  <opcode> <FLAG> <REG | DIR08 | DIR16> (size = 3 or 4 bytes)
 *          - reg   = push
 *          - dir8  = pushb
 *          - dir16 = pushl
 *
 *  [RET]   <opcode> (size = 1 byte)
 *
 *  [SUB]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = sub
 *          - dir8  = subb
 *          - dir16 = subl
 *
 *  [SYSCALL] <opcode> (size = 1 byte)
 *
 *  [TEST]  <opcode> <REG> <REG> (size = 3 bytes)
 *
 *  [XCHG]  <opcode> <REG> <REG> (size = 3 bytes
 *
 *  [XOR]   <opcode> <FLAG> <REG> <REG | DIR8 | DIR16> (size = 4 or 5 bytes)
 *          - reg   = xor
 *          - dir8  = xorb
 *          - dir16 = xorl
 *
 *  -------------------------------------
 *
 *
 *
 *  ------------------------------------- Syscall:
 *
 *  r0 = syscall number
 *  r1 = arg1
 *  r2 = arg2
 *  r3 = arg3
 *  r4 = arg4
 *
 *
 *  syscalls supported: open(), read(), write(), close(), exit(), setuid(), setgid(), dup2(), send()
 *                      recv(), socket(), listen(), bind(), accept(), chdir(), chmod(), lseek(),
 *                      getpid(), getuid(), pause()
 *
 *  [sys_open]    r1 = uint16_t *
 *                r2 = uint16_t
 *                r3 = uint16_t
 *
 *  [sys_exit]    r1 = uint16_t
 *
 *  [sys_read]    r1 = uint16_t
 *                r2 = uint16_t *
 *                r3 = uint16_t
 *
 *  [sys_write]   r1 = uint16_t
 *                r2 = uint16_t *
 *                r3 = uint16_t
 *
 *  [sys_close]   r1 = uint16_t
 *
 *  [sys_exit]    r1 = uint16_t
 *
 *  [sys_setuid]  r1 = uint16_t
 *
 *  [sys_setgid]  r1 = uint16_t
 *
 *  [sys_dup2]    r1 = uint16_t
 *                r2 = uint16_t
 *
 *  [sys_send]    r1 = uint16_t
 *                r2 = uint16_t *
 *                r3 = uint16_t
 *                r4 = uint16_t
 *
 *
 *  [sys_recv]    r1 = uint16_t
 *                r2 = uint16_t *
 *                r3 = uint16_t
 *                r4 = uint16_t
 *
 *  [sys_socket]  r1 = uint16_t
 *                r2 = uint16_t
 *                r3 = uint16_t
 *
 *  [sys_listen]  r1 = uint16_t
 *                r2 = uint16_t
 *
 *  [sys_bind]    r1 = uint16_t (socket)
 *                r2 = uint16_t (port)
 *
 *  [sys_accept]  r1 = uint16_t (socket)
 *
 *  [SYS_CHDIR]   r1 = uint16_t *
 *
 *  [SYS_CHMOD]   r1 = uint16_t *
 *                r2 = uint16_t
 *
 *  [SYS_LSEEK]   r1 = uint16_t
 *                r2 = uint16_t
 *                r3 = uint16_t
 *
 *  [SYS_GETPID]  n/a
 *
 *  [SYS_GETUID]  n/a
 *
 *  [SYS_PAUSE]   n/a
 *
 *
 *
 *  The return value is set in r0.
 *
 *  --------------------------------------
 *
 *  -------------------------------------- Zero Flag:
 *
 *  ZF is set with following instructions:
 *
 *    - ADD
 *    - SUB
 *    - MUL
 *    - DIC
 *    - INC
 *    - DEC
 *    - OR
 *    - XOR
 *    - AND
 *    - NOT
 *    - TEST
 *    - CMP
 *
 *  --------------------------------------
 *
 *
 *  -------------------------------------- BF/AF Flag:
 *
 *  AF & BF is set with following instructions:
 *
 *    - CMP
 *
 *  AF & BF is used for JA and JB instructions.
 *
 *  --------------------------------------
 *
 *  ****************************************/

#define OP_FLAG_REG_REG                   0x00
#define OP_FLAG_REG_DIRECT08              0x01
#define OP_FLAG_REG_DIRECT16              0x02
#define OP_FLAG_REG                       0x03
#define OP_FLAG_DIRECT16                  0x04
#define OP_FLAG_DIRECT08                  0x05
#define OP_FLAG_REGINDIRECT_REG           0x06
#define OP_FLAG_REGINDIRECT_DIRECT08      0x07
#define OP_FLAG_REGINDIRECT_DIRECT16      0x08
#define OP_FLAG_REGINDIRECT_REGINDIRECT   0x09
#define OP_FLAG_REG_REGINDIRECT           0x0a

#define REG_0         0x00
#define REG_1         0x01
#define REG_2         0x02
#define REG_3         0x03
#define REG_4         0x04
#define REG_5         0x05
#define REG_6         0x06
#define REG_7         0x07
#define REG_SP        0x08
#define REG_BP        0x09

/* Stack */
#define OP_PUSH       0x01
#define OP_POP        0x03

/* Memory */
#define OP_MOV        0x04
/*#define OP_STRCPY     0x05*/

/* Arithmetics */
#define OP_ADD        0x06
#define OP_SUB        0x07
#define OP_MUL        0x08
#define OP_DIV        0x09
#define OP_INC        0x0A
#define OP_DEC        0x0B

/* Logic */
#define OP_OR         0x0C
#define OP_AND        0x0D
#define OP_XOR        0x0E
#define OP_NOT        0x0F

/* Control */
#define OP_JZ         0x10
#define OP_JNZ        0x11
#define OP_JMPS       0x16 /* jmp short operande 8 bits */
#define OP_TEST       0x17
#define OP_CMP        0x18
#define OP_CALL       0x19
#define OP_RET        0x1A
#define OP_JMPL       0x1B /* jmp long operande 16 bit */
#define OP_END        0x1C
#define OP_XCHG       0x1D
#define OP_JA         0x1E
#define OP_JB         0x1F

/* SYSCALLS */
#define OP_SYSCALL    0x30
#define OP_NOP        0x02

/**
 *
 * Syscalls Number
 *
 * **/

#define SYS_EXIT        0x01
#define SYS_OPEN        0x02
#define SYS_READ        0x03
#define SYS_WRITE       0x04
#define SYS_CLOSE       0x05
#define SYS_SETUID      0x06
#define SYS_SETGID      0x07
#define SYS_DUP2        0x08
#define SYS_SEND        0x09
#define SYS_RECV        0x0a
#define SYS_SOCKET      0x0b
#define SYS_LISTEN      0x0c
#define SYS_BIND        0x0d
#define SYS_ACCEPT      0x0e
#define SYS_CHDIR       0x0f
#define SYS_CHMOD       0x10
#define SYS_LSEEK       0x11
#define SYS_GETPID      0x12
#define SYS_GETUID      0x13
#define SYS_PAUSE       0x14

#endif     /* !__OP_H__ */

