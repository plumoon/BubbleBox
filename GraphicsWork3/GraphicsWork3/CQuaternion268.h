#pragma once
class CVector268;
class CMatrix268;
class CEuler268;

class CQuaternion268
{
public:
	CQuaternion268();
	~CQuaternion268();
	//���ι�����
	CQuaternion268(float cx,float cy,float cz,float cw);
//��Ա����
	float x, y, z, w;
	void Set(float fx, float fy, float fz, float fw);
	operator float*() { return &x; }

//��Ա����
	CEuler268 ToEuler();//��Ԫ��ת��Ϊŷ����
	CMatrix268 ToMatrix();//��Ԫ��ת��Ϊ����
	void SetAngle(float angle, CVector268 axis); //��Ԫ������
	CQuaternion268& operator=(const CQuaternion268& p); //���ظ�ֵ
	CQuaternion268 operator+(const CQuaternion268& p); //���ء�+��
	CQuaternion268 operator*(float data); //��������
	CQuaternion268 operator*(const CQuaternion268&p); //��Ԫ���˷�
	float dotMul(const CQuaternion268&p); //���
	float len(); //��ģ
	bool Normalize();	//���׼��
	CQuaternion268& Inverse();//������Ԫ��,��ı�����
	CQuaternion268 GetInverse();//������Ԫ��,���ı����������µ���Ԫ��
	CQuaternion268 Div(const CQuaternion268&b); //��� ��ǰΪa,��c=a-b
	void GetAngle(float& angle, CVector268& axis); //����ת��ͽǶ�
	CQuaternion268 Slerp(const CQuaternion268& Vend, float t); //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
	void Slerp(const CQuaternion268& Vend, int n, float *t, CQuaternion268 *Result);//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result�С�
	CQuaternion268 getPower(float t);//��Ԫ������
};

