; NDH Hello world sample
movl r0, :helloworld
movl r1, #0
movl r2, #0
movl r5, #0

.label 	loop
	mov r2, [r0]
	test r2,r2
	inc r5
	inc r0
	jnz :loop

movb r0, #4
movb r1, #1
movl r2, :helloworld
mov r3, r5
syscall
end

.label helloworld
.db "Hello World !",0x0A,0
