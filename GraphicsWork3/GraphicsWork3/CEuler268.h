#pragma once
class CVector268;
class CMatrix268;
class CQuaternion268;

class CEuler268
{
public:
	//Ĭ��
	CEuler268();
	~CEuler268();
	//���ι�����
	CEuler268(float ch,float cp, float cb);
//��Ա����
	float h, p, b;
	void Set(float fh, float fp, float fb);
	operator float*() { return &h; }

//��Ա����

	
	CVector268 ToVector();//ŷ����ת��Ϊ����
	CMatrix268 ToMatrix();//ŷ����ת��Ϊ����
	CQuaternion268 ToQuaternion();//ŷ����ת��Ϊ��Ԫ��
	void Normal();//ŷ���ǹ淶��//h,r�Ƕ���-180~180��p�Ƕ���-90~90��������ʱ,h��������r�Ķ�����

};

