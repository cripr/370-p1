        add     1       2       3       # reg3 = reg1 + reg2
        nor     3       3       4       # reg4 = ~(reg3 | reg3) = ~reg3
        sw      0       4       result  # store reg4 to result
        lw      0       5       result  # load result into reg5
        halt
result  .fill   0
