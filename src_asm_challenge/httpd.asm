; Exploitme #1 - NDH 2012 Prequals

; nos includes
#include inc/stdlib.inc
#include inc/socket.inc

#define PORT "#8080"

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
	;mov r0, r4		; on accept() sur la socket serveur
	;call :accept


	; on rend la main quand une connexion est créée
	mov r1, r4		; on remet dans r1 la socket serveur
	ret

	.label create_sock_error
	end

; ici la vuln
.label  vuln
	pushl #xBEEF
	subl sp, #512
	mov r1, sp
	movl r2, #1020
	call :sock_recv
	mov r0, r1
	addl sp, #512
	pop r1
	cmpl r1, #xBEEF
	jz :__retvuln
	end
.label  __retvuln
	ret

.label 	receved
	subl sp, #512
	call :vuln
	addl sp, #512
	ret

; le main
.label main

	; on crée la socket et on attend une connexion
	call :create_sock_and_accept

	; on envoie le message de bienvenue
	mov r6, r1	; on sauve dans r6 la socket serveur
	;mov r7, r0	; on sauve dans r7 la socket client

	;subb sp, #32
	.label retry
	; recoit la commande
	mov r0, r6
	call :accept
	mov r7, r0	; on sauve dans r7 la socket client

	call :receved
	; r2 string receved
	mov r0, r2
	movl r1, :get
	movb r2, #2
	call :strncmp
	cmpb r0, #0
	jnz :closec
	; affiche la page si c'est GET
	mov r0, r7
	movl r1, :index
	call :send_str

.label closec
	; ferme la connexion client
	mov r0, r7
	call :close

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

.label index
.db "HTTP/1.0 200 OK",0x0A
.db "Content-Type : text/HTML",0x0A
.db "Content-Length : 70",0x0A,0x0A
.db "<html><center><b>Exploit Me if you can ;)</b></html></center>",0

.label get
.db "GET",0

.label socket_error_msg
.db "Cannot create server",0x0A,0

