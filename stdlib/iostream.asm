print:
push bp
mov bp, sp
load gp0, [bp + 8]

.loop:
	load gp1, const[gp0]
	cmp gp1, 0
	jz .end
	out gp1
	movi gp1, 1
	add gp0, gp0, gp1
	jmp .loop

.end:
mov sp, bp
pop bp
ret