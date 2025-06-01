; ¬ход: [bp+8] Ч указатель на строку
; ¬ыход: gp0 Ч результат

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



; ¬ход:
;   [bp+8]  Ч int значение
;   [bp+12] Ч указатель на буфер (в data[])
; ¬ыход:
;   строка с '\0' в буфере

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

