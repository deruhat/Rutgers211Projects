	.file	"mystery.c"
	.comm	num,800,32
	.text
.globl add
	.type	add, @function
add:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	leal	(%rdx,%rax), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	add, .-add
.globl dothething
	.type	dothething, @function
dothething:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	cmpl	$-1, %eax
	jne	.L4
	.cfi_offset 3, -32
	.cfi_offset 12, -24
	cmpl	$0, -20(%rbp)
	jne	.L5
	movl	-20(%rbp), %eax
	cltq
	movl	$0, num(,%rax,4)
	jmp	.L4
.L5:
	cmpl	$1, -20(%rbp)
	jne	.L6
	movl	-20(%rbp), %eax
	cltq
	movl	$1, num(,%rax,4)
	jmp	.L4
.L6:
	movl	-20(%rbp), %r12d
	movl	-20(%rbp), %eax
	subl	$2, %eax
	movl	%eax, %edi
	call	dothething
	movl	%eax, %ebx
	movl	-20(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	dothething
	leal	(%rbx,%rax), %edx
	movslq	%r12d, %rax
	movl	%edx, num(,%rax,4)
.L4:
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	dothething, .-dothething
	.section	.rodata
.LC0:
	.string	"%i\n"
	.text
.globl main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movl	$0, -8(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -4(%rbp)
	jmp	.L9
.L10:
	movl	-8(%rbp), %eax
	cltq
	movl	$-1, num(,%rax,4)
	addl	$1, -8(%rbp)
.L9:
	cmpl	$199, -8(%rbp)
	jle	.L10
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	dothething
	movl	%eax, %edx
	movl	$.LC0, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-16)"
	.section	.note.GNU-stack,"",@progbits
