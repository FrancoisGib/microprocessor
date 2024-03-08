0200:  52 00
0202:  53 05
0204:  54 01
0206:  55 00
0208:  7D
0209:  7A
020A:  63
020B:  72 19 02
020E:  65
020F:  72 1D 02
0212:  59
0213:  71 21 02
0216:  70 08 02
0219:  62
021A:  70 12 02
021D:  64
021E:  70 12 02
FFFF

_start:
   mv $0, %R2
   mv $5, %R3
   mv $1, %R4
   mv $0, %R5

loop: (0208)
   load %R0, %RX1
   store %R0, %RX2
   inc %R3
   jc hold_RX1
   inc %R5
   jc hold_RX2

cond: (0212)
   dec %R1
   jz done
   jmp loop

hold_RX1: (0219)
   inc %R2
   jmp cond

hold_RX2: (021D)
   inc %R4
   jmp cond

done: (0221)