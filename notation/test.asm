	[section code]

load R0, 5
STORE R0, 0x00000001
load R1, 10
STORE R0, 0x00000004
add R2, R1, R0
STORE R0, 0x00000008
hlt

	[section data_bank]
