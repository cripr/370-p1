        lw      0       1       fib1    # reg1 = 1 (F(1))
        lw      0       2       fib2    # reg2 = 1 (F(2))  
        lw      0       3       count   # reg3 = 3 (counter)
        lw      0       7       neg1 reg7 = -1 (for decrementing)
loop    beq     0       3       done    # if counter == 0, done
        add     1       2       4       # reg4 = F(i-1) + F(i) = next Fibonacci number
        add     0       2       1       # reg1 = F(i) (move current to previous)
        add     0       4       2       # reg2 = F(i+1) (move next to current)
        add     3       7       3       # decrement counter
        beq     0       0       loop    # repeat loop
done    halt                            # result is in reg2
fib1    .fill   1
fib2    .fill   1
count   .fill   3
neg1 .fill  -1
