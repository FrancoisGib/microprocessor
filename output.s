ld R2, (0001)
ld R3, (0101)
ld R4, (0201)
ld R5, (0301)
ld R0, RX1
st R0, RX2
mv R1, $0
sub R0, R1
jz (2702)
inc R2 
jc (1F02)
inc R4 
jc (2302)
jmp (0C02)
inc R3 
jmp (0C02)
inc R5 
jmp (0C02)
