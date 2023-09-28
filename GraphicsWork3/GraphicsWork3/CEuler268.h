#pragma once
class CVector268;
class CMatrix268;
class CQuaternion268;

class CEuler268
{
public:
	//默认
	CEuler268();
	~CEuler268();
	//含参构造器
	CEuler268(float ch,float cp, float cb);
//成员变量
	float h, p, b;
	void Set(float fh, float fp, float fb);
	operator float*() { return &h; }

//成员函数

	
	CVector268 ToVector();//欧拉角转换为向量
	CMatrix268 ToMatrix();//欧拉角转换为矩阵
	CQuaternion268 ToQuaternion();//欧拉角转换为四元数
	void Normal();//欧拉角规范化//h,r角度在-180~180，p角度在-90~90。万向锁时,h代替所有r的度数。

};

