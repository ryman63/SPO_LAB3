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