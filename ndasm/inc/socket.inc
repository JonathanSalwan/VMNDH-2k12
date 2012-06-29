; VMNDH socket

#include inc/constants.inc

#define	SOCK_STREAM		"#x01"
#define	SOCK_DGRAM		"#x02"
#define	AF_INET			"#x02"

; socket impl
; @param r0 domain
; @param r1 type
; @param r2 protocol

.label socket
	; wrap the syscall
	push r3
	mov r3, r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_SOCKET
	syscall
	pop r3
	ret


; bind impl
; @param r0 socket
; @param r1 port

.label bind
	; wrap the syscall
	push r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_BIND
	; bind socket
	syscall
	pop r2
	ret


; listen impl
; @param r0 socket

.label listen
	; wrap the syscall
	movb r2, #5
	mov r1, r0
	movb r0, SYS_LISTEN
	syscall
	ret

; accept
; @param r0 socket

.label accept
	mov r1, r0
	movb r0, SYS_ACCEPT
	syscall
	ret

; send impl
; @param r0 socket
; @param r1 data to send
; @param r2 size to send

.label sock_send
	push r4
	xor r4,r4
	mov r3, r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_SEND
	syscall
	pop r4
	ret

; recv impl
; @param r0 socket
; @param r1 dest buffer
; @param r2 size of buffer

.label sock_recv
	push r4
	xor r4, r4 ;flags = 0
	mov r3, r2
	mov r2, r1
	mov r1, r0
	movb r0, SYS_RECV
	syscall
	pop r4
	ret




