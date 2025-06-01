; ¬ход: [bp+8] Ч указатель на строку
; ¬ыход: gp0 Ч результат

atoi:
    push bp
    mov bp, sp

    load gp1, [bp + 8]   ; указатель на строку
    movi gp0, 0          ; результат = 0
    movi gp2, 0          ; offset
    movi gp3, 10         ; множитель
    movi gp4, 0          ; '\0'
    movi gp5, '0'        ; символ '0'

.loop:
    add gp6, gp1, gp2
    load8 gp7, [gp6]     ; gp7 = str[offset]
    cmp gp7, gp4
    je .end

    ; digit = ch - '0'
    sub gp7, gp7, gp5

    ; result = result * 10
    mul gp0, gp0, gp3

    ; result = result + digit
    add gp0, gp0, gp7

    add gp2, gp2, 1
    jmp .loop

.end:
    mov tmp, gp0
    mov sp, bp
    pop bp
    ret



; ¬ход:
;   [bp+8]  Ч int значение
;   [bp+12] Ч указатель на буфер (в data[]!)
; ¬ыход: строка с '\0' в конце

itoa:
    push bp
    mov bp, sp

    load gp0, [bp + 8]   ; число
    load gp1, [bp + 12]  ; указатель на буфер

    movi gp2, 0          ; offset
    movi gp3, 10         ; делитель
    movi gp4, '0'        ; символ '0'

.reverse_loop:
    cmp gp0, 0
    je .done_digits

    mod gp5, gp0, gp3      ; цифра
    div gp0, gp0, gp3      ; число /= 10

    add gp5, gp5, gp4      ; преобразуем в ASCII
    store8 gp5, [gp1 + gp2]
    add gp2, gp2, 1

    jmp .reverse_loop

.done_digits:
    ; конец строки
    movi gp6, 0
    store8 gp6, [gp1 + gp2]

    ; теперь перевернЄм строку (в gp1, длина Ч gp2)
    movi gp7, 0          ; i = 0
    sub gp8, gp2, 1      ; j = len - 1

.reverse:
    cmp gp7, gp8
    jge .end

    ; swap buf[i] <-> buf[j]
    load8 gp9, [gp1 + gp7]
    load8 gp10, [gp1 + gp8]
    store8 gp10, [gp1 + gp7]
    store8 gp9, [gp1 + gp8]

    add gp7, gp7, 1
    sub gp8, gp8, 1
    jmp .reverse

.end:
    mov sp, bp
    pop bp
    ret
