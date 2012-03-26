; VMNDH Crackme #3 -- "vmception"
;
; OPCODES:
;
; OP [| RX [|RX]]
;
; OPS: 
;
; - MOV RX, RX			(01)
; - MOV RX, IMM8		(02)
; - INC RX 					(03)
; - DEC RX					(04)
; - ADD RX, RX			(05)
; - XOR RX, RX			(06)
; - CMP	RX, RX			(07)
; - JZ ADDR					(08)
; - JNZ ADDR				(09)
; - JMP ADDR				(0A)
; - END 						(07)
;
; REGISTRES:
; R0-R7: registres generaux
; RLOOP: registre de loop
; IP: pointeur d'instruction
;
; PAS DE STACK
;
; MAPPING MEMOIRE:
;
; 0000 - R0 R1 R2 R3 R4 R5 R6 R7 R8
; 0008 - ZF IP
;
;
; PROGRAMME EXECUTE EN VM (8 bits)
;
; 5c1t33k
;  xor
; Mar10Ld
; -------
;R0123456
;
; 5 xor M = 0x35 xor 0x4d = 0x78
; c xor a = 0x63 xor 0x61 = 0x02
; 1 xor r = 0x31 xor 0x72 = 0x43
; t xor 1 = 0x74 xor 0x31 = 0x45
; 3 xor 0 = 0x33 xor 0x30 = 0x03
; 3 xor L = 0x33 xor 0x4c = 0x7f
; k xor d = 0x6b xor 0x64 = 0x0f
;
;
; JMP @@
; fin:
; XOR R0,R0
; END
; @@:
; MOV R7, 0x4D
; XOR R0, R7
; MOV R7, 0x78
; CMP R0, R7
; JNZ fin
; MOV R7, 0x97
; XOR R1, R7
; MOV R7, 0x02
; CMP R0, R7
; JNZ fin
; MOV R7, 0x72
; XOR R1, R7
; MOV R7, 0x43
; CMP R0, R7
; JNZ fin
; MOV R7, 0x31
; XOR R1, R7
; MOV R7, 0x45
; CMP R0, R7
; JNZ fin
; MOV R7, 0x30
; XOR R1, R7
; MOV R7, 0x03
; CMP R0, R7
; JNZ fin
; MOV R7, 0x4C
; XOR R1, R7
; MOV R7, 0x7F
; CMP R0, R7
; JNZ fin
; MOV R7, 0x64
; XOR R1, R7
; MOV R7, 0x0F
; CMP R0, R7
; JNZ fin
; MOV R0, 0x1
; END
;
; Fichier de flag: 0zuNEiu4.txt

#include inc/stdlib.inc

;--------------------------------
;----       Constants         ---
;--------------------------------

#define _R0	"#x00"
#define _ZF "#x08"
#define _IP "#x09"
#define _CS "#x0A"

#define OP_MOV_RR "#x01"
#define OP_MOV_RI "#x02"
#define OP_INC    "#x03"
#define OP_DEC    "#x04"
#define OP_ADD    "#x05"
#define OP_XOR    "#x06"
#define OP_CMP    "#x07"
#define OP_JZ     "#x08"
#define OP_JNZ    "#x09"
#define OP_JMP    "#x0A"
#define OP_END    "#x0B"


;--------------------------------
;---- Initialisation de la VM ---
;--------------------------------

; load_bytecode
;
; r0: bytecode addr
; r1: bytecode size

.label load_bytecode
	push r2
	movl r2, _CS
	.label @@
		mov [r2],[r0]
		inc r0
		inc r2
		dec r1
		test r1,r1
		jnz :@b
	pop r2
	ret

; init_vm
;
; r0: IP init value
; r1: Param1
; r2: Param2
; r3: Param3

.label init_vm
	; save registers	
	push r4
	push r5
	
	; initialize vm registers R0->R7
	xor r4, r4
	movl r4, _R0
	movb r5, #8
	.label @@
		movb [r4], #0
		inc r4
		dec r5
		test r5,r5
		jnz :@b
	
	; initialize IP
	push r1
	addl r0, _CS
	movl r1, _IP
	mov [r1], r0
	pop r1
		
	; initialize registers with parameters
	push r0
	movl r0, _R0
	mov [r0], r1
	inc r0
	mov [r0], r2
	inc r0
	mov [r0], r3
	pop r0
	
	;return
	pop r5
	pop r4
	ret


;--------------------------------
;---- Manip des registres
;--------------------------------


; vm_get_reg
;
; r0: reg index
;
; return in r0 the reg content

.label vm_get_reg
	push r1
	movl r1, _R0
	add r1, r0
	mov r0, [r1]
	pop r1
	ret	


; vm_set_reg
;
; r0: reg index
; r1: reg value

.label vm_set_reg
	push r2
	movl r2, _R0
	add r2, r0
	mov [r2], r1
	pop r2
	ret	


; read_byte_ip
;

.label read_byte
	push r1
	push r2
	movl r0, _IP
	movl r1, [r0]
	movl r2, [r1]
	inc r1
	mov [r0], r1
	mov r0, r2
	pop r2
	pop r1
	ret

; jump_to
; 
; r0: addr to jump

.label jump_to
	push r1
	push r2
	movl r1, _IP
	movl r2, _CS
	add r2, r0
	mov [r1], r2
	pop r2	
	pop r1
	ret
	
;--------------------------------
;---- Operations math
;--------------------------------

; ADD

.label __vm_add
	; save registers
	push r2
	push r3
	
	; save indexes
	push r0
	
	; get reg1
	call :read_byte
	call :vm_get_reg
	mov r2, r0 ; R0 value in r2
	
	; get reg2
	call :read_byte
	call :vm_get_reg
	mov r3, r0 ; R1 value in r3
	
	; add
	add r2, r3
	pop r0
	mov r1, r2
	call :vm_set_reg
	
	; end
	pop r3
	pop r2
	ret


; XOR

.label __vm_xor
	; save registers
	push r0
	push r1
	push r2
	push r3
	
	; read dest reg index
	call :read_byte
	mov r3, r0
	; read src reg value
	call :read_byte
	call :vm_get_reg
	mov r2, r0
	; read dest reg value
	mov r0, r3
	call :vm_get_reg
	xor r0, r2
	mov r1, r0
	mov r0, r3
	call :vm_set_reg
	
	; restore registers
	pop r3
	pop r2
	pop r1
	pop r0

	; return
	ret

; INC	
	
.label __vm_inc
	; save registers
	push r0
	push r1
	push r2
	
	; read reg index
	call :read_byte
	mov r2, r0
	; get reg value
	call :vm_get_reg
	; inc value
	inc r0
	; set reg value
	mov r1, r0
	mov r0, r2
	call :vm_set_reg
	
	; restore registers
	pop r2
	pop r1
	pop r0
	
	; return
	ret


; DEC
	
.label __vm_dec
	; save registers
	push r0
	push r1
	push r2
	
	; read reg index
	call :read_byte
	mov r2, r0
	; get reg value
	call :vm_get_reg
	; inc value
	dec r0
	; set reg value
	mov r1, r0
	mov r0, r2
	call :vm_set_reg
	
	; restore registers
	pop r2
	pop r1
	pop r0
	
	; return
	ret

; MOV RX, RX
	
.label __vm_mov_rr
	; save registers
	push r0
	push r1
	push r2
	
	; get first reg index
	call :read_byte
	; save it in r1
	mov r1, r0
	
	; get second reg index
	call :read_byte
	; read reg content
	call :vm_get_reg
	; save reg value in r2
	mov r2, r0
	; put dest reg index in r0
	mov r0, r1
	; put reg value in r1
	mov r1, r2
	; set register
	call :vm_set_reg
	
	; restore registers
	pop r2
	pop r1
	pop r0
	
	; return
	ret

; MOV RX, IMM8
	
.label __vm_mov_ri
	; save registers
	push r0
	push r1
	push r2
	
	; get first reg index
	call :read_byte
	; save it in r1
	mov r1, r0
	
	; get value
	call :read_byte
	mov r2, r0
	mov r0, r1
	mov r1, r2
	call :vm_set_reg

	; restore registers
	pop r2
	pop r1
	pop r0
	
	; return
	ret


; CMP

.label __vm_cmp
	; save registers
	push r0
	push r1
	push r2
	
	; read first reg
	call :read_byte
	call :vm_get_reg
	mov r1, r0
	
	; read second reg
	call :read_byte
	call :vm_get_reg
	mov r2, r0
	
	; cmp values
	cmp r1, r2
	; ZF=0 by default
	movl r1, #0
	jnz :@f
	; if zero, ZF=1
	inc r1
	.label @@	
	movl r0, _ZF
	call :vm_set_reg
	
	pop r2
	pop r1
	pop r0
	ret
	
; JZ ADDR

.label __vm_jz
	; save registers
	push r0
	push r1
	
	; read addr
	call :read_byte
	mov r1, r0	
	
	; check if ZF is set to 1
	movl r0, _ZF
	call :vm_get_reg
	test r0, r0
	jz :@f
	
	; if ZF==1, then jump to addr
	mov r0, r1 
	call :jump_to
	
	.label @@	
	pop r1
	pop r0
	ret

; JNZ ADDR

.label __vm_jnz
	; save registers
	push r0
	push r1
	
	; read addr
	call :read_byte
	mov r1, r0	
	
	; check if ZF is set to 1
	movl r0, _ZF
	call :vm_get_reg
	test r0, r0
	jnz :@f
	
	; if ZF==1, then jump to addr
	mov r0, r1 
	call :jump_to
	
	.label @@	
	pop r1
	pop r0
	ret


; JMP 
.label __vm_jmp
	; read addr
	call :read_byte
	; jump to addr
	call :jump_to
	ret

;--------------------------------
;---- vm_main
;--------------------------------

.label vm_run

	push r2
	
	.label __vm_cycle
		call :read_byte

		; switch emulation
		; if something goes wrong, call __vm_error handler

		; if opcode==7 (END), then exit exec loop
		cmpb r0, OP_END
		jnz :@f
		jmpl :__vm_end
		
		; if opcode == MOV RX, RX
		.label @@		
		cmpb r0, OP_MOV_RR
		jnz :@f
		call :__vm_mov_rr
		jmpl :__vm_cycle
		
		; if opcode == MOV RX, IMM8
		.label @@
		cmpb r0, OP_MOV_RI
		jnz :@f
		call :__vm_mov_ri
		jmpl :__vm_cycle
		
		; if opcode == INC RX
		.label @@
		cmpb r0, OP_INC
		jnz :@f
		call :__vm_inc
		jmpl :__vm_cycle

		; if opcode == DEC RX
		.label @@
		cmpb r0, OP_DEC
		jnz :@f
		call :__vm_dec
		jmpl :__vm_cycle

		; if opcode == ADD RX, RX
		.label @@
		cmpb r0, OP_ADD
		jnz :@f
		call :__vm_add
		jmpl :__vm_cycle

		; if opcode == XOR RX, RX
		.label @@
		cmpb r0, OP_XOR
		jnz :@f
		call :__vm_xor
		jmpl :__vm_cycle


		; if opcode == CMP RX, RX
		.label @@
		cmpb r0, OP_CMP
		jnz :@f
		call :__vm_cmp
		jmpl :__vm_cycle


		; if opcode == JZ ADDR
		.label @@
		cmpb r0, OP_JZ
		jnz :@f
		call :__vm_jz
		jmpl :__vm_cycle


		; if opcode == JNZ ADDR	call :ask_pwd
		.label @@
		cmpb r0, OP_JNZ
		jnz :@f
		call :__vm_jnz
		jmpl :__vm_cycle

		; if opcode == JMP ADDR
		.label @@
		cmpb r0, OP_JMP
		jnz :@f
		call :__vm_jmp
		jmpl :__vm_cycle		
		
		.label @@
		jmpl :__vm_error
	
	; end program, retrieve R0 and R1 value in r0 and r1
	.label __vm_end
	movl r0, #0
	call :vm_get_reg
	mov r1, r0
	movl r0, #1
	call :vm_get_reg
	mov r2, r0
	mov r0, r1
	mov r1, r2

	; restore saved registers and return
	pop r2
	ret

.label __vm_error
	; display error message
	movl r0, :vm_error_msg
	call :print
	; quit !
	end


;--------------------------------
;---- Our main
;--------------------------------

.label ask_pwd
	push r1
	push r2
	movl r0, :ask_pwd_msg
	call :print
	movl r0, STDIN
	movl r1, #0
	movl r2, #8
	call :read
	pop r2
	pop r1
	ret

.label main
	movl r0, :prog
	movl r1, :fin_prog
	sub r1, r0
	call :load_bytecode
	movl r0, #0
	movl r1, #x35
	movl r2, #0
	movl r3, #0
	call :init_vm
	; paste password to registers (little trick, do not use vm_set_reg)
	call :ask_pwd
	; call the main check
	call :vm_run
	
	; check return
	movl r0, #0
	call :vm_get_reg
	test r0,r0
	jz :@f
	movl r0, :flag
	call :disp_file_content
	end
	.label @@
	movl r0, :failer
	call :print
	end
	
.label prog
.db 0x0A,0x06
.db 0x06,0x00,0x00 
.db 0x0B
; check R0
.db 0x02,0x07,0x4D
.db 0x06,0x00,0x07
.db 0x02,0x07,0x78
.db 0x07,0x00,0x07
.db 0x09,0x02
; check 41
.db 0x02,0x07,0x61
.db 0x06,0x01,0x07
.db 0x02,0x07,0x02
.db 0x07,0x01,0x07
.db 0x09,0x02
; check R2
.db 0x02,0x07,0x72
.db 0x06,0x02,0x07
.db 0x02,0x07,0x43
.db 0x07,0x02,0x07
.db 0x09,0x02
; check R3
.db 0x02,0x07,0x31
.db 0x06,0x03,0x07
.db 0x02,0x07,0x45
.db 0x07,0x03,0x07
.db 0x09,0x02
; check R4
.db 0x02,0x07,0x30
.db 0x06,0x04,0x07
.db 0x02,0x07,0x03
.db 0x07,0x04,0x07
.db 0x09,0x02
; check R5
.db 0x02,0x07,0x4C
.db 0x06,0x05,0x07
.db 0x02,0x07,0x7F
.db 0x07,0x05,0x07
.db 0x09,0x02
; check R6
.db 0x02,0x07,0x64
.db 0x06,0x06,0x07
.db 0x02,0x07,0x0F
.db 0x07,0x06,0x07
.db 0x09,0x02
; success
.db 0x02,0x00,0x01
.db 0x0B
.label fin_prog

.label ask_pwd_msg
.db "Please enter Sciteek admin password: ",0

.label hint
.db "sciteek.nuitduhack.com:4002",0

.label flag
.db "0zuNEiu4.txt",0

.label failer
.db "Password is wrong.",0x0A,0

.label vm_error_msg
.db "Unhandled exception occured during execution. Exiting.",0x0A,0


