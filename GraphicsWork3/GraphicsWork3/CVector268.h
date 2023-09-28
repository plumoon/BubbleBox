#pragma once

class CEuler268;

class CVector268
{
public:
	//默认构造函数，通过对含参构造函数的调用，使向量为0向量
	CVector268();
	//含参构造函数
	CVector268(float xo, float yo, float zo);
	~CVector268();

	float x = 0.;
	float y = 0.;
	float z = 0.;
	void Set(float x, float y, float z);//设置成员变量值
	CVector268 operator+(CVector268 &p);//重载“+”操作
	CVector268 operator-(CVector268 &p);//重载“-”操作
	CVector268 operator*(float p);//重载“*”操作 数乘
	void operator=(CVector268 &p);//重载“=”操作
	bool operator==(CVector268 &p);//重载“==”操作
	bool operator!=(CVector268 &p);//重载“!=”操作
	operator float*() { return &x; }; 	//自动类型转换

	float dotMul(CVector268 &n);//向量点乘操作
	CVector268 crossMul(CVector268 &n);//向量叉乘操作
	void Normalize();//向量单位化操作 
	float len();//向量求模操作
	CVector268 project(CVector268 &n);//向量投影操作
	float& operator[] (int i);//数组化成员变量引用
	friend CVector268 operator*(float a, CVector268 &p);

	CEuler268 ToEuler();//向量转换为欧拉角
};
