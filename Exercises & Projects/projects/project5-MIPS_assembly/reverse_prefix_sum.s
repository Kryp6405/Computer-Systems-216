
reverse_prefix_sum:
    #PROLOGUE
    subu    $sp,    $sp,    8               # grow stack by 8
    sw      $ra,    8($sp)                  # save return addr on stack
    sw      $fp,    4($sp)                  # save previous frame ptr on stack
    addu    $fp,    $sp,    8               # set frame ptr to start of new frame

    #BODY
    subu    $sp,    $sp,    4               # grow stack by 4
    sw      $s0,    4($sp)                  # push $s0 in stack
    
    move    $t0,    $a0                     # store $t0 = arr
    lw      $t1,    0($t0)                  # store first elem of arr
    bne     $t1,    0xffffffff, calc_rps    # if *arr != -1, then continue with calcualtion
    li      $v0,    0                       # default return val = 0
    j       end_rps                         # jump to end of method

calc_rps:
    subu    $sp,    $sp,    4               # grow stack by 4 to store curr arr
    sw      $t0,    4($sp)                  # push curr arr to stack

    addiu   $t0,    $t0,    4               # save arg0 = arr + 1 (i.e. next elemtn in arr)
    move    $a0,    $t0                     # store arr+1 in arg for recursvie call
    
    jal     reverse_prefix_sum              # recursive call
    
    lw      $t0,    4($sp)                  # load back original arr from stack
    lw      $t1,    0($t0)                  # get first element in arr
    addu    $s0,    $v0,    $t1             # add together r = return val + fist elem in arr
    sw      $s0,    0($t0)                  # store r to current arr location (*arr = r)
    move    $v0,    $s0                     # set return val = r

end_rps:
    lw      $s0,    -8($fp)                 # restore $s0 from stack

    #EPILOGUE
    move    $sp,    $fp                     # restore stack
    lw      $ra,    0($sp)                  # restore saved return address
    lw      $fp,    -4($sp)                 # restore saved frame pointer
    jr      $ra                             # return to kernel/caller program