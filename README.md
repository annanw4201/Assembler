# Assembler for MIPS

* This is an assembler that can translate the MIPS language into a machine code.

# ToUse
- i is an immediate value (two's complement), $s, $t, $d are registers, when type in instructions below, assembler will translate them into machine code
* .word i
* add $d, $s, $t 
* sub $d, $s, $t 
* mult $s, $t 
* multu $s, $t 
* div $s, $t 
* divu $s, $t 
* mfhi $d
* mflo $d
* lis $d
* lw $t, i($s) 
* sw $t, i($s) 
* slt $d, $s, $t 
* sltu $d, $s, $t 
* beq $s, $t, i 
* bne $s, $t, i 
* jr $s
* jalr $s
