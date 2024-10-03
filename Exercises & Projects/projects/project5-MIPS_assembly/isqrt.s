
isqrt:
    #PROLOGUE
    subu    $sp,    $sp,    8               # grow stack by 8
    sw      $ra,    8($sp)                  # save return addr on stack
    sw      $fp,    4($sp)                  # save previous frame ptr on stack
    addu    $fp,    $sp,    8               # set frame ptr to start of new frame

    #BODY
    subu    $sp,    $sp,    8               # grow stack by 8 saving registers
    sw      $s0,    8($sp)                  # push $s0 in stack
    sw      $s1,    4($sp)                  # push $s1 in stack
    
    move    $s2,    $a0                     # save current func param $s2 = n
    subu    $sp,    $sp,    4               # grow stack by 4 to keep track of n
    sw      $s2,    4($sp)                  # push $s2 = n in stack

    bge     $s2,    2,      calc_isqrt      # if n >= 2, then continue calculation
    move    $v0,    $s2                     # return val = n
    j       end_isqrt                       # jump to end of method

calc_isqrt:
    srl     $a0,    $s2,    2               # store arg n = n >> 2 bitwise shift
    jal     isqrt                           # recursive call
    
    lw      $s2,    -16($fp)                # restore n for comparison
    sll     $s0,    $v0,    1               # small = return value << 1 bitwise shift
    addi    $s1,    $s0,    1               # large = small + 1

    mul     $t3,    $s1,    $s1             # $t3 = large * large to compare with n next
    ble     $t3,    $s2,    isqrt_large     # if large*large <= n, then go to else
    move    $v0,    $s0                     # return val = small
    j       end_isqrt                       # jump to end of method

isqrt_large: 
    move    $v0,    $s1                     # return val = large

end_isqrt:      
    lw      $s0,    -8($fp)                 # restore $s0 from stack
    lw      $s1,    -12($fp)                # restore $s1 from stack
    
    #EPILOGUE
    move    $sp,    $fp                     # restore stack
    lw      $ra,    0($sp)                  # restore saved return address
    lw      $fp,    -4($sp)                 # restore saved frame pointer
    jr      $ra                             # return to kernel/caller program
