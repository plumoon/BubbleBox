#include "stdafx.h"
#include "CVector268.h"
#include "CEuler268.h"
#include "math.h"

//默认构造器
CVector268::CVector268()
{
	CVector268(0., 0., 0.);
}

//含参构造器
CVector268::CVector268(float xo, float yo, float zo)
{
	x = xo;
	y = yo;
	z = zo;
}

CVector268::~CVector268()
{
}

//Set
void CVector268::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// +
CVector268 CVector268::operator+(CVector268 &p)
{
	CVector268 cvtool;
	cvtool.x = this->x + p.x;
	cvtool.y = this->y + p.y;
	cvtool.z = this->z + p.z;
	return cvtool;
}

// -
CVector268 CVector268::operator-(CVector268 &p)
{
	CVector268 cvtool;
	cvtool.x = this->x - p.x;
	cvtool.y = this->y - p.y;
	cvtool.z = this->z - p.z;
	return cvtool;
}

// *
CVector268 CVector268::operator*(float p)
{
	CVector268 cvtool;
	cvtool.x = x * p;
	cvtool.y = y * p;
	cvtool.z = z * p;
	return cvtool;
}

// =
void CVector268::operator=(CVector268 &p)
{
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
}

// ==
bool CVector268::operator==(CVector268 &p)
{
	if (this->x == p.x
		&& this->y == p.y
		&& this->z == p.z) return true;
	else return false;
}

// !=
bool CVector268::operator!=(CVector268 &p)
{
	if (this->x == p.x
		&& this->y == p.y
		&& this->z == p.z) return false;
	else return true;
}

//向量点乘操作
float CVector268::dotMul(CVector268 &n)
{
	float f1 = 0;
	f1 += this->x*n.x;
	f1 += this->y*n.y;
	f1 += this->z*n.z;
	return f1;
}

//向量叉乘操作
CVector268 CVector268::crossMul(CVector268 &n)
{
	CVector268 cv;
	cv.x = y * n.z - z * n.y;
	cv.y = z * n.x - x * n.z;
	cv.z = x * n.y - y * n.x;
	return cv;
}

//向量求模操作
float CVector268::len()
{
	float f2;
	f2 = (float)sqrt(x*x + y*y + z*z);
	return f2;
}

//向量单位化操作 
void CVector268::Normalize()
{
	float f1 = this->len();
	x /= f1;
	y /= f1;
	z /= f1;
}

//向量投影操作
CVector268 CVector268::project(CVector268 &n)
{
	CVector268 cvtool;
	float d = n.len();
	cvtool.x = (this->dotMul(n)) / d * n.x / d;
	cvtool.y = (this->dotMul(n)) / d * n.y / d;
	cvtool.z = (this->dotMul(n)) / d * n.z / d;
	return cvtool;
}

//数组化成员变量引用
float& CVector268::operator[] (int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	return x;
}

// *定义向量数乘，其中数在左边 
CVector268 operator*(float a, CVector268 &p)
{
	CVector268 cvtool;
	cvtool.x = p.x * a;
	cvtool.y = p.y * a;
	cvtool.z = p.z * a;
	return cvtool;
}

//向量转换为欧拉角
CEuler268 CVector268::ToEuler()
{
	CEuler268 tool;
	CVector268 temp(x, 0, z);
	this->Normalize();
	temp.Normalize();
	float fp = this->dotMul(temp);
	float fh = temp.dotMul(CVector268(0, 0, -1));

	tool.p = 180 / 3.1415926 *(float)acos(fp);
	tool.h = 180 / 3.1415926 *(float)acos(fh);
	if (x > 0) tool.h *= -1;
	if (y < 0) tool.p *= -1;
	tool.b = 0.;
	//tool.Normal();

	return tool;
}
