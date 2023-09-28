#pragma once
class CVector268;
class CEuler268;
class CQuaternion268;
class CMatrix268
{
public:
	//默认构造函数，通过对含参构造函数的调用，使矩阵为0矩阵
	CMatrix268();
	//含参构造函数
	CMatrix268(float a00, float a10, float a20, float a30,
		float a01, float a11, float a21, float a31,
		float a02, float a12, float a22, float a32,
		float a03, float a13, float a23, float a33);
	~CMatrix268();

	//属性
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	//转换
	operator float*() { return &m00; }
	operator  const float*() const { return &m00; }//仅供重载 = 操作符时循环赋值使用
												   //成员函数
	void Set(float *p);	//给矩阵16个元素赋值
	CMatrix268& operator=(const CMatrix268& p);//矩阵赋值
	CMatrix268 operator*(float d);//矩阵数乘
	CMatrix268 operator*(CMatrix268& p);//矩阵相乘
	CVector268 vecMul(CVector268& p); //矩阵与向量相乘
	CVector268 posMul(CVector268& p);//矩阵与位置相乘
	void SetRotate(float seta, CVector268 axis);	//设置为旋转矩阵
	void SetTrans(CVector268 trans);		//设置为平移矩阵
	void SetScale(CVector268 p);		//设置为缩放矩阵
	float Inverse();//矩阵求逆,成功返回行列式的值，否则返回0
	CMatrix268 GetInverse();//返回逆矩阵
	float GetDet();//返回行列式的值
	void noneNegativeZero();//将所有负0转为0

	CEuler268 ToEuler();//矩阵转换为欧拉角
	CQuaternion268 ToQuaternion();//矩阵转换为四元数
};
