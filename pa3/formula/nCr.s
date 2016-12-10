.globl nCr
	.type	nCr, @function
nCr:
	pushl	%ebp # setting the stack frame
	movl	%esp, %ebp
	subl	$36, %esp
	movl	8(%ebp), %eax # moving i value to EAX
	movl	%eax, (%esp) # setting the value of the current stack pointer to EAX 
	call	Factorial # call factorial
	movl	%eax, -20(%ebp) # take the value in EAX and save it at the location 20 above EBP
	cmpl	$-1, -20(%ebp) # compare the location 20 above EBP to -1
	jne	PROCEED_1 # if it's not equal, go to L6
	movl	$-1, %eax # if it's equal, move the value -1 to EAX (Overflow)
	jmp	END_nCr # go to END_nCr to return value from nCr
PROCEED_1:
	movl	12(%ebp), %eax # take value located 12 below EBP and save it in EAX
	movl	%eax, (%esp) # take EAX and save it's value in ESP (value not address)
	call	Factorial # call factorial
	movl	%eax, -16(%ebp) # take what is in EAX now and save it at 16 above EBP
	cmpl	$-1, -16(%ebp) #compare -1 to what is in 16 above EBP
	jne	PROCEED_2 # if it's not equal, go to L8
	movl	$-1, %eax # if it's equal, copy -1 to EAX (Overflow)
	jmp	END_nCr # then jump to END_nCr and return from nCr
PROCEED_2:
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	subl	%eax, %edx
	movl	%edx, (%esp)
	call	Factorial
	movl	%eax, -12(%ebp)
	cmpl	$-1, -12(%ebp)
	jne	PROCEED_3
	movl	$-1, %eax # (Overflow)
	jmp	END_nCr
PROCEED_3:
	movl	-16(%ebp), %eax
	imull	-12(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	-8(%ebp) # EAX will contain the quotient, EDX will contain the remainder
END_nCr:
	leave
	ret
	
.globl Factorial
	.type	Factorial, @function
Factorial:
  # Your code for Factorial should go here
	pushl	%ebp # setting the new stack frame
	movl	%esp, %ebp
	subl	$16, %esp
	movl	$1, -4(%ebp)
	jmp	LOOPCMP
OVERFLOW_f:	
	movl	$-1, %eax #(Overflow)
	jmp	END_f
KEEP_GOING:	
	movl	-4(%ebp), %eax
	imull	8(%ebp), %eax
	jb	OVERFLOW_f
	movl	%eax, -4(%ebp)
	subl	$1, 8(%ebp)
LOOPCMP:
	cmpl	$0, 8(%ebp) # compare 0 to what's at 8 below EBP 
	jg	KEEP_GOING # if it's greater, dont break the loop, keep multiplying
	movl	-4(%ebp), %eax # loop broken, move what's at 4 above EBP to EAX and proceed to L5 and end the call 
END_f:
	leave
	ret
	