[section code]

main:
;prologue
subi sp, sp, 1056
;expressions
.cnd_0:
.cnd_1:
movi gp1, 16
sub gp0, bp, gp1
movi gp2, 1
movi gp3, 0
store32 gp2, [gp0 + gp3]
movi gp2, 2
movi gp3, 4
store32 gp2, [gp0 + gp3]
movi gp2, 3
movi gp3, 8
store32 gp2, [gp0 + gp3]
movi gp2, 4
movi gp3, 12
store32 gp2, [gp0 + gp3]
jmp .cnd_2
.cnd_2:
movi gp0, str_0
push gp0
call print
jmp .cnd_3
.cnd_3:
mov gp0, sp
store gp0, [bp - 20]
jmp .cnd_4
.cnd_4:
load gp0, [bp - 20]
push gp0
call input
jmp .cnd_5
.cnd_5:
movi gp0, str_1
push gp0
call print
jmp .cnd_6
.cnd_6:
mov gp0, sp
store gp0, [bp - 276]
jmp .cnd_7
.cnd_7:
load gp0, [bp - 276]
push gp0
call input
jmp .cnd_8
.cnd_8:
movi gp0, str_2
push gp0
call print
jmp .cnd_9
.cnd_9:
mov gp0, sp
store gp0, [bp - 532]
jmp .cnd_10
.cnd_10:
load gp0, [bp - 532]
push gp0
call input
jmp .cnd_11
.cnd_11:
load gp0, [bp - 532]
push gp0
call atoi
store tmp, [bp - 788]
jmp .cnd_12
.cnd_12:
movi gp0, 0
store gp0, [bp - 792]
jmp .cnd_13
.cnd_13:
load gp0, [bp - 20]
push gp0
call atoi
store tmp, [bp - 796]
jmp .cnd_14
.cnd_14:
load gp0, [bp - 276]
push gp0
call atoi
store tmp, [bp - 800]
jmp .cnd_15
.cnd_15:
load gp1, [bp - 788]
movi gp3, 16
sub gp2, bp, gp3
movi gp4, 0
movi gp6, 4
mul gp4, gp4, gp6
load32 gp5, [gp2 + gp4]
cmp gp1, gp5
je .cnd_17
jmp .cnd_18
.cnd_16:
.cnd_17:
load gp0, [bp - 800]
push gp0
load gp1, [bp - 796]
push gp1
call sum_2
store tmp, [bp - 792]
jmp .cnd_18
.cnd_18:
load gp1, [bp - 788]
movi gp3, 16
sub gp2, bp, gp3
movi gp4, 1
movi gp6, 4
mul gp4, gp4, gp6
load32 gp5, [gp2 + gp4]
cmp gp1, gp5
je .cnd_20
jmp .cnd_21
.cnd_19:
.cnd_20:
load gp0, [bp - 800]
push gp0
load gp1, [bp - 796]
push gp1
call sub_2
store tmp, [bp - 792]
jmp .cnd_21
.cnd_21:
load gp1, [bp - 788]
movi gp3, 16
sub gp2, bp, gp3
movi gp4, 2
movi gp6, 4
mul gp4, gp4, gp6
load32 gp5, [gp2 + gp4]
cmp gp1, gp5
je .cnd_23
jmp .cnd_24
.cnd_22:
.cnd_23:
load gp0, [bp - 800]
push gp0
load gp1, [bp - 796]
push gp1
call mul_2
store tmp, [bp - 792]
jmp .cnd_24
.cnd_24:
load gp1, [bp - 788]
movi gp3, 16
sub gp2, bp, gp3
movi gp4, 3
movi gp6, 4
mul gp4, gp4, gp6
load32 gp5, [gp2 + gp4]
cmp gp1, gp5
je .cnd_26
jmp .cnd_27
.cnd_25:
.cnd_26:
load gp0, [bp - 800]
push gp0
load gp1, [bp - 796]
push gp1
call div_2
store tmp, [bp - 792]
jmp .cnd_27
.cnd_27:
mov gp0, sp
store gp0, [bp - 804]
jmp .cnd_28
.cnd_28:
load gp0, [bp - 804]
push gp0
load gp1, [bp - 792]
push gp1
call itoa
jmp .cnd_29
.cnd_29:
load gp0, [bp - 804]
push gp0
call print
.cnd_30:
;epilogue
mov tmp, tmp
hlt

str_0: db "input a: ", 0
str_1: db "input b: ", 0
str_2: db "input number of op, 1 - sum, 2 - sub, 3 - mul, 4 - div: ", 0

div_2:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
.cnd_31:
.cnd_32:
load gp1, [bp + 8]
load gp2, [bp + 12]
div gp0, gp1, gp2
store gp0, [bp - 4]
.cnd_33:
;epilogue
mov tmp, gp0
mov sp, bp
pop bp
ret


mul_2:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
.cnd_34:
.cnd_35:
load gp1, [bp + 8]
load gp2, [bp + 12]
mul gp0, gp1, gp2
store gp0, [bp - 4]
.cnd_36:
;epilogue
mov tmp, gp0
mov sp, bp
pop bp
ret


sub_2:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
.cnd_37:
.cnd_38:
load gp1, [bp + 8]
load gp2, [bp + 12]
sub gp0, gp1, gp2
store gp0, [bp - 4]
.cnd_39:
;epilogue
mov tmp, gp0
mov sp, bp
pop bp
ret


sum_2:
;prologue
push bp
mov bp, sp
subi sp, sp, 4
;expressions
.cnd_40:
.cnd_41:
load gp1, [bp + 8]
load gp2, [bp + 12]
add gp0, gp1, gp2
store gp0, [bp - 4]
.cnd_42:
;epilogue
mov tmp, gp0
mov sp, bp
pop bp
ret

;BuiltInFunc
print:
    push bp
    mov bp, sp
    load gp0, [bp + 8]    ; аргумент Ч указатель на строку
    movi gp2, 0           ; offset
    movi gp3, 1           ; шаг
    movi gp4, 0           ; '\0'
    movi gp7, 0x4000      ; граница между data и const
.loop_print:
    ; ќпредел€ем источник: const[] или data[]
    cmp gp0, gp7
    jg .from_data
.from_const:
    load8 gp1, const[gp0 + gp2]
    jmp .check_end
.from_data:
    load8 gp1, [gp0 + gp2]
.check_end:
    cmp gp1, gp4
    je .end_print
    out gp1
    add gp2, gp2, gp3
    jmp .loop_print
.end_print:
    mov sp, bp
    pop bp
    ret

;BuiltInFunc
itoa:
    push bp
    mov bp, sp

    load gp0, [bp + 8]    ; gp0 = число
    load gp1, [bp + 12]   ; gp1 = буфер

    movi gp2, 0           ; offset = 0
    movi gp3, 10          ; делитель
    movi gp4, 48          ; '0'
    movi gp5, 1           ; шаг +1
    movi gp6, 0           ; временный дл€ '\0'

.rev_loop:
    cmp gp0, gp6          ; cmp число, 0
    je .done_digits

    mod gp7, gp0, gp3     ; digit = число % 10
    div gp0, gp0, gp3     ; число /= 10

    add gp7, gp7, gp4     ; преобразуем в ASCII
    store8 gp7, [gp1 + gp2]
    add gp2, gp2, gp5     ; offset++

    jmp .rev_loop

.done_digits:
    store8 gp6, [gp1 + gp2] ; '\0'

    ; реверс строки: i = 0, j = offset - 1
    movi gp0, 0           ; i
    sub gp2, gp2, gp5     ; j = offset - 1 (перезаписываем gp2)

.reverse_itoa:
    cmp gp0, gp2
    jge .end_itoa

    ; swap buf[i] <-> buf[j]
    load8 gp3, [gp1 + gp0]
    load8 gp4, [gp1 + gp2]
    store8 gp4, [gp1 + gp0]
    store8 gp3, [gp1 + gp2]

    add gp0, gp0, gp5
    sub gp2, gp2, gp5
    jmp .reverse_itoa

.end_itoa:
    mov sp, bp
    pop bp
    ret

;BuiltInFunc
atoi:
    push bp
    mov bp, sp

    load gp1, [bp + 8]   ; указатель на строку
    movi gp0, 0          ; результат = 0
    movi gp2, 0          ; offset
    movi gp3, 10         ; множитель
    movi gp4, 0          ; символ '\0'
    movi gp5, 48         ; символ '0'
    movi gp6, 57         ; символ '9'

.loop_atoi:
    load8 gp7, [gp1 + gp2]     ; gp7 = str[offset]

    cmp gp7, gp4               ; если символ == '\0'
    je .end_atoi

    cmp gp7, gp5               ; сравнить с '0'
    jl .end_atoi               ; если gp7 < '0', выход

    cmp gp7, gp6               ; сравнить с '9'
    jg .end_atoi               ; если gp7 > '9', выход

    ; digit = ch - '0'
    sub gp7, gp7, gp5

    ; result = result * 10
    mul gp0, gp0, gp3

    ; result = result + digit
    add gp0, gp0, gp7

    movi gp4, 1
    add gp2, gp2, gp4
    movi gp4, 0
    jmp .loop_atoi

.end_atoi:
    mov tmp, gp0
    mov sp, bp
    pop bp
    ret

;BuiltInFunc
input:
    push bp
    mov bp, sp

    load gp0, [bp + 8]      ; gp0 = буфер
    movi gp2, 0             ; gp2 = offset
    movi gp3, 10            ; gp3 = '\n'
    movi gp4, 0             ; gp4 = '\0'
    movi gp5, 1             ; gp5 = шаг

.read_loop:
    in gp1                  ; gp1 = ввод символа
    cmp gp1, gp3            ; сравниваем с '\n'
    je .end_input
    store8 gp1, [gp0+gp2]
    add gp2, gp2, gp5
    jmp .read_loop

.end_input:
    store8 gp4, [gp0+gp2] ; добавл€ем '\0'
    mov sp, bp
    pop bp
    ret
