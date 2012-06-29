; VMNDH String functions lib

#include inc/constants.inc

; strlen impl
; @param r0 input string
; @return r0 string length in bytes

.label strlen
	; save r1,r2 on stack
	push r1
	push r2
	; init r1,r2
	movl r1, #0
	movl r2, #0

	; loop until a zero char is found
	.label @@
		mov r1, [r0]
		test r1,r1
		inc r2
		inc r0
		jnz :@b

	; save size to r0
	dec r2
	mov r0,r2
	pop r2
	pop r1
	ret

; strncpy impl
; @param r0 dest string
; @param r1 source string
; @param r2 buffer size
; @return void

.label strncpy

	; save r3,r4
	push r1
	push r2
	push r3
	push r4
	; loop on chars
	xor r3, r3
	.label @@
		mov r4, [r1]
		test r4,r4
		jz :@f
		cmp r3, r2
		ja :@f
		mov [r0], r4
		inc r0
		inc r1
		inc r3
		jmps :@b
	.label @@
	pop r4
	pop r3
	pop r2
	pop r1
	ret

; strtoup impl
; @param r0 string to uppercase

.label strtoup
	; save registers
	push r0
	push r1

	; scan string
	.label __strtoup_loop
		mov r1, [r0]
		test r1, r1
		jz :__strtoup_end
		cmpb r1, #97
		jb :__strtoup_nothing
		cmpb r1, #122
		ja :__strtoup_nothing
		subb r1, #32
		mov [r0], r1
		.label __strtoup_nothing
		inc r0
		jmps :__strtoup_loop
	.label __strtoup_end
	pop r1
	pop r0
	ret

; strtolower impl
; @param r0 string to uppercase


.label strtolower
	; save registers
	push r0
	push r1

	; scan string
	.label __strtolower_loop
		mov r1, [r0]
		test r1, r1
		jz :__strtolower_end
		cmpb r1, #65
		jb :__strtolower_nothing
		cmpb r1, #90
		ja :__strtolower_nothing
		addb r1, #32
		mov [r0], r1
		.label __strtolower_nothing
		inc r0
		jmps :__strtolower_loop
	.label __strtolower_end

	; restore registers
	pop r1
	pop r0
	ret

; strncmp impl
; @param r0 string to compare
; @param r1 reference string
; @param r2 max string size
; @return r0 0 if equals, 1 if different

.label strncmp
	; save registers
	push r3
	push r4
	push r5
	xor r5, r5

	; compare strings
	.label __strncmp_loop
		cmp r5, r2
		ja :__strncmp_eq
		mov r3, [r0]
		mov r4, [r1]
		cmp r3, r4
		jz :__strncmp_next
		movb r0, #1
		jmps :__strncmp_end
		.label __strncmp_next
		test r3, r3
		jz :__strncmp_eq
		inc r0
		inc r1
		inc r5
		jmps :__strncmp_loop

	.label __strncmp_eq
	xor r0, r0
	.label __strncmp_end
	pop r5
	pop r4
	pop r3
	ret


; atoi impl
; @param r0 string to convert
; @return r0 numeric value of string (decimal)

.label atoi
	; save registers
	push r1
	push r2
	push r3
	push r4

	; convert decimal string
	xor r2, r2
	; put strlen into r3
	mov r3, r0
	call :strlen
	xchg r3, r0
	dec r3

	.label __atoi_loop
		mov r1, [r0]
		test r1,r1
		jz :__atoi_end		; if end of string then jump to end

		; check if current char is a decimal digit
		cmpb r1, #x30
		jb :__atoi_error
		cmpb r1, #x39
		ja :__atoi_error
		subb r1, #x30

		; if yes, compute the value
		xor r4, r4
		movb r4, #1
		push r3
		test r3, r3
		jz :__atoi_nopow
		.label __atoi_pow
			mulb r4, #10
			dec r3
			test r3, r3
			jnz :__atoi_pow
		.label __atoi_nopow
		mul r4, r1
		add r2, r4
		pop r3
		dec r3
		; next char
		inc r0
		jmps :__atoi_loop
	.label __atoi_error
	xor r2,r2
	.label __atoi_end
	mov r0, r2

	; restore registers
	pop r4
	pop r3
	pop r2
	pop r1
	ret


