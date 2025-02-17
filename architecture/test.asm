	[section code]
main:
	load gp0, 50
	load gp1, 100
	pushr gp1 ; положил
	pushr gp0 ; положил
	call sum ; положил +3
	out gp2
	hlt

sum:
	pop gp0 ; забрал
	pop gp1 ; забрал + 1
	add gp2, gp1, gp0
	pushr rag
	call sum_2 ; положил + 2
	pop rag
	ret ; забрал 0

sum_2:
	add gp2, gp2, gp1
	pushr rag
	call sum_3
	pop rag
	ret ; забрал + 1


sum_3:
	add gp2, gp2, gp1
	ret