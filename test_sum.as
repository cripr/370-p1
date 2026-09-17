        lw      0       1       count # load counter with 15
        lw      0       2       sum     # load sum with 0
        lw      0       3       one     # load constant 1
loop    beq     0       1       done    # if counter == 0, exit loop
        add     2       1       2       # sum = sum + counter
        lw      0       4       neg1    # load -1 for decrement
        add     1       4       1       # counter = counter - 1
        beq     0       0       loop    # jump back to loop
done    sw      0       2       result  # store final sum
        halt                            # stop program
count .fill   15                      # start with 15
sum     .fill   0                       # initialize sum to 0
one     .fill   1                       # constant 1
neg1    .fill   -1                      # constant -1 for decrementing
result  .fill   0                       # storage for result
