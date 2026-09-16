        lw      0       1       -2      load with a negative NUMERIC offset (not a label)
start   add     0       0       0       first instruction in the loop
        add     0       0       0       filler
        add     0       0       0       filler
        beq     0       0       start   backward branch spanning multiple instructions
        halt                            end of program
self 	.fill	self                 label whose value points to itself
