#------------------------------------------------------------------------------------------------------------
# Parker Hendry
#------------------------------------------------------------------------------------------------------------
# This program is an implementation of Euclid's algorithm, a decrease and conquer algorithm
# for finding the greatest common divisor of two positive integers. x86 assemlby (GNU assembler format) 
# was used along with the C functions printf and scanf for I/O. GCC was used for assembling and linking.
#------------------------------------------------------------------------------------------------------------

        .global  main
        
        .text
gcd:
        cmp    $0, %rsi		        #if b == 0 , do not go to foo
        mov    %rdi, %rax		#Move a to return (or dividend)
        jne     foo			#if b != 0, go to foo
        ret
foo:
        cltd				#Sign extend rax
        idiv   %rsi		        #a / b
        mov    %rsi, %rdi		#Move b to arg1
        mov    %rdx, %rsi		#Move remainder to arg2 
        call    gcd			#Recursively call gcd
        ret				#Return a
        
main:
        push   %rbp			#Create stack frame
        mov    %rsp, %rbp		#
        sub    $16, %rsp		#Allocate space on stack (aligned boundary)
        mov    $num1, %rdi		#Move num1 prompt to arg1
        call    printf			#Call printf
        lea    -8(%rbp), %rax		#Load address of a into return
        mov    %rax, %rsi		#Move address of a into arg2
        mov    $input, %rdi		#Move input prompt to arg1
        call    scanf			#Call scanf
        mov    $num2, %rdi		#Move num2 prompt to arg1
        call    printf			#Call printf
        lea    -16(%rbp), %rax		#Load address of b into return
        mov    %rax, %rsi		#Move address of b into arg2
        mov    $input, %rdi		#Move input prompt to arg1
        call    scanf			#Call scanf
        mov    -16(%rbp), %rsi		#Move b to arg2
        mov    -8(%rbp), %rdi		#Move a to arg1
        call    gcd			#Call gcd
        mov    %rax, %rsi		#Move result to arg2
        mov    $answer, %rdi		#Move answer prompt to arg1
        call    printf			#Call printf
        leave				#Destroy stack frame    
        ret				#End program

#Section for prompting user using strings        
        .data
num1:
        .string "Enter the first number: "
input:
        .string "%ld"
num2:
        .string "Enter the second number: "
answer:
        .string "The greatest common divisor is %ld\n"
