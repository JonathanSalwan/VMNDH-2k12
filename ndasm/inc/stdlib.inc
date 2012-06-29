; NDASM standard lib
; written by virtualabs
;
; Functions implemented:
;
; - strlen(s)
; - print(s)
; - input(s, buffer, size)

#include inc/constants.inc
#include inc/string.inc


; print impl
; @param r0 string to display
; @return void

.label print
	; save r0 and r1 on stack
	push r0
	push r1
	; save string pointer
	mov r1,r0
	; get string's length
	call :strlen
	; write(stdout,string,size)
	mov r3,r0
	mov r2,r1
	movb r1, STDOUT
	movb r0, SYS_WRITE
	syscall
	pop r1
	pop r0
	ret


; memset impl
; @param r0 memory buffer to fill
; @param r1 value to put in buffer
; @param r2 buffer size

.label memset
	; save registers
	push r0
	push r3
	xor r3, r3
	.label @@
		cmp r3, r2
		jz :@f
		ja :@f
		mov [r0], r1
		inc r3
		inc r0
		jmps :@b
	.label @@
	; restore registers
	pop r3
	pop r0
	ret
	
; close impl
; @param r0 file descriptor to close

.label close
	mov r1, r0
	movb r0, SYS_CLOSE
	syscall
	ret
	
; open impl
; @param r0 pathname
; @param r1 flags

.label open
	mov r2, r1
	mov r1, r0
	movb r0, SYS_OPEN
	syscall
	ret
	
; read impl
; @param r0 file descriptor
; @param r1 dest buffer
; @param r2 size of buffer
; @return r0 nb of chars read

.label read
	mov r3, r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_READ
	syscall
	ret
	
; lseek impl
; @param r0 file descriptor
; @param r1 offset
; @param r2 whence

.label lseek
	push r3
	mov r3, r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_LSEEK
	syscall
	pop r3
	ret
	
; disp_file_content
; displays file content
; @param r0 filepath
; returns 1 on success, 0 if an error occured


.label disp_file_content
	; save registers
	push r1
	push r2
	push r3
	push r4
	push r5
	
	; open file
	movl r1, O_RDONLY
	call :open
	cmpl r0, #xFFFF
	jnz :@f
	
	; error (cannot open file)
	xor r0, r0
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	ret	
	
	.label @@
	
	; on recupere la taille du fichier
	mov r3, r0
	movl r1, #0
	movl r2, SEEK_END
	call :lseek
	mov r4, r0
	inc r4

	mov r0, r3
	movl r1, #0
	movl r2, SEEK_SET
	call :lseek

	; allocate content on stack
	sub sp, r4
	mov r5, sp
	
	; and read content
	mov r0, r3
	mov r1, sp	
	mov r2, r4
	call :read

	; force a null byte
	add r4, r5
	dec r4
	movb [r4], #0

	mov r0, r5
	call :print
	movb r0, #1

	; restore stack
	inc r4
	sub r4, r5
	add sp, r4	
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	ret		

