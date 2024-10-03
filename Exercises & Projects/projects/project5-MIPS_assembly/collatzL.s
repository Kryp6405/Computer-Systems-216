collatz:
    # Prologue
    addiu   $sp, $sp, -12  # Allocate stack space
    sw      $ra, 8($sp)    # Save return address
    sw      $fp, 4($sp)    # Save frame pointer
    move    $fp, $sp       # Update frame pointer

    # Load parameters
    move    $t0, $a0       # Move n into $t0
    move    $t1, $a1       # Move d into $t1

    # Check if n is not 1
    li      $t2, 1
    bne     $t0, $t2, check_odd  # If n != 1, go to check_odd
    move    $v0, $t1             # If n == 1, return d
    j       end_function

check_odd:
    # Check if n is odd
    andi    $t3, $t0, 1
    beq     $t3, $zero, even     # If n is even, go to even

    # Odd: collatz(3 * n + 1, d + 1)
    addiu   $t1, $t1, 1          # Increment d
    li      $t4, 3
    mul     $t0, $t0, $t4
    addiu   $t0, $t0, 1
    j       recursive_call

even:
    # Even: collatz(n / 2, d + 1)
    addiu   $t1, $t1, 1          # Increment d
    sra     $t0, $t0, 1          # Divide n by 2

recursive_call:
    move    $a0, $t0             # Set first argument (n)
    move    $a1, $t1             # Set second argument (d)
    jal     collatz              # Recursive call
    j       end_function

end_function:
    # Epilogue
    move    $sp, $fp       # Restore stack pointer
    lw      $fp, 4($sp)    # Restore frame pointer
    lw      $ra, 8($sp)    # Restore return address
    addiu   $sp, $sp, 12   # Deallocate stack space
    jr      $ra            # Return from function
