.text
#函数main代码
	.global main
main:
	# 函数入口
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# 开辟栈帧
	sub sp,sp,#48
	# 加载参数变量到寄存器
	# 函数内代码
.LFB0:
	mov r8,#1
	str r8,[fp,#-16]
	mov r8,#2
	str r8,[fp,#-20]
	mov r8,#3
	str r8,[fp,#-24]
	mov r8,#4
	str r8,[fp,#-28]
	ldr r8,[fp,#-20]
	ldr r9,[fp,#-24]
	mul r10,r8,r9
	mov r8,r10
	str r8,[fp,#-40]
	ldr r8,[fp,#-16]
	ldr r9,[fp,#-40]
	add r8,r8,r9
	str r8,[fp,#-44]
	ldr r8,[fp,#-28]
	mov r9,#1
	lsl r8,r8,r9
	str r8,[fp,#-48]
	ldr r8,[fp,#-44]
	ldr r9,[fp,#-48]
	add r8,r8,r9
	str r8,[fp,#-52]
	ldr r8,[fp,#-52]
	str r8,[fp,#-32]
	ldr r8,[fp,#-16]
	mov r9,#0
	cmp r8,#0
	moveq r8,#0
	movne r8,#1
	cmpne r9,#0
	moveq r8,#0
	str r8,[fp,#-56]
	ldr r8,[fp,#-56]
	strb r8,[fp,#-36]
	ldr r0,[fp,#-32]
	b .LFE0
.LFE0:
	# 函数出口
	ldmea fp,{fp,sp,pc}
