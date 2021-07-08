.text
#函数main代码
	.global main
main:
	# 函数入口
	mov ip,sp
	stmfd sp!,{fp,ip,lr,pc}
	sub fp,ip,#4
	# 开辟栈帧
	sub sp,sp,#12
	# 加载参数变量到寄存器
	# 函数内代码
.LFB0:
	mov r8,#1
	str r8,[fp,#-16]
	mov r8,#2
	str r8,[fp,#-20]
	ldr r0,[fp,#-16]
	b .LFE0
.LFE0:
	# 函数出口
	ldmea fp,{fp,sp,pc}
