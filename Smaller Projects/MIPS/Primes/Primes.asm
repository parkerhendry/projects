
# Task 4: Array and function practice
# Parker Hendry
# CSCI 311

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

#move user input into $t0
move $t0, $v0

#create value for i
addi $s0, $0, 1 

#create base address
addi $s2, $0, 0x10010040

#create value for offset
addi $s3, $0, -4

#create counter to hold number of primes
addi $s4, $0, 0

foo:				#FIRST FOR LOOP
	addi $s0, $s0, 1	# i++
	slt $t1, $s0, $t0	# if i < n
	beq $t1, $0, load	# if i = n then load
	addi $s5, $0, 0		# p = 0
	addi $s1, $0, 2		# j = 2
	slt $t2, $s1, $s0	# if j < i
	bne $t2, $0, bar	# if j < i then bar		

bar:				#NESTED FOR LOOP
	slt $s7, $s1, $s0	# if j < i	
	beq $s7, $0, waldo	# if j = i then go to waldo
	div $s0, $s1		# i / j
	mfhi $t3		# i % j
	addi $s1, $s1, 1	# j++
	beq $t3, $0, baz	# if i % j = 0 go to baz
	j bar			# go back to bar

baz:
	addi $s5, $0, 1		# p = 1
	j bar			# go back to bar
	
waldo:
	beq $s5, $0, store	# if p = 0 then store
	j foo			# go back to foo	
	

store:
	addi $s3, $s3, 4	#calculate offset
	add $s2, $s2, $s3	#add offset to address
	move $a1, $s2		#create argument array_base
	move $a2, $s0		#create argument for max_value
	jal prime_array		#call prime_array with arguments array_base and max_value
	move $t7, $v0		#add number of primes to num_vals variable
	j foo			#go back to foo

#$a1 = array_base
#$a2 = max_value	
prime_array:
	addi $s4, $s4, 1	#increment number of primes
	sw $a2, 0($a1)		#store max_value at array_base
	move $v0, $s4		#move number of primes into v0 to return
	jr $ra			#return number of primes

load:
	addi $a1, $0, 0x10010040	#set array_base argument to starting value
	move $a3, $t7			#set num_vals argument to num_vals variable
	addi $s6, $0, -1		#starting index
	addi $t6, $0, -4 		#set value for offset
	jal array_print			#call array_print with arguments array_base and num_vals

			
#$a1 = array_base
#$a3 = num_vals			
array_print:
	addi $s6, $s6, 1	#starting index
	slt $t4, $s6, $a3	#if current position < num_vals
	beq $t4, $0, done	#if at number of primes, go to done
	addi $t6, $t6, 4	#calculate offset
	add $a1, $a1, $t6	#put calculated array_base into $a1
	lw $t5, 0($a1)		#load at calulated array_base
	li $v0, 1		#print integer syscall
	move $a0, $s6		#move index into a0
	syscall			#print index
	li $v0, 4		#print string syscall
	la $a0, prompt2		#load address of prompt 2
	syscall			#print colon and space
	li $v0, 1		#print integer syscall
	move $a0, $t5		#move prime number into a0
	syscall			#print prime number
	li $v0, 4		#print string syscall
	la $a0, prompt3		#move address of prompt3 into a0
	syscall			#print newline
	j array_print		#go back to array_print
	
done:
	li $v0, 10		#end program
	syscall
	
	

.data
prompt1:
	.asciiz "Please enter a positive number: "	#string for user
prompt2:
	.asciiz ": "					#colon and space characters for answer
prompt3:
	.asciiz "\n"					#newline	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
