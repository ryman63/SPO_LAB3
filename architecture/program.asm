[section code]

main:
;prologue
subi sp, sp, 28
;expressions
.cnd_1:
movi gp0, 5
store gp0, [bp - 4]
.cnd_2:
movi gp0, 10
store gp0, [bp - 8]
.cnd_3:
movi gp0, 15
store gp0, [bp - 12]
.cnd_4:
movi gp0, 20
store gp0, [bp - 16]
.cnd_5:
movi gp0, 25
store gp0, [bp - 20]
.cnd_6:
movi gp0, str_0
store gp0, [bp - 24]
.cnd_7:
load gp0, [bp - 8]
push gp0
load gp2, [bp - 4]
movi gp3, 1
add gp1, gp2, gp3
push gp1
call sum
store tmp, [bp - 28]
.cnd_8:
load gp1, [bp - 28]
movi gp2, 16
cmp gp1, gp2
jne .cnd_9
.cnd_10:
movi gp0, str_1
push gp0
call print
jmp .cnd_12
.cnd_9:
load gp0, [bp - 24]
push gp0
call print
.cnd_12:
load gp1, [bp - 28]
load gp2, [bp - 20]
add gp0, gp1, gp2
store gp0, [bp - 28]
;epilogue
hlt

str_0: db "succes", 0
str_1: db "error", 0

sum:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
.cnd_15:
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
	movi gp2, 0
	cmp gp1, gp2
	je .end
	out gp1
	movi gp1, 1
	add gp0, gp0, gp1
	jmp .loop

.end:
mov sp, bp
pop bp
ret