#pragma once 
#include"common.h"
#include "iloc.h"

//指令选择器
class Selector
{
	vector<InterInst*>& ir;
	ILoc& iloc;
	Func *cur_func;
	//翻译
	void translate(InterInst*inst);
public:
	Selector(vector<InterInst*>& ir,ILoc& iloc);
	//指令选择
	void select();
};
