#pragma once
class CVector268;
class CEuler268;
class CQuaternion268;
class CMatrix268
{
public:
	//Ĭ�Ϲ��캯����ͨ���Ժ��ι��캯���ĵ��ã�ʹ����Ϊ0����
	CMatrix268();
	//���ι��캯��
	CMatrix268(float a00, float a10, float a20, float a30,
		float a01, float a11, float a21, float a31,
		float a02, float a12, float a22, float a32,
		float a03, float a13, float a23, float a33);
	~CMatrix268();

	//����
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	//ת��
	operator float*() { return &m00; }
	operator  const float*() const { return &m00; }//�������� = ������ʱѭ����ֵʹ��
												   //��Ա����
	void Set(float *p);	//������16��Ԫ�ظ�ֵ
	CMatrix268& operator=(const CMatrix268& p);//����ֵ
	CMatrix268 operator*(float d);//��������
	CMatrix268 operator*(CMatrix268& p);//�������
	CVector268 vecMul(CVector268& p); //�������������
	CVector268 posMul(CVector268& p);//������λ�����
	void SetRotate(float seta, CVector268 axis);	//����Ϊ��ת����
	void SetTrans(CVector268 trans);		//����Ϊƽ�ƾ���
	void SetScale(CVector268 p);		//����Ϊ���ž���
	float Inverse();//��������,�ɹ���������ʽ��ֵ�����򷵻�0
	CMatrix268 GetInverse();//���������
	float GetDet();//��������ʽ��ֵ
	void noneNegativeZero();//�����и�0תΪ0

	CEuler268 ToEuler();//����ת��Ϊŷ����
	CQuaternion268 ToQuaternion();//����ת��Ϊ��Ԫ��
};
