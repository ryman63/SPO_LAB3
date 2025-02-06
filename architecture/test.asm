	[section code]
main:
	load gp0, 50
	load gp1, 100
	add gp2, gp1, gp0
	out gp2
	hlt
	dw 0x1234