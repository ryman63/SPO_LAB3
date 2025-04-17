[section code]

main:
;prologue
subi sp, sp, 28
;expressions
movi gp0, 5
store gp0, [bp - 4]
movi gp0, 10
store gp0, [bp - 8]
movi gp0, 15
store gp0, [bp - 12]
movi gp0, 20
store gp0, [bp - 16]
movi gp0, 25
store gp0, [bp - 20]
movi gp0, str_0
store gp0, [bp - 24]
load gp0, [bp - 8]
push gp0
load gp2, [bp - 4]
movi gp3, 1
add gp1, gp2, gp3
push gp1
call sum
store tmp, [bp - 28]
movi gp0, str_0
push gp0
call print
load gp1, [bp - 28]
load gp2, [bp - 20]
add gp0, gp1, gp2
store gp0, [bp - 28]
;epilogue
hlt

str_0: db "sum", 0

sum:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
load gp1, [bp + 8]
load gp2, [bp + 12]
add gp0, gp1, gp2
store gp0, [bp - 4]
;epilogue
mov tmp, gp0
mov sp, bp
pop bp
ret

;BuiltInFunc
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