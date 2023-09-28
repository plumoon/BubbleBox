#include "stdafx.h"
#include "CMatrix268.h"
#include "CVector268.h"
#include "CEuler268.h"
#include "CQuaternion268.h"
#include <math.h>

#define PI 3.1415926

CMatrix268::CMatrix268()
{
	CMatrix268(0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.);
}
CMatrix268::CMatrix268(float a00, float a10, float a20, float a30,
	float a01, float a11, float a21, float a31,
	float a02, float a12, float a22, float a32,
	float a03, float a13, float a23, float a33)
{
	m00 = a00; m10 = a10; m20 = a20; m30 = a30;
	m01 = a01; m11 = a11; m21 = a21; m31 = a31;
	m02 = a02; m12 = a12; m22 = a22; m32 = a32;
	m03 = a03; m13 = a13; m23 = a23; m33 = a33;
}

CMatrix268::~CMatrix268()
{
}

//����ֵ
//�����أ�ֻ�е��Ҳ�Ϊ����Ϊconst��CMatrix268����Ч
CMatrix268& CMatrix268::operator=(const CMatrix268 & p)
{
	//����ѭ��������ֵ
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = p[i];
	}
	//���ض����������
	return *this;
}
//������16��Ԫ�ظ�ֵ
void CMatrix268::Set(float *p)
{
	//����ѭ��������ֵ
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = *p;
	}
}
//��������
CMatrix268 CMatrix268::operator*(float d)
{
	CMatrix268 cmtool;
	//����ѭ��������ֵ
	for (int i = 0; i < 16; i++)
	{
		cmtool[i] = (*this)[i] * d;
	}
	cmtool.noneNegativeZero();
	return cmtool;
}
//�������
CMatrix268 CMatrix268::operator*(CMatrix268& p)
{
	CMatrix268 cmtool;
	//����һ����ά���鷽����˵Ĳ���
	float cm1[4][4], cm2[4][4];
	//����ѭ������������ֵ�浽������
	for (int i = 0, j = 0, k = 0; i < 16; i++, j++)
	{
		cm1[j][k] = (*this)[i];
		cm2[j][k] = p[i];
		//ÿ����һ�У��б��0���б�����
		if (j == 3)
		{
			j = -1;
			k++;
		}
	}
	//��ʼ���
	for (int i = 0, j = 0, k = 0; i < 16; i++)
	{
		float ftool = 0.;
		for (int l = 0; l < 4; l++)
		{
			ftool += cm1[j][l] * cm2[l][k];
		}
		j++;
		//ÿ�˳�һ�е�ֵ���б��0���б�����
		if (j == 4)
		{
			j = 0;
			k++;
		}
		cmtool[i] = ftool;
	}
	cmtool.noneNegativeZero();
	return cmtool;
}
//�������������
CVector268 CMatrix268::vecMul(CVector268& p)
{
	CVector268 cvtool;
	//����һ����ά���鷽����˵Ĳ���
	float cm1[4][4];
	for (int i = 0, j = 0, k = 0; i < 16; i++, j++)
	{
		cm1[j][k] = (*this)[i];
		//ÿ����һ�У��б��0���б�����
		if (j == 3)
		{
			j = -1;
			k++;
		}
	}
	//��ʼ���
	for (int i = 0; i < 3; i++)
	{
		float ftool = 0.;
		for (int l = 0; l < 3; l++)
		{
			ftool += cm1[i][l] * p[l];
		}

		cvtool[i] = ftool;
	}

	return cvtool;
}
//������λ�����
CVector268 CMatrix268::posMul(CVector268& p)
{
	CVector268 cvtool;
	//����һ��һά�����������
	float cv[4];
	for (int i = 0; i < 3; i++) { cv[i] = p[i]; };
	cv[3] = 1;
	//����һ����ά���鷽����˵Ĳ���
	float cm1[4][4];

	for (int i = 0, j = 0, k = 0; i < 16; i++, j++)
	{
		cm1[j][k] = (*this)[i];
		//ÿ����һ�У��б��0���б�����
		if (j == 3)
		{
			j = -1;
			k++;
		}
	}
	//��ʼ���
	for (int i = 0; i < 3; i++)
	{
		float ftool = 0.;
		for (int l = 0; l < 4; l++)
		{
			ftool += cm1[i][l] * cv[l];
		}

		cvtool[i] = ftool;
	}

	return cvtool;
}
//����Ϊ��ת����
void CMatrix268::SetRotate(float seta, CVector268 axis)
{
	seta *= (PI / 180);
	(*this)[3] = 0; (*this)[7] = 0; (*this)[11] = 0;
	(*this)[12] = 0; (*this)[13] = 0; (*this)[14] = 0;
	(*this)[15] = 1;
	axis.Normalize();
	(*this)[0] = axis[0] * axis[0] * (1 - cos(seta)) + cos(seta);
	(*this)[1] = axis[0] * axis[1] * (1 - cos(seta)) + axis[2] * sin(seta);
	(*this)[2] = axis[0] * axis[2] * (1 - cos(seta)) - axis[1] * sin(seta);

	(*this)[4] = axis[0] * axis[1] * (1 - cos(seta)) - axis[2] * sin(seta);
	(*this)[5] = axis[1] * axis[1] * (1 - cos(seta)) + cos(seta);
	(*this)[6] = axis[2] * axis[1] * (1 - cos(seta)) + axis[0] * sin(seta);

	(*this)[8] = axis[0] * axis[2] * (1 - cos(seta)) + axis[1] * sin(seta);
	(*this)[9] = axis[1] * axis[2] * (1 - cos(seta)) - axis[0] * sin(seta);
	(*this)[10] = axis[2] * axis[2] * (1 - cos(seta)) + cos(seta);

}
//����Ϊƽ�ƾ���
void CMatrix268::SetTrans(CVector268 trans)
{
	(*this)[0] = 1; (*this)[4] = 0; (*this)[8] = 0; (*this)[12] = trans[0];
	(*this)[1] = 0; (*this)[5] = 1; (*this)[9] = 0; (*this)[13] = trans[1];
	(*this)[2] = 0; (*this)[6] = 0; (*this)[10] = 1; (*this)[14] = trans[2];
	(*this)[3] = 0; (*this)[7] = 0; (*this)[11] = 0; (*this)[15] = 1;

}
//����Ϊ���ž���
void CMatrix268::SetScale(CVector268 p)
{

	(*this)[0] = p[0]; (*this)[4] = 0; (*this)[8] = 0; (*this)[12] = 0;
	(*this)[1] = 0; (*this)[5] = p[1]; (*this)[9] = 0; (*this)[13] = 0;
	(*this)[2] = 0; (*this)[6] = 0; (*this)[10] = p[2]; (*this)[14] = 0;
	(*this)[3] = 0; (*this)[7] = 0; (*this)[11] = 0; (*this)[15] = 1;

	/*float k = p.len();
	(*this)[3] = 0; (*this)[7] = 0; (*this)[11] = 0;
	(*this)[12] = 0; (*this)[13] = 0; (*this)[14] = 0;
	(*this)[15] = 1;

	(*this)[0] = p[0] * p[0] * (k - 1) + 1;
	(*this)[1] = p[0] * p[1] * (k - 1);
	(*this)[2] = p[0] * p[2] * (k - 1);

	(*this)[4] = p[0] * p[1] * (k - 1);
	(*this)[5] = p[1] * p[1] * (k - 1) + 1;
	(*this)[6] = p[1] * p[2] * (k - 1);

	(*this)[8] = p[0] * p[2] * (k - 1);
	(*this)[9] = p[1] * p[2] * (k - 1);
	(*this)[10] = p[2] * p[2] * (k - 1) + 1;*/

}
//��������,�ɹ���������ʽ��ֵ�����򷵻�0
float CMatrix268::Inverse()
{
	float temp = this->GetDet();
	if (temp == 0) return 0;
	else
	{
		const CMatrix268 cmtool = GetInverse();
		*this = cmtool;
		return  temp;
	}

}
//���������
CMatrix268 CMatrix268::GetInverse()
{
	float temp = this->GetDet();
	CMatrix268 cmtool;
	//����һ����ά���鷽����˵Ĳ���
	float cm1[4][4];
	//����ѭ������������ֵ�浽������
	for (int i = 0, j = 0, k = 0; i < 16; i++, j++)
	{
		cm1[j][k] = (*this)[i];

		//ÿ����һ�У��б��0���б�����
		if (j == 3)
		{
			j = -1;
			k++;
		}
	}
	//��ʼ�������

	cmtool[0] = (
		//���׾�һ��һ��
		(cm1[1][1] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[1][2] * (cm1[2][3] * cm1[3][1] - cm1[2][1] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1]))) / temp;

	cmtool[1] = -(
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[1][2] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][0] * cm1[3][2] - cm1[2][2] * cm1[3][0]))
		) / temp;

	cmtool[2] = (
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][1] * cm1[3][3] - cm1[2][3] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[1][1] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))
		) / temp;


	cmtool[3] = -(
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[1][1] * (cm1[2][2] * cm1[3][0] - cm1[2][0] * cm1[3][2])) +
		//���׾�һ������
		(cm1[1][2] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))
		) / temp;

	cmtool[4] = -(
		//���׾�һ��һ��
		(cm1[0][1] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[2][3] * cm1[3][1] - cm1[2][1] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1]))) / temp;

	cmtool[5] = (
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[2][0] * cm1[3][2] - cm1[2][2] * cm1[3][0]))
		) / temp;

	cmtool[6] = -(
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[2][1] * cm1[3][3] - cm1[2][3] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))
		) / temp;


	cmtool[7] = (
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[2][2] * cm1[3][0] - cm1[2][0] * cm1[3][2])) +
		//���׾�һ������
		(cm1[0][2] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))) / temp;


	cmtool[8] = (
		//���׾�һ��һ��
		(cm1[0][1] * (cm1[1][2] * cm1[3][3] - cm1[3][2] * cm1[1][3])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[3][1] * cm1[1][3] - cm1[1][1] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][1] * cm1[3][2] - cm1[3][1] * cm1[1][2]))) / temp;

	cmtool[9] = -(
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][2] * cm1[3][3] - cm1[3][2] * cm1[1][3])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[3][0] * cm1[1][3] - cm1[1][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][0] * cm1[3][2] - cm1[1][2] * cm1[3][0]))
		) / temp;

	cmtool[10] = (
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][1] * cm1[3][3] - cm1[1][3] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[1][3] * cm1[3][0] - cm1[1][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][0] * cm1[3][1] - cm1[1][1] * cm1[3][0]))
		) / temp;


	cmtool[11] = -(
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][1] * cm1[3][2] - cm1[1][2] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[1][2] * cm1[3][0] - cm1[1][0] * cm1[3][2])) +
		//���׾�һ������
		(cm1[0][2] * (cm1[1][0] * cm1[3][1] - cm1[1][1] * cm1[3][0]))
		) / temp;

	cmtool[12] = -(
		//���׾�һ��һ��
		(cm1[0][1] * (cm1[1][2] * cm1[2][3] - cm1[2][2] * cm1[1][3])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[1][3] * cm1[2][1] - cm1[1][1] * cm1[2][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][1] * cm1[2][2] - cm1[2][1] * cm1[1][2]))) / temp;

	cmtool[13] = (
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][2] * cm1[2][3] - cm1[2][2] * cm1[1][3])) +
		//���׾�һ�ж���
		(cm1[0][2] * (cm1[1][3] * cm1[2][0] - cm1[1][0] * cm1[2][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][0] * cm1[2][2] - cm1[2][0] * cm1[1][2]))
		) / temp;

	cmtool[14] = -(
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][1] * cm1[2][3] - cm1[2][1] * cm1[1][3])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[1][3] * cm1[2][0] - cm1[1][0] * cm1[2][3])) +
		//���׾�һ������
		(cm1[0][3] * (cm1[1][0] * cm1[2][1] - cm1[2][0] * cm1[1][1]))
		) / temp;


	cmtool[15] = (
		//���׾�һ��һ��
		(cm1[0][0] * (cm1[1][1] * cm1[2][2] - cm1[2][1] * cm1[1][2])) +
		//���׾�һ�ж���
		(cm1[0][1] * (cm1[2][0] * cm1[1][2] - cm1[2][2] * cm1[1][0])) +
		//���׾�һ������
		(cm1[0][2] * (cm1[1][0] * cm1[2][1] - cm1[1][1] * cm1[2][0]))
		) / temp;

	cmtool.noneNegativeZero();
	return cmtool;
}
//��������ʽ��ֵ
float CMatrix268::GetDet()
{
	float det1 = 0.;
	//����һ����ά���鷽����˵Ĳ���
	float cm1[4][4];
	//����ѭ������������ֵ�浽������
	for (int i = 0, j = 0, k = 0; i < 16; i++, j++)
	{
		cm1[j][k] = (*this)[i];
		//ÿ����һ�У��б��0���б�����
		if (j == 3)
		{
			j = -1;
			k++;
		}
	}
	//��ʼ��������ʽ����ֵ
	//һ��һ��
	float temp = 0.;
	temp += cm1[0][0] * (
		//���׾�һ��һ��
		(cm1[1][1] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[1][2] * (cm1[2][3] * cm1[3][1] - cm1[2][1] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1]))
		);

	//һ�ж���
	temp -= cm1[0][1] * (
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][2] * cm1[3][3] - cm1[2][3] * cm1[3][2])) +
		//���׾�һ�ж���
		(cm1[1][2] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][0] * cm1[3][2] - cm1[2][2] * cm1[3][0]))
		);

	//һ������
	temp += cm1[0][2] * (
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][1] * cm1[3][3] - cm1[2][3] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[1][1] * (cm1[2][3] * cm1[3][0] - cm1[2][0] * cm1[3][3])) +
		//���׾�һ������
		(cm1[1][3] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))
		);

	//һ������
	temp -= cm1[0][3] * (
		//���׾�һ��һ��
		(cm1[1][0] * (cm1[2][1] * cm1[3][2] - cm1[2][2] * cm1[3][1])) +
		//���׾�һ�ж���
		(cm1[1][1] * (cm1[2][2] * cm1[3][0] - cm1[2][0] * cm1[3][2])) +
		//���׾�һ������
		(cm1[1][2] * (cm1[2][0] * cm1[3][1] - cm1[2][1] * cm1[3][0]))
		);

	det1 = temp;

	return det1;
}

//�����и�0תΪ0
void CMatrix268::noneNegativeZero()
{
	for (int i = 0; i < 16; i++)
	{
		if (((*this)[i] - 0 >-5e-10) && ((*this)[i] - 0 < 5e-10)) (*this)[i] = 0.00;
	}

}

CEuler268 CMatrix268::ToEuler()
{
	CEuler268 tool;
	tool.p = 180/3.1415926*asin(-m12);
	tool.h = 180 / 3.1415926*atan2(m02, m22);
	tool.b = 180 / 3.1415926*atan2(m10, m11);
	
	if (fabs(tool.p - 90)<1e-5 || fabs(tool.p + 90)<1e-5)
	{
		tool.h = 180 / 3.1415926*atan2(-m20, m00);
		tool.b = 0;
	}

	return tool;
}

CQuaternion268 CMatrix268::ToQuaternion()
{
	
	CQuaternion268  tool;
	//�˼��㷽����������⣬δ֪ԭ��

	/*tool.w = sqrt(m00 + m11 + m22 + 1) / 2.;
	tool.x = sqrt(m00 - m11 - m22 + 1) / 2.;
	tool.y = sqrt(-m00 + m11 - m22 + 1) / 2.;
	tool.z = sqrt(-m00 - m11 + m22 + 1) / 2.;

	int flag = 1;
	float max = tool.w;
	if (tool.x > max) {
		max = tool.x;
		flag = 2;
	}

	if (tool.y > max) {
		max = tool.y;
		flag = 3;
	}
	if (tool.z > max)
	{
		max = tool.z;
		flag = 4;
	}
	
	

	switch (flag)
	{
	case 1:
		tool.x = (m21 - m12) / (4 * tool.w);
		tool.y = (m02 - m20) / (4 * tool.w);
		tool.z = (m10 - m01) / (4 * tool.w);
	
	case 2:
		tool.w = (m21 - m12) / (4 * tool.x);
		tool.y = (m10 + m01) / (4 * tool.x);
		tool.z = (m02 + m20) / (4 * tool.x);

	case 3:
		tool.w = (m02 - m20) / (4 * tool.y);
		tool.x = (m10 + m01) / (4 * tool.y);
		tool.z = (m21 + m12) / (4 * tool.y);

	case 4:
		tool.w = (m10 - m01) / (4 * tool.z);
		tool.x = (m02 + m20) / (4 * tool.z);
		tool.y = (m21 + m12) / (4 * tool.z);
	}*/
	tool = this->ToEuler().ToQuaternion();

	return tool;
}
