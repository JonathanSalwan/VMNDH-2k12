; NDH Prequals 2012
;
; VMNDH Crackme #1

; Principe du crackme:
; --------------------
;
; - Fournir un serial valide
; - Le serial est compose de 2 séries de 4 chiffres, et d'une clef de 2 chiffres
;   xxxx-yyyy-kk
; - A = atoi(xxxx), B=atoi(yyyy), C=atoi(kk) -> ((A+B)*k) xor 0xB33F == 0x1337

; Code avec CCA:
;
; jmps :@f
; .db 0x0D
; .label @@
; jmps :@f
; xor r0,r0
; movl r0, #xB33F
; add r1, r0
; push r1
; push r2
; ret
; 
; .label @@
;
; Ce code est désassemblé en:
;
; jmps $+3
; and ???
; ??? 
; <false disassembly>
; ...
; 
; Le but est de compliquer l'analyse statique


#include inc/stdlib.inc


.label main
	
	; on demande le password
	movl r0, :welcome
	call :print
	
	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	
	; on lit le password
	subb sp, #40
	mov r4, sp
	movb r0, STDIN
	mov r1, sp
	movb r2, #40
	call :read
	
	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
	
	; on recupere le nb d'octet
	mov r1, r0
	cmpb r1, #13
	jnz :wrong_format

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	; verification du format (xxxx-xxxx-xx)
	mov r2, r4
	xor r6, r6
	.label boucle_verif_cars	
		; on met dans r3 le car pointé
		mov r3, [r2]

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

		; si 5eme car et different de '-', mauvais format
		cmpb r6, #4
		jnz :@f
			cmpb r3, #45
			jnz :wrong_format
			jmpl :continue_boucle
		.label @@

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
		
		; si 10eme car different de '-', mauvais format
		cmpb r6, #9
		jnz :@f
			cmpb r3, #45
			jnz :wrong_format
			jmpl :continue_boucle
		.label @@

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

		; si < '0' alors mauvais format
		cmpb r3, #x30
		jb :wrong_format

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
	
		; si > '9' alors mauvais format
		cmpb r3, #x39
		ja :wrong_format

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
		
		.label continue_boucle
		inc r2
		inc r6
		cmpb r6, #11
		jnz :boucle_verif_cars

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	; extraction des nombres
	mov r2, r4
	addb r2, #4
	movb [r2], #0
	addb r2, #5
	movb [r2], #0
	addb r2, #3
	movb [r2], #0
	mov r2, r4
	mov r0, r2
	call :atoi
	mov r5, r0

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
		
	addb r2, #5
	mov r0, r2
	call :atoi
	mov r6, r0

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------	
	
	addb r2, #5
	mov r0, r2
	call :atoi
	mov r7, r0

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------
		
	; on calcule
	xor r0, r0
	add r0, r5
	add r0, r6
	mul r0, r7
	xorl r0, #xB33F
	cmpl r0, #x1337
	jnz :wrong_password

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	movl r0, :well_done
	call :print
	end	

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	.label wrong_format
		movl r0, :bad_password
		call :print
		end

	; -------- CCA + JUNK CODE
  jmps :@f
  .db 0x16
  .label @@
  jmpl :@f
  xor r0,r0
  add r1,r2
  push r1
  pop r2
  mov r1, [r2]
  addb r1, #1
  sub r2, r1
  cmp r2, r1
  sub r1, r0
  test r1, r1
  jmpl :@f
  cmp r2, r3
  push r2
  pop r3
  .label @@	
	; -------------------------

	.label wrong_password
		movl r0, :bad_password2
		call :print
		end
	
.label welcome
.db "Serial Number required: ",0

.label bad_password
.db "Bad password format",0x0A,0

.label bad_password2
.db "Bad password",0x0A,0

.label well_done
.db "Well done !",0x0A,0