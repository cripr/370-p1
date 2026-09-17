        lw      0       1       one     # load constant 1
        lw      0       2       zero    # initialize sum to 0
        lw      0       3       count   # load array count (3 elements)
        lw      0       7       neg1    # load -1 for decrementing
load    lw     0       4       array # SELF-MODIFYING: load from array
        add     2       4       2       # add array element to sum
        add     3       7       3       # decrement counter
        beq     0       3       done    # if counter == 0, we're done
        lw      0       4       load    # load the instruction itself
        add     4       1       4       # increment the address field by 1
        sw      0       4       load    # store modified instruction back
        beq     0       0       load    # jump back to modified load
done    halt                            # sum of array in reg2
one     .fill   1
zero    .fill   0
count   .fill   3
neg1    .fill   -1
array .fill 10                     # array[0] = 10
        .fill   20                     # array[1] = 20  
        .fill   30                     # array[2] = 30
