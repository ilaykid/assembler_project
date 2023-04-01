MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
.extern label1 label32
jmp L1
mov r3,r6
mcr m1
sub r1, r4
bne END
endmcr
prn #-5
.entry HELLO
HELLO: add #1, r1
bne LOOP(r4,r3)
jmp L1(#41,r6)
m1
L1: inc K
bne LOOP(K,STR)
bne HELLO(r4,r6)
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
mov r2, label1
