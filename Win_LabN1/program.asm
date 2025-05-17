[section code]

main:
;prologue
subi sp, sp, 32
;expressions
.cnd_0:
.cnd_1:
movi gp0, 5
store gp0, [bp - 4]
jmp .cnd_2
.cnd_2:
movi gp0, 4
store gp0, [bp - 8]
jmp .cnd_3
.cnd_3:
load gp1, [bp - 8]
movi gp2, 4
cmp gp1, gp2
jl .cnd_6
jmp .cnd_7
.cnd_4:
.cnd_5:
.cnd_6:
movi gp0, 10
store gp0, [bp - 12]
jmp .cnd_8
.cnd_7:
load gp1, [bp - 4]
load gp2, [bp - 4]
add gp0, gp1, gp2
store gp0, [bp - 4]
jmp .cnd_9
.cnd_8:
movi gp0, 15
store gp0, [bp - 16]
jmp .cnd_10
.cnd_9:
movi gp1, 1
movi gp2, 2
add gp0, gp1, gp2
store gp0, [bp - 12]
jmp .cnd_3
.cnd_10:
movi gp0, 20
store gp0, [bp - 20]
jmp .cnd_11
.cnd_11:
movi gp0, 25
store gp0, [bp - 24]
jmp .cnd_12
.cnd_12:
movi gp0, str_0
store gp0, [bp - 28]
jmp .cnd_13
.cnd_13:
load gp0, [bp - 12]
push gp0
load gp2, [bp - 4]
movi gp3, 1
add gp1, gp2, gp3
push gp1
call sum
store tmp, [bp - 32]
jmp .cnd_14
.cnd_14:
load gp1, [bp - 32]
movi gp2, 51
cmp gp1, gp2
jne .cnd_15
jmp .cnd_16
.cnd_15:
movi gp0, str_1
push gp0
call print
.cnd_16:
load gp1, [bp - 32]
load gp2, [bp - 24]
add gp0, gp1, gp2
store gp0, [bp - 32]
.cnd_17:
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
.cnd_18:
.cnd_19:
load gp1, [bp + 8]
load gp2, [bp + 12]
add gp0, gp1, gp2
store gp0, [bp - 4]
.cnd_20:
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