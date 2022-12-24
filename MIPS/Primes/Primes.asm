#Parker Hendry
#Prime Numbers

.text
.globl main

main:

	#Print a prompt for the user
	li $v0, 4
	la $a0, prompt1
	syscall

	#Get user input from the keyboard
	li $v0, 5
	syscall
	
	#create argument for max_value
	move $a0, $v0
	
	#move argument to stack
	sw $a0, 0($sp)
	
	#allocate memory for 100 integers	
	li $v0, 9
	li $a0, 100
	syscall
	
	#take argument off of stack
	lw $a0, 0($sp)
	
	#create argument for array_base
	move $a1, $v0 
	
	#put base address into s2 to hold
	move $s2, $v0
	
	#call prime_array function
	jal prime_array
	
	#create argument for num_vals from return value from prime_array function
	move $a0, $v0
	
	#create argument for array_base
	move $a1, $s2
	
	#call array_print function
	jal array_print
	
	#finish program
	li $v0, 10
	syscall
	
	
prime_array:
	addi $s0, $0, -4	#create value for offset
	foo:			#FIRST FOR LOOP
	addi $t0, $t0, 1	# i++
	slt $t1, $t0, $a0	# if i < n
	beq $t1, $0, fin	# if i = n then fin
	addi $t2, $0, 0		# p = 0
	addi $t3, $0, 2		# j = 2
	slt $t4, $t3, $t0	# if j < i
	bne $t4, $0, bar	# if j < i then bar		
	bar:			#NESTED FOR LOOP
	slt $t5, $t3, $t0	# if j < i	
	beq $t5, $0, waldo	# if j = i then go to waldo
	div $t0, $t3		# i / j
	mfhi $t6		# i % j
	addi $t3, $t3, 1	# j++
	beq $t6, $0, baz	# if i % j = 0 go to baz
	beq $0, $0, bar		# go back to bar
	baz:
	addi $t2, $0, 1		# p = 1
	beq $0, $0, bar		# go back to bar	
	waldo:
	beq $t2, $0, store	# if p = 0 then store
	beq $0, $0, foo		# go back to foo	
	store:
	addi $s0, $s0, 4	#calculate offset
	add $a1, $a1, $s0	#add offset to address
	sw $t0, 0($a1)		#store prime number at address
	addi $s1, $s1, 1	#increment number of primes
	move $v0, $s1		#move number of primes into v0 to return
	beq $0, $0, foo   	#go back to foo
	fin:
	jr $ra			#return
	

array_print:
	addi $t0, $0, -1	#starting index
	addi $s0, $0, -4 	#set value for offset		
	fred:
	addi $t0, $t0, 1	#increment starting index
	slt $t1, $t0, $a0	#if current position < num_vals
	beq $t1, $0, done	#if at number of primes, go to done
	addi $s0, $s0, 4	#calculate offset
	add $a1, $a1, $s0	#add offset to address
	lw $t2, 0($a1)		#load at calulated array_base
	li $v0, 1		#print integer syscall
	addi $sp, $sp, -4	#allocate space on stack
	sw $a0, 0($sp)		#move num_vals onto stack
	move $a0, $t0		#move index into a0
	syscall			#print index
	li $v0, 4		#print string syscall
	la $a0, prompt2		#load address of prompt 2
	syscall			#print colon and space
	li $v0, 1		#print integer syscall
	move $a0, $t2		#move prime number into a0
	syscall			#print prime number
	li $v0, 4		#print string syscall
	la $a0, prompt3		#move address of prompt3 into a0
	syscall			#print newline
	lw $a0, 0($sp)		#load num_vals from stack
	addi $sp, $sp, 4	#deallocate stack space
	beq $0, $0, fred	#go back to previous line
	done:
	jr $ra			#return





.data
prompt1:
	.asciiz "Please enter a positive number: "	#string for user
prompt2:
	.asciiz ": "					#colon and space characters for answer
prompt3:
	.asciiz "\n"					#newline
