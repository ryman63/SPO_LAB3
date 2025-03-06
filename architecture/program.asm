[section code]
main:
load gp0, 5
mov gp1, gp0
load gp0, 10
mov gp2, gp0
load gp0, 15
mov gp3, gp0
load gp5, 1
add gp0, gp1, gp5
pushr gp0
pushr gp2
call sum
pop gp7
subi sp, sp, 4
movi gp7, 4
pushr gp4
hlt
sum:
pop gp0
pop gp1
add gp2, gp0, gp1
mov gp5, gp2
pushr gp5
ret
