; ����: [bp+8] � ��������� �� ������
; �����: gp0 � ���������

atoi:
    push bp
    mov bp, sp

    load gp1, [bp + 8]   ; ��������� �� ������
    movi gp0, 0          ; ��������� = 0
    movi gp2, 0          ; offset
    movi gp3, 10         ; ���������
    movi gp4, 0          ; '\0'
    movi gp5, '0'        ; ������ '0'

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



; ����:
;   [bp+8]  � int ��������
;   [bp+12] � ��������� �� ����� (� data[]!)
; �����: ������ � '\0' � �����

itoa:
    push bp
    mov bp, sp

    load gp0, [bp + 8]   ; �����
    load gp1, [bp + 12]  ; ��������� �� �����

    movi gp2, 0          ; offset
    movi gp3, 10         ; ��������
    movi gp4, '0'        ; ������ '0'

.reverse_loop:
    cmp gp0, 0
    je .done_digits

    mod gp5, gp0, gp3      ; �����
    div gp0, gp0, gp3      ; ����� /= 10

    add gp5, gp5, gp4      ; ����������� � ASCII
    store8 gp5, [gp1 + gp2]
    add gp2, gp2, 1

    jmp .reverse_loop

.done_digits:
    ; ����� ������
    movi gp6, 0
    store8 gp6, [gp1 + gp2]

    ; ������ ��������� ������ (� gp1, ����� � gp2)
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
