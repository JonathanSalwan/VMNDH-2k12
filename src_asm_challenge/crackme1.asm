; Crackme #1 - NDH
;
; Affiche le fichier myxzJaKE.txt (dans le path courant)


#include inc/stdlib.inc


; strlen ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; r0 doit contenir le pointeur sur la chaine
; renvoi le la taille dans r0
.label 	strlen
	xor 	r1, r1		; int i
	xor	r2, r2		; contiendra notre char

.label	strlenbcl
	mov	r2, [r0] 	; eax contient l'@ de la chaine
	test	r2, r2		; test si le caractere est 0x00
	jz	:strlenend
	inc	r1
	inc	r0
	jmps	:strlenbcl

.label 	strlenend
	mov	r0, r1		; return i
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; printf ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; r0 contient la chaine à print
.label	printf
	mov	r7, r0 		; save ptr dans r7
	call	:strlen
	mov	r2, r7		; restore ptr
	mov	r3, r0		; strlen(str) dans r3
	movb	r1, #1		; stdout dans r1
	movb	r0, #4		; sys_write
	syscall
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; bad password ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.label 	badpassword
	movl	r0, :badpwd
	call 	:printf
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; good password ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.label 	goodpassword
	movl	r0, :flag
	call 	:disp_file_content
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; checkserial ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; r0 contient @buff[]
.label	checkserial
	mov	r7, r0		; save ptr buff dans r7
	movl	r6, :key	; r6 ptr sur key
	call	:strlen
	cmpb	r0, #9		; check si la taille est de 8 char
	jz	:sok0
	call	:badpassword
	end

.label 	sok0
	mov	r0, [r7]	; r0 contient buf[0]
	mov	r1, [r6]	; r1 contient key[0]
	xor	r0, r1
	cmpb	r0, #x78
	jz	:sok1
	call	:badpassword
	end

.label 	sok1
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[1]
	mov	r1, [r6]	; r1 contient key[1]
	xor	r0, r1
	cmpb	r0, #x44
	jz	:sok2
	call	:badpassword
	end

.label	sok2
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[2]
	mov	r1, [r6]	; r1 contient key[2]
	xor	r0, r1
	cmpb	r0, #x73
	jz	:sok3
	call	:badpassword
	end

.label	sok3
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[3]
	mov	r1, [r6]	; r1 contient key[3]
	xor	r0, r1
	cmpb	r0, #x6b
	jz	:sok4
	call	:badpassword
	end

.label	sok4
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[4]
	mov	r1, [r6]	; r1 contient key[4]
	xor	r0, r1
	cmpb	r0, #x61
	jz	:sok5
	call	:badpassword
	end

.label	sok5
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[5]
	mov	r1, [r6]	; r1 contient key[5]
	xor	r0, r1
	cmpb	r0, #x3e
	jz	:sok6
	call	:badpassword
	end

.label	sok6
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[6]
	mov	r1, [r6]	; r1 contient key[6]
	xor	r0, r1
	cmpb	r0, #x6e
	jz	:sok7
	call	:badpassword
	end

.label	sok7
	inc	r7
	inc	r6
	mov	r0, [r7]	; r0 contient buf[7]
	mov	r1, [r6]	; r1 contient key[7]
	xor	r0, r1
	cmpb	r0, #x5e
	jz	:endok
	call	:badpassword
	end

.label	endok
	xor	r0, r0
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; main ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.label 	main
	movl 	r0, :str1
	call 	:printf
	subb	sp, #32		; alloue char buff[32]
	mov	r2, sp		; r2 @buff[]
	movb	r1, #0		; stdin
	movb	r3, #31		; size read
	movb	r0, #3		; sys_read
	syscall
	mov	r0, r2		; place @buff dans r0
	call	:checkserial
	call	:goodpassword
	end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.label key
.db 0x02, 0x05, 0x03, 0x07, 0x8, 0x6, 0x1, 0x9

.label str1
.db "Sciteek protected storage #1",0x0a,"Enter your password: ",0x00

.label hint
.db "sciteek.nuitduhack.com:4001",0

.label flag
.db "myxzJaKE.txt",0

.label goodpwd
.db "Good password",0x0a,0x00

.label badpwd
.db "Bad password",0x0a,0x00

