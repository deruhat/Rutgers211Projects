	.file	"mystery.c"
	.text
.globl add
	.type	add, @function
add:
	pushl	%ebp
	movl	%esp, %ebp
	movl	12(%ebp), %eax # copy second variable to EAX
	addl	8(%ebp), %eax # add first variable to EAX and save in EAX
	popl	%ebp
	ret
	.size	add, .-add
.globl dothething
	.type	dothething, @function
dothething:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$28, %esp # subtract 28 from stack pointer
	movl	8(%ebp), %eax # move the first variable to EAX 
	movl	num(,%eax,4), %eax
	cmpl	$-1, %eax # see if the value is -1, if yes go to if statement in L4 
	je	.L4
	movl	8(%ebp), %eax # if not equal, copy the first value to EAX 
	movl	num(,%eax,4), %eax # move 4 times the value to EAX 
	movl	%eax, -24(%ebp)
	jmp	.L6
.L4:
	movl	$-1, -8(%ebp)
	cmpl	$0, 8(%ebp) # check the value if it equals to 0
	jne	.L7 # if not, go to next if statement 
	movl	$0, -8(%ebp)
	jmp	.L9
.L7:
	cmpl	$1, 8(%ebp) # see if value equals 1 
	jne	.L10 # if not, go to last if statement in L10 and start recursion process to find the fib num 
	movl	$1, -8(%ebp)
	jmp	.L9
.L10:
	movl	8(%ebp), %eax
	subl	$2, %eax
	movl	%eax, (%esp)
	call	dothething # recursion  ( find value for fib process )
	movl	%eax, %ebx
	movl	8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, (%esp)
	call	dothething # recursion ( find valuer for fib process )
	movl	%ebx, 4(%esp)
	movl	%eax, (%esp)
	call	add # add the two values from recursion together
	movl	%eax, -8(%ebp)
.L9:
	movl	8(%ebp), %eax
	movl	num(,%eax,4), %eax
	cmpl	$-1, %eax
	jne	.L12
	movl	8(%ebp), %edx
	movl	-8(%ebp), %eax
	movl	%eax, num(,%edx,4)
.L12:
	movl	8(%ebp), %eax
	movl	num(,%eax,4), %eax
	movl	%eax, -24(%ebp)
.L6:
	movl	-24(%ebp), %eax
	addl	$28, %esp
	popl	%ebx
	popl	%ebp
	ret
	.size	dothething, .-dothething
	.section	.rodata
.LC0:
	.string	"Value:   %d\n"
	.text
.globl main
	.type	main, @function
main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ecx
	subl	$36, %esp
	movl	4(%ecx), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	call	atoi # convert the argument into an integer
	movl	%eax, -12(%ebp)
	movl	$0, -8(%ebp)
	jmp	.L16
.L17:
	movl	-8(%ebp), %eax
	movl	$-1, num(,%eax,4)
	addl	$1, -8(%ebp)
.L16:
	cmpl	$199, -8(%ebp) # compare the value of counter to 199 
	jle	.L17 # if less or equal to, stay in WHILE LOOP! If not:
	movl	-12(%ebp), %eax 
	movl	%eax, (%esp)
	call	dothething
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	movl	$0, %eax
	addl	$36, %esp
	popl	%ecx
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
	.size	main, .-main
	.comm	num,800,32
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-51)"
	.section	.note.GNU-stack,"",@progbits