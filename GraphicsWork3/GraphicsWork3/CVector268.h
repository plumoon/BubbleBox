#pragma once

class CEuler268;

class CVector268
{
public:
	//Ĭ�Ϲ��캯����ͨ���Ժ��ι��캯���ĵ��ã�ʹ����Ϊ0����
	CVector268();
	//���ι��캯��
	CVector268(float xo, float yo, float zo);
	~CVector268();

	float x = 0.;
	float y = 0.;
	float z = 0.;
	void Set(float x, float y, float z);//���ó�Ա����ֵ
	CVector268 operator+(CVector268 &p);//���ء�+������
	CVector268 operator-(CVector268 &p);//���ء�-������
	CVector268 operator*(float p);//���ء�*������ ����
	void operator=(CVector268 &p);//���ء�=������
	bool operator==(CVector268 &p);//���ء�==������
	bool operator!=(CVector268 &p);//���ء�!=������
	operator float*() { return &x; }; 	//�Զ�����ת��

	float dotMul(CVector268 &n);//������˲���
	CVector268 crossMul(CVector268 &n);//������˲���
	void Normalize();//������λ������ 
	float len();//������ģ����
	CVector268 project(CVector268 &n);//����ͶӰ����
	float& operator[] (int i);//���黯��Ա��������
	friend CVector268 operator*(float a, CVector268 &p);

	CEuler268 ToEuler();//����ת��Ϊŷ����
};
