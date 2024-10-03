   .data
str1:
   .asciiz "abba"
str2:
   .asciiz "racecar"
str3:
   .asciiz "swap paws",
str4:
   .asciiz "not a palindrome"
str5:
   .asciiz "another non palindrome"
str6:
   .asciiz "almost but tsomla"
str7: 
   .asciiz " "
str8: 
   .asciiz "+&@!#@&+"
str9:
   .asciiz "123421"

# array of char pointers = {&str1, &str2, ..., &str6}
ptr_arr:
   .word str1, str2, str3, str4, str5, str6, str7, str8, str9, 0

yes_str:
   .asciiz " --> Y\n"
no_str:
   .asciiz " --> N\n"

   .text

# main(): ##################################################
#   char ** j = ptr_arr
#   while (*j != 0):
#     rval = is_palindrome(*j)
#     printf("%s --> %c\n", *j, rval ? yes_str: no_str)
#     j++
#
main:
   li   $sp, 0x7ffffffc    # initialize $sp

   # PROLOGUE
   subu $sp, $sp, 8        # expand stack by 8 bytes
   sw   $ra, 8($sp)        # push $ra (ret addr, 4 bytes)
   sw   $fp, 4($sp)        # push $fp (4 bytes)
   addu $fp, $sp, 8        # set $fp to saved $ra

   subu $sp, $sp, 8        # save s0, s1 on stack before using them
   sw   $s0, 8($sp)        # push $s0
   sw   $s1, 4($sp)        # push $s1

   la   $s0, ptr_arr        # use s0 for j. init ptr_arr
main_while:
   lw   $s1, ($s0)         # use s1 for *j
   beqz $s1, main_end      # while (*j != 0):
   move $a0, $s1           #    print_str(*j)
   li   $v0, 4
   syscall
   move $a0, $s1           #    v0 = is_palindrome(*j)
   jal  is_palindrome
   beqz $v0, main_print_no #    if v0 != 0:
   la   $a0, yes_str       #       print_str(yes_str)
   b    main_print_resp
main_print_no:             #    else:
   la   $a0, no_str        #       print_str(no_str)
main_print_resp:
   li   $v0, 4
   syscall

   addu $s0, $s0, 4       #     j++
   b    main_while        # end while
main_end:

   # EPILOGUE
   move $sp, $fp           # restore $sp
   lw   $ra, ($fp)         # restore saved $ra
   lw   $fp, -4($sp)       # restore saved $fp
   j    $ra                # return to kernel
# end main ################################################

is_palindrome: 
   # PROLOGUE 
   subu     $sp,     $sp, 8               # grow stack by 8 bytes for ra and fp 
   sw       $ra,     8($sp)               # maintain return address in bottom of stack 
   sw       $fp,     4($sp)               # maintain frame pointer next in stack 
   addu     $fp,     $sp, 8               # move fp to caller location in program aka ra 
    
   # BODY    
   move     $t0,     $a0                  # $t0 = pointer to the string 
   move     $t1,     $a0                  # $t1 = pointer to the string 
   li       $t3,     0                    # $t3 = 0 for strlen counter 
 
strlen: 
   lb       $t2,     0($t1)               # load a character from the string 
   beqz     $t2,     end_strlen           # if the character is null, exit the loop 
   addi     $t1,     $t1,     1           # move to the next character 
   addu     $t3,     $t3,     1           # increment strlen counter 
   j        strlen 
 
end_strlen: 
   addi     $t1,     $t1,     -1          # make pointer skip null terminator 
   li       $t2,     0                    # reuse $t2 = i = 0 (loop counter) 
   divu     $t4,     $t3,     2           # divide strlen by 2 for looping condition 

palin_loop:  
   bge      $t2,     $t4,     true_palin  # if index >= len/2, execute end_loop 

   lb       $t5,     ($t0)                # load first char in string 
   lb       $t6,     ($t1)                # load last char in string 

   bne      $t5,     $t6,     false_palin # if first and last char diff, then execute not_palin 

   addi     $t0,     $t0,     1           # move to the next char from the front 
   addi     $t1,     $t1,     -1          # move to the next char from the end 
   addu     $t2,     $t2,     1           # increment the loop counter 
   j        palin_loop                    # reiterate loop by jumping to palin_loop 

true_palin: 
   li       $v0,     1                    # return val =  1 for finding palindrome 
   j        end_is_palindrome             # jump to end of method -> fix stack -> epilogue 
	 
false_palin: 
   li       $v0,     0                    # return val = 0 for not finding palindrome 
 
end_is_palindrome: 
   # EPILOGUE 
   move     $sp,     $fp                  # restore $sp from stack 
   lw       $ra,     0($fp)               # restore $ra in frame from stack 
   lw       $fp,     -4($sp)              # restore $fp in fram from stack 
   jr       $ra                           # return to kernel/program 