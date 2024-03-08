0300:  50 05
0302:  51 05
0304:  52 00
0306:  53 05
0308:  79
0309:  63
030A:  58
030B:  71 12 03
030E:  70 08 03
FFFF

Store [5,4,3,2,1] à partir de l'adresse 0005 (jusqu'à 0009 compris);

_start:
   mv $4, %R0
   mv $0, %R2
   mv $5, %R3

loop:
   st %R0, %RX1
   inc %R2
   dec %R0
   jz done
   jmp loop

done: