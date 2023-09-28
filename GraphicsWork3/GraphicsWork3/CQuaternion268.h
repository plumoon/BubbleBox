#pragma once
class CVector268;
class CMatrix268;
class CEuler268;

class CQuaternion268
{
public:
	CQuaternion268();
	~CQuaternion268();
	//含参构造器
	CQuaternion268(float cx,float cy,float cz,float cw);
//成员变量
	float x, y, z, w;
	void Set(float fx, float fy, float fz, float fw);
	operator float*() { return &x; }

//成员函数
	CEuler268 ToEuler();//四元数转换为欧拉角
	CMatrix268 ToMatrix();//四元数转换为矩阵
	void SetAngle(float angle, CVector268 axis); //四元数设置
	CQuaternion268& operator=(const CQuaternion268& p); //重载赋值
	CQuaternion268 operator+(const CQuaternion268& p); //重载‘+’
	CQuaternion268 operator*(float data); //重载数乘
	CQuaternion268 operator*(const CQuaternion268&p); //四元数乘法
	float dotMul(const CQuaternion268&p); //点乘
	float len(); //求模
	bool Normalize();	//求标准化
	CQuaternion268& Inverse();//求逆四元数,会改变自身。
	CQuaternion268 GetInverse();//求逆四元数,不改变自身，生成新的四元数
	CQuaternion268 Div(const CQuaternion268&b); //求差 当前为a,求c=a-b
	void GetAngle(float& angle, CVector268& axis); //求旋转轴和角度
	CQuaternion268 Slerp(const CQuaternion268& Vend, float t); //插值。从当前四元数插值到Vend四元数,t是参数[0,1]
	void Slerp(const CQuaternion268& Vend, int n, float *t, CQuaternion268 *Result);//插值。一次插值出n个数据。插值参数保存在数组t中，结果返回到数组Result中。
	CQuaternion268 getPower(float t);//四元数求幂
};

