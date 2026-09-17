        lw      0       1       mcand   # load multiplicand (3)
        lw      0       2       mplier  # load multiplier (4)
        lw      0       7       neg1    # load -1 for decrementing
        add     0       0       4       # initialize product to 0
start   beq     0       2       done    # branch if multiplier is 0
        add     1       4       4       # add multiplicand to product
        add     2       7       2       # decrement multiplier
        beq     0       0       start   # repeat
done    halt                            # result in reg4
mcand   .fill   3
mplier  .fill   4
neg1    .fill   -1
