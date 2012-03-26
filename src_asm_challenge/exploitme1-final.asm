; Exploitme #1 - NDH 2012 Prequals

; Objectif: recuperer le mot de passe (qui fait office de flag) pour s'authentifier et recup le flag
; Contrainte: une redirection vers la routine d'affichage du flag seule ne fonctionne pas
; Exploitations possibles:
;  1. inc r7 et call disp_file_content
;  2. affichage du mot de passe (movl r0, :good_pwd | call :print)

; nos includes
#include inc/stdlib.inc

#define SEEK_SET	"#0"
#define SEEK_CUR	"#1"
#define SEEK_END	"#2"
#define O_CREAT 		"#4"
#define O_EXCL			"#8"
#define O_TRUNC   	"#16"
#define O_APPEND  	"#32"
#define O_RDONLY    "#0"
#define O_WRONLY    "#1"
#define O_RDWR      "#2"

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
  test r7,r7
  jz :cheat_detected
  
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
	.label cheat_detected
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

.label ask_password
	; affiche l'invite
	movl r0, :pwd_msg
	call :print
	subb sp, #50
	mov r5, sp
	; on reserve 100 octets pour le mdp
	movl r0, STDIN
	mov r1,  r5
	movb r2, #54
	; on lit 54 octets
	call :read
	
	; on copie
	movl r0, #x1111
	mov r1, r5
	movl r2, #50
	call :strncpy
	addb sp, #50
	ret
	
.label check_password
	; on compare le mdp
	movl r0, #x1111
	movl r1, :good_pwd
	movl r2, #256
	call :strncmp
	jnz :@f
	inc r7
	movl r0, :payload
	call :disp_file_content
	jmps :fin
	.label @@
	movl r0, :bad_guy
	call :print
	.label fin
	ret
	
.label main
  xor r7,r7
	call :ask_password
	call :check_password
	end
	
	
.label pwd_msg
.db "Password (required): ",0

.label well_done
.db "Good job dude !",0x0A,0

.label bad_guy
.db "Try again !",0x0A,0

.label payload
.db "ep1-payload.txt",0

.label good_pwd
.db "ArkamAsylut",0x0A,0
