# <Parker Hendry>
# Recursive Russian Peasant Multiplication

.text
# driver program: input two numbers, call malar, and output the product
main:	li	$v0, 4
	la	$a0, firstPrompt
	syscall
	li	$v0, 5
	syscall
	move	$t0, $v0
	li	$v0, 4
	la	$a0, secondPrompt
	syscall
	li	$v0, 5
	syscall
	move	$t1, $v0
	move	$a0, $t0		# set up two args (nums to be multiplied)
	move	$a1, $t1
	jal	malar			# call mult a la russe
	move	$t0, $v0		# save product in $t0
	li	$v0, 4
	la	$a0, productPrompt
	syscall				# print "The product is: "
	move	$a0, $t0
	li	$v0, 1
	syscall				# print product
	li	$v0, 10
	syscall				# exit
	
.data 
firstPrompt:	.asciiz "Enter first number: "
secondPrompt:	.asciiz "Enter second number: "
productPrompt:	.asciiz "The product is: "

.text
# recursive multiplication a la russe (Russian Peasant Multiplication)
malar:	beq $a0, $0, foo		# if n = 0 then foo	
	addi $t0, $0, 2			# set temp reg to 2
	div $a0, $t0			# n / 2	
	mfhi $t0			# n % 2
	bne $t0, $0, bar		# if n is odd go to bar
	srl $a0, $a0, 1  		# n /= 2
	sll $a1, $a1, 1			# m *= 2
	addi $sp, $sp, -4		# allocate space on stack for $ra
	sw $ra, 0($sp)			# store $ra on stack
	jal malar			# recursively call malar
	j foo				# go to foo
	bar:				# IF N IS ODD
	add $t2, $t2, $a1		# answer = answer + m				
	srl $a0, $a0, 1  		# n /= 2
	sll $a1, $a1, 1			# m *= 2
	addi $sp, $sp, -4		# allocate space on stack for $ra
	sw $ra, 0($sp)			# store $ra on stack
	jal malar			# recursively call malar
	foo:
	move $v0, $t2			# move answer into v0 to return
	addi $t0, $0, 0x7fffeffc	# set temp to default stack location
	slt $t1, $sp, $t0		# if stack < default location 
	bne $t1, $0, baz		# if stack < default location then go to baz
	jr	$ra			# return
	baz:
	lw $ra, 0($sp)			# get return address from stack
	addi $sp, $sp, 4		# deallocate space from stack
	jr $ra				# return
	



