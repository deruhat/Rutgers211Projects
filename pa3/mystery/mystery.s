	.file	"mystery.c"
	.text
.globl add
	.type	add, @function
add:
.LFB18:
	.cfi_startproc
	leal	(%rsi,%rdi), %eax
	ret
	.cfi_endproc
.LFE18:
	.size	add, .-add
.globl dothething
	.type	dothething, @function
dothething:
.LFB19:
	.cfi_startproc
	movq	%rbx, -16(%rsp)
	movq	%rbp, -8(%rsp)
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -16
	.cfi_offset 3, -24
	movl	%edi, %ebx
	movslq	%edi, %rax
	cmpl	$-1, num(,%rax,4)
	jne	.L4
	testl	%edi, %edi
	jne	.L5
	movl	$0, num(%rip)
	jmp	.L4
.L5:
	cmpl	$1, %edi
	jne	.L6
	movl	$1, num+4(%rip)
	jmp	.L4
.L6:
	leal	-2(%rbx), %edi
	call	dothething
	movl	%eax, %ebp
	leal	-1(%rbx), %edi
	call	dothething
	movslq	%ebx, %rdx
	leal	(%rax,%rbp), %ebp
	movl	%ebp, num(,%rdx,4)
.L4:
	movslq	%ebx, %rbx
	movl	num(,%rbx,4), %eax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE19:
	.size	dothething, .-dothething
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%i\n"
	.text
.globl main
	.type	main, @function
main:
.LFB20:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	8(%rsi), %rdi
	movl	$10, %edx
	movl	$0, %esi
	call	strtol
	movl	$0, %edx
	movl	$num, %ecx
.L9:
	movl	$-1, (%rcx,%rdx)
	addq	$4, %rdx
	cmpq	$800, %rdx
	jne	.L9
	movl	%eax, %edi
	call	dothething
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE20:
	.size	main, .-main
	.comm	num,800,32
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-16)"
	.section	.note.GNU-stack,"",@progbits
