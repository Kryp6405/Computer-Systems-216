   .data
# uint arrays, each terminated by -1 (which is not part of array)
arr0:
   .word 1, 2, 3, 4, -1
arr1:
   .word 2, 3, 4, 5, -1
arr2:
   .word 5, 4, 3, 2,  -1
arr3:
   .word 1, 2, 1, 2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 4, -1
overflow:
   .word 1, 1, 1, 1, 2147483646, -1

   .text

# main(): ##################################################
#   process_array(arr0)
#   process_array(arr1)
#   process_array(arr2)
#   process_array(arr3)
#   process_array(arr4)
#
main:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   la   $a0, arr0
   jal  process_array
   la   $a0, arr1
   jal  process_array
   la   $a0, arr2
   jal  process_array
   la   $a0, arr3
   jal  process_array
   la   $a0, overflow
   jal  process_array

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
## end main ################################################

# process_array(uint* arr): #################################
#   print_array(arr)
#   reverse_prefix_sum(arr)
#   print_array(arr)
#
process_array:
   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 4        # save s0 on stack before using it
   sw   $s0, 4($sp)

   move $s0, $a0           # use s0 to save a0
   jal  print_array
   move $a0, $s0
   jal  reverse_prefix_sum
   move $a0, $s0
   jal  print_array

   lw   $s0, -8($fp)       # restore s0 from stack

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
## end process_array #######################################

# print_array(uint arr): ########################################
#   uint x
#   while (-1 != (x = *arr++)):
#     printf("%d ", x)
#   printf("\n")
#
print_array:
   # use t0 to hold arr. use t1 to hold *arr
   move $t0, $a0
print_array_while:
   lw   $t1, ($t0)
   beq  $t1, -1, print_array_endwhile
   move $a0, $t1           # print_int(*arr)
   li   $v0, 1
   syscall
   li   $a0, 32            # print_char(' ')
   li   $v0, 11
   syscall
   addu $t0, $t0, 4
   b    print_array_while
print_array_endwhile:
   li   $a0, 10            # print_char('\n')
   li   $v0, 11
   syscall
   jr   $ra
## end print_array #########################################

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