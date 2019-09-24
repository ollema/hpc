	.file	"time.c"
	.text
	.section	.rodata
.LC0:
	.string	"Result: \t\t\t%zd\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	$0, -8(%rbp)
	movl	$1, -12(%rbp)
	jmp	.L2
.L3:
	movl	-12(%rbp), %eax
	cltq
	addq	%rax, -8(%rbp)
	addl	$1, -12(%rbp)
.L2:
	cmpl	$1000000000, -12(%rbp)
	jle	.L3
	movq	-8(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
