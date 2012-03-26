; Exploitme #1 - NDH 2012 Prequals

; nos includes
#include inc/stdlib.inc
#include inc/socket.inc

#define PORT "#1337"

; @param r0 socket
; @param r1 string à envoyer


.label send_str
	push r0
	push r1		; on sauve r1
	mov r0, r1
	call :strlen
	mov r2, r0 	; size dans r2
	pop r1		; on restaure r1 (data à envoyer)
	pop r0		; r0 -> socket
	call :sock_send
	ret

.label create_sock_and_accept
	; on crée notre socket (socket(AF_INET, SOCK_STREAM, 0)
	movb r0, AF_INET
	movb r1, SOCK_STREAM
	movb r2, #0
	call :socket
	mov r4, r0		; on sauve dans r4 la socket serveur

	; on la bind sur un port défini
	movl r1, PORT
	call :bind
	cmpl r0, #xFFFF
	jz :create_sock_error

	; on listen
	mov r0, r4
	call :listen

	; on accept()
	mov r0, r4		; on accept() sur la socket serveur
	call :accept


	; on rend la main quand une connexion est créée
	mov r1, r4		; on remet dans r1 la socket serveur
	ret

	.label create_sock_error
	end

.label ask_passwd
	mov r0, r7
	subb sp, #200
	mov r1, sp
	movl r2, #202
	call :sock_recv

	; on copie en 0
	movl r0, #x1111	; [0] -> entered password
	mov r1, sp
	movb r2, #203
	call :strncpy
	addb sp, #200
	ret

; @param r0 password to compare
.label compare_password
	movb r0, #0
	movl r1, :password
	movb r2, #43
	call :strncmp
	ret

; le main
.label main

	; on crée la socket et on attend une connexion
	call :create_sock_and_accept

	; on envoie le message de bienvenue
	mov r6, r1	; on sauve dans r6 la socket serveur
	mov r7, r0	; on sauve dans r7 la socket client

	.label retry

	mov r0, r7
	movl r1, :welcome_msg
	call :send_str

	; on alloue de la place pour le mot de passe
	call :ask_passwd

	call :compare_password
	test r0, r0
	jnz :bad_password

	mov r0, r7
	movl r1, :syeah
	call :send_str
	jmpl :terminate

	.label bad_password
	mov r0, r7
	movl r1, :bad_password_msg
	call :send_str
	jmpl :retry

	.label terminate
	; on ferme la socket client
	mov r0, r7
	call :close
	; puis la socket serveur
	mov r0, r6
	call :close
	end

	.label end
	movl r0, :socket_error_msg
	call :print
	end

.label bad_password_msg
.db "Bad password",0x0A,0

.label welcome_msg
.db "Enter admin password: ",0

.label socket_error_msg
.db "Cannot create server",0x0A,0

.label syeah
.db "Administrator rights granted. Quitting.",0x0A,0


.label password
.db "ArkamAsylum",0x0A,0
