; ����: [bp+8] � ��������� �� ������
; �����: gp0 � ���������

atoi:
    push bp
    mov bp, sp

    load gp1, [bp + 8]   ; ��������� �� ������
    movi gp0, 0          ; ��������� = 0
    movi gp2, 0          ; offset
    movi gp3, 10         ; ���������
    movi gp4, 0          ; ������ '\0'
    movi gp5, 48         ; ������ '0'
    movi gp6, 57         ; ������ '9'

.loop_atoi:
    load8 gp7, [gp1 + gp2]     ; gp7 = str[offset]

    cmp gp7, gp4               ; ���� ������ == '\0'
    je .end_atoi

    cmp gp7, gp5               ; �������� � '0'
    jl .end_atoi               ; ���� gp7 < '0', �����

    cmp gp7, gp6               ; �������� � '9'
    jg .end_atoi               ; ���� gp7 > '9', �����

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



; ����:
;   [bp+8]  � int ��������
;   [bp+12] � ��������� �� ����� (� data[])
; �����:
;   ������ � '\0' � ������

itoa:
    push bp
    mov bp, sp

    load gp0, [bp + 8]    ; gp0 = �����
    load gp1, [bp + 12]   ; gp1 = �����

    movi gp2, 0           ; offset = 0
    movi gp3, 10          ; ��������
    movi gp4, 48          ; '0'
    movi gp5, 1           ; ��� +1
    movi gp6, 0           ; ��������� ��� '\0'

.rev_loop:
    cmp gp0, gp6          ; cmp �����, 0
    je .done_digits

    mod gp7, gp0, gp3     ; digit = ����� % 10
    div gp0, gp0, gp3     ; ����� /= 10

    add gp7, gp7, gp4     ; ����������� � ASCII
    store8 gp7, [gp1 + gp2]
    add gp2, gp2, gp5     ; offset++

    jmp .rev_loop

.done_digits:
    store8 gp6, [gp1 + gp2] ; '\0'

    ; ������ ������: i = 0, j = offset - 1
    movi gp0, 0           ; i
    sub gp2, gp2, gp5     ; j = offset - 1 (�������������� gp2)

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

