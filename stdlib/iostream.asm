print:
push bp
mov bp, sp
load gp0, [bp + 8]

.loop:
	load8 gp1, const[gp0+gp2]
	movi gp2, 0
	cmp gp1, gp2
	je .end
	out gp1
	movi gp3, 1
	add gp2, gp2, gp3
	jmp .loop

.end:
mov sp, bp
pop bp
ret