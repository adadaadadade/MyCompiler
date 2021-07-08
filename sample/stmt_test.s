.text
#函数main代码
	.global main
main:
	# 函数入口
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# 开辟栈帧
	sub sp,sp,#28
	# 加载参数变量到寄存器
	# 函数内代码
.LFB0:
	mov r8,#1
	cmp r8,#0
	beq .L0
	mov r8,#1
	str r8,[fp,#-16]
	b .L1
.L0:
	mov r8,#2
	str r8,[fp,#-16]
.L1:
	mov r8,#0
	str r8,[fp,#-16]
.L2:
	ldr r8,[fp,#-16]
	mov r9,#5
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-20]
	ldr r8,[fp,#-20]
	cmp r8,#0
	beq .L3
	ldr r8,[fp,#-16]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-24]
	ldr r8,[fp,#-24]
	str r8,[fp,#-16]
	ldr r8,[fp,#-16]
	mov r9,#3
	cmp r8,r9
	moveq r8,#1
	movne r8,#0
	str r8,[fp,#-28]
	ldr r8,[fp,#-28]
	cmp r8,#0
	beq .L7
	b .L3
	b .L8
.L7:
.L8:
	b .L2
.L3:
	mov r8,#0
	str r8,[fp,#-16]
.L9:
	ldr r8,[fp,#-16]
	mov r9,#5
	cmp r8,r9
	movlt r8,#1
	movge r8,#0
	str r8,[fp,#-24]
	ldr r8,[fp,#-24]
	cmp r8,#0
	beq .L10
	b .L12
.L13:
	ldr r8,[fp,#-16]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-28]
	ldr r8,[fp,#-28]
	str r8,[fp,#-16]
	b .L9
.L12:
	b .L13
	mov r8,#245
	str r8,[fp,#-16]
	b .L13
.L10:
	ldr r0,[fp,#-16]
	b .LFE0
.LFE0:
	# 函数出口
	ldmea fp,{fp,sp,pc}
