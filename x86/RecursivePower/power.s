	.global power		# power(number, power)

	.text
power:
	cmp 	$0, %rsi	# If power != 0, decrement and make recursive call
	jne 	foo		
	mov 	$1, %rax	# If power == 0, return 1
	ret	
foo:	
	dec 	%rsi		
	call 	power		
	imul 	%rdi, %rax	# Multiply number by return value and return
	ret
