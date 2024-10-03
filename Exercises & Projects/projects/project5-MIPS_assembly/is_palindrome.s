
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