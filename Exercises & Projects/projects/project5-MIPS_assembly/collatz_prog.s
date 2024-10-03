.data

# array terminated by 0 (which is not part of the array)
xarr:
.word 2, 4, 6, 8, 10, 0
.data

arrow: .asciiz " -> "

.text

main:
    li      $sp,        0x7ffffffc      # initialize $sp

# PROLOGUE
    subu    $sp,        $sp,        8   # expand stack by 8 bytes
    sw      $ra,        8($sp)          # push $ra (ret addr, 4 bytes)
    sw      $fp,        4($sp)          # push $fp (4 bytes)
    addu    $fp,        $sp,        8   # set $fp to saved $ra

    subu    $sp,        $sp,        12  # save s0 and s1 on stack before using them
    sw      $s0,        12($sp)         # push $s0
    sw      $s1,        8($sp)          # push $s1
    sw      $s2,        4($sp)          # push $s2

    la      $s0,        xarr            # load address to s0

main_for:
    lw      $s1,        ($s0)           # use s1 for xarr[i] value
    li      $s2,        0               # use s2 for initial depth (steps)
    beqz    $s1,        main_end        # if xarr[i] == 0, stop.

# save args on stack rightmost one first
    subu    $sp,        $sp,        8   # save args on stack
    sw      $s2,        8($sp)          # save depth
    sw      $s1,        4($sp)          # save xarr[i]

    li      $v0,        1
    move    $a0,        $s1             # print_int(xarr[i])
    syscall 

    li      $v0,        4               # print " -> "
    la      $a0,        arrow
    syscall 

    jal     collatz                     # result = collatz(xarr[i])

    move    $a0,        $v0             # print_int(result)
    li      $v0,        1
    syscall 

    li      $a0,        10              # print_char('\n')
    li      $v0,        11
    syscall 

    addu    $s0,        $s0,        4   # make s0 point to the next element

    lw      $s2,        8($sp)          # save depth
    lw      $s1,        4($sp)          # save xarr[i]
    addu    $sp,        $sp,        8   # save args on stack
    j       main_for

main_end:
    lw      $s0,        12($sp)         # push $s0
    lw      $s1,        8($sp)          # push $s1
    lw      $s2,        4($sp)          # push $s2

# EPILOGUE
    move    $sp,        $fp             # restore $sp
    lw      $ra,        ($fp)           # restore saved $ra
    lw      $fp,        -4($sp)         # restore saved $fp
    jr      $ra                         # return to kernel

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