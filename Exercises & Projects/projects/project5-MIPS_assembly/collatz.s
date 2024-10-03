
collatz: 
    #PROLOGUE
    subu    $sp,    $sp,    8           # grow stack by 8
    sw      $ra,    8($sp)              # push return addr on stack
    sw      $fp,    4($sp)              # push previous frame ptr on stack
    addu    $fp,    $sp,    8           # set frame ptr to start of new frame 

    #BODY
    lw      $t0,    4($fp)              # $t0 = n from stack
    lw      $t1,    8($fp)              # $t1 = d from stack
    
    beq     $t0,    1,      end_case   # if n == 1, then go to end case to return d

    and     $t2,    $t0,    0x1         # check if n is even or odd ($s2 = n % 2)
    beq     $t2,    0,      alt_op      # if n is even, then do alternative operation
    
    mul     $t2,    $t0,    3           # first make arg0 = 3 * n
    add     $t2,    $t2,    1           # then make arg0 = (3 * n) + 1
    add     $t3,    $t1,    1           # make arg1 = d + 1

    subu    $sp,    $sp,    8           # grow stack by 8 to store new params 3n+1, d
    sw      $t3,    8($sp)              # push d+1 to stack first
    sw      $t2,    4($sp)              # push 3n+1 to stack next

    jal     collatz                     # recursive call with new args

    j       end_collatz                 # jump to end of function
    
alt_op: 
    div     $t2,    $t0,    2
    add     $t3,    $t1,    1           # make arg1 = d + 1

    subu    $sp,    $sp,    8           # grow stack by 8 to store new params 3n+1, d 
    sw      $t3,    8($sp)              # push d+1 to stack first
    sw      $t2,    4($sp)              # push n/2 to stack next

    jal     collatz                     # recursive call with new args
    
    j   end_collatz                     # jump to end of function

end_case:
    move    $v0,    $t1                 # set return val = d

end_collatz:
    #EPILOGUE
    move    $sp,    $fp                 # restore stack
    lw      $ra,    0($fp)              # restore saved return address
    lw      $fp,    -4($sp)             # restore saved frame pointer
    jr      $ra                         # return to kernel/caller program