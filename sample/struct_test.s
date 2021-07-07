	.arch armv8-a
	.file	"struct_test.c"
	.text
	.align	2
	.global	func
	.type	func, %function
func:
.LFB0:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	x0, [sp, 8]
	str	wzr, [sp, 20]	#i1
	mov	w0, 1
	str	w0, [sp, 24]	#i2
	mov	w0, 2
	str	w0, [sp, 28]	#i3
	ldrsw	x1, [sp, 20]
	mov	x0, x1
	lsl	x0, x0, 1	#2i1
	add	x0, x0, x1	#3i1
	lsl	x0, x0, 3	#24i1
	mov	x1, x0
	ldr	x0, [sp, 8]	#a
	add	x2, x0, x1	#a + 24i1
	ldrsw	x3, [sp, 28]	#i3
	ldrsw	x1, [sp, 24]	#i2
	mov	x0, x1	#i2
	lsl	x0, x0, 1	#2i2
	add	x0, x0, x1	#3i2
	add	x0, x0, x3	#3i2 + i3
	mov	w1, 1
	str	w1, [x2, x0, lsl 2]
	ldrsw	x1, [sp, 20]
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	lsl	x0, x0, 3
	mov	x1, x0
	ldr	x0, [sp, 8]
	add	x2, x0, x1
	ldrsw	x3, [sp, 28]
	ldrsw	x1, [sp, 24]
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	add	x0, x0, x3
	ldr	w0, [x2, x0, lsl 2]
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	func, .-func
	.align	2
	.global	main
	.type	main, %function
main:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 56]
	mov	x1,0
	str	wzr, [sp, 16]
	mov	w0, 1
	str	w0, [sp, 20]
	mov	w0, 2
	str	w0, [sp, 24]
	ldrsw	x3, [sp, 24]
	ldrsw	x1, [sp, 16]
	ldrsw	x2, [sp, 20]
	mov	x0, x1
	lsl	x0, x0, 1
	add	x0, x0, x1
	lsl	x0, x0, 1
	mov	x1, x2
	lsl	x1, x1, 1
	add	x1, x1, x2
	add	x0, x0, x1
	add	x0, x0, x3
	lsl	x0, x0, 2
	add	x1, sp, 32
	mov	w2, 2
	str	w2, [x1, x0]
	add	x0, sp, 32
	bl	func
	str	w0, [sp, 28]
	mov	w0, 0
	mov	w1, w0
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 56]
	ldr	x3, [x0]
	subs	x2, x2, x3
	mov	x3, 0
	beq	.L5
	bl	__stack_chk_fail
.L5:
	mov	w0, w1
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
