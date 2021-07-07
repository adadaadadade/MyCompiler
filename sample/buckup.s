	.arch armv8-a
	.file	"struct_test.c"
	.text
.Ltext0:
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.file 1 "struct_test.c"
	.loc 1 3 1
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	.loc 1 3 1
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 56]
	mov	x1,0
	.loc 1 5 9
	str	wzr, [sp, 20]
	.loc 1 6 9
	mov	w0, 1
	str	w0, [sp, 24]
	.loc 1 7 9
	mov	w0, 2
	str	w0, [sp, 28]
	.loc 1 8 19
	ldrsw	x3, [sp, 28]
	ldrsw	x1, [sp, 20]
	ldrsw	x2, [sp, 24]
	mov	x0, x1	#x1
	lsl	x0, x0, 1	#2i1
	add	x0, x0, x1	#3i1
	lsl	x0, x0, 1	#6x1
	mov	x1, x2	#x2
	lsl	x1, x1, 1	#2x2
	add	x1, x1, x2	#3x2
	add	x0, x0, x1	#6x1 + 3x2
	add	x0, x0, x3	#6x1 + 3x2 +x3
	lsl	x0, x0, 2	#24x1 + 12x2 + 4x3
	add	x1, sp, 32
	mov	w2, 2
	str	w2, [x1, x0]
	mov	w0, 0
	mov	w1, w0
	.loc 1 9 1
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, #:got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 56]
	ldr	x3, [x0]
	subs	x2, x2, x3
	mov	x3, 0
	beq	.L3
	bl	__stack_chk_fail
.L3:
	mov	w0, w1
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.align	2
	.global	func
	.type	func, %function
func:
.LFB1:
	.loc 1 11 1
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	.loc 1 16 6
	ldrsw	x0, [sp, 12]	#i3
	lsl	x0, x0, 3	#8i3
	ldr	x1, [sp, 24]	#a
	add	x0, x1, x0	#a + 8i3
	ldr	x1, [x0]	#o1 = [a + 8i3]
	.loc 1 16 10
	ldrsw	x0, [sp, 16]	#i2
	lsl	x0, x0, 3	#8i2
	add	x0, x1, x0	#o1 + 8i2
	ldr	x1, [x0]	#o2 = [i + 8i2]
	.loc 1 16 14
	ldrsw	x0, [sp, 20]	#i1
	lsl	x0, x0, 2	#8i1
	add	x0, x1, x0	#o2 + 8i1
	.loc 1 16 19
	mov	w1, 1
	str	w1, [x0]
	.loc 1 18 1
	nop
	add	sp, sp, 32
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	func, .-func
	.align	2
	.global	sum
	.type	sum, %function
sum:
.LFB2:
	.loc 1 21 1
	.cfi_startproc
	.loc 1 22 12
	mov	w0, 1
	.loc 1 23 1
	ret
	.cfi_endproc
.LFE2:
	.size	sum, .-sum
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x138
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.4byte	.LASF3
	.byte	0xc
	.4byte	.LASF4
	.4byte	.LASF5
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.string	"sum"
	.byte	0x1
	.byte	0x14
	.byte	0x5
	.4byte	0x4b
	.8byte	.LFB2
	.8byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x4
	.4byte	.LASF0
	.byte	0x1
	.byte	0xa
	.byte	0x5
	.4byte	0x4b
	.8byte	.LFB1
	.8byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.4byte	0xac
	.uleb128 0x5
	.string	"a"
	.byte	0x1
	.byte	0xc
	.byte	0xc
	.4byte	0xac
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.uleb128 0x5
	.string	"a1"
	.byte	0x1
	.byte	0xd
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x5
	.string	"a2"
	.byte	0x1
	.byte	0xe
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x5
	.string	"a3"
	.byte	0x1
	.byte	0xf
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x6
	.byte	0x8
	.4byte	0xb2
	.uleb128 0x6
	.byte	0x8
	.4byte	0xb8
	.uleb128 0x6
	.byte	0x8
	.4byte	0x4b
	.uleb128 0x7
	.4byte	.LASF1
	.byte	0x1
	.byte	0x2
	.byte	0x5
	.4byte	0x4b
	.8byte	.LFB0
	.8byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x118
	.uleb128 0x5
	.string	"a"
	.byte	0x1
	.byte	0x4
	.byte	0x9
	.4byte	0x118
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x5
	.string	"i1"
	.byte	0x1
	.byte	0x5
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x5
	.string	"i2"
	.byte	0x1
	.byte	0x6
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.string	"i3"
	.byte	0x1
	.byte	0x7
	.byte	0x9
	.4byte	0x4b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.byte	0
	.uleb128 0x8
	.4byte	0x4b
	.4byte	0x134
	.uleb128 0x9
	.4byte	0x134
	.byte	0
	.uleb128 0x9
	.4byte	0x134
	.byte	0x1
	.uleb128 0x9
	.4byte	0x134
	.byte	0x2
	.byte	0
	.uleb128 0xa
	.byte	0x8
	.byte	0x7
	.4byte	.LASF2
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.8byte	0
	.8byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF0:
	.string	"func"
.LASF2:
	.string	"long unsigned int"
.LASF5:
	.string	"/home/weijj/projects/MyCompiler/sample"
.LASF4:
	.string	"struct_test.c"
.LASF3:
	.string	"GNU C17 9.3.0 -mlittle-endian -mabi=lp64 -g -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection"
.LASF1:
	.string	"main"
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
