.text
#函数ret_one代码
	.global ret_one
ret_one:
	# 函数入口
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# 开辟栈帧
	sub sp,sp,#4
	# 加载参数变量到寄存器
	# 函数内代码
.LFB0:
	mov r8,#1
	b .LFE0
.LFE0:
	# 函数出口
	ldmea fp,{fp,sp,pc}
#函数main代码
	.global main
main:
	# 函数入口
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# 开辟栈帧
	sub sp,sp,#24
	# 加载参数变量到寄存器
	# 函数内代码
.LFB1:
	mov r8,#2
	str r8,[fp,#-16]
	ldr r8,[fp,#-16]
	mov r9,#1
	add r8,r8,r9
	str r8,[fp,#-20]
	ldr r8,[fp,#-20]
	str r8,[fp,#-24]
	bl ret_one
	add sp,sp,#0
	str r8,[fp,#-28]
	ldr r8,[fp,#-28]
	str r8,[fp,#-32]
.LFE1:
	# 函数出口
	ldmea fp,{fp,sp,pc}
