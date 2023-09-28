#include "stdafx.h"
#include "CQuaternion268.h"
#include "CVector268.h"
#include "CMatrix268.h"
#include "CEuler268.h"
#include "math.h"


CQuaternion268::CQuaternion268()
{
	CQuaternion268(0, 0, 1, 0);
}


CQuaternion268::~CQuaternion268()
{
}

CQuaternion268::CQuaternion268(float cx, float cy, float cz, float cw)
{
	x = cx;
	y = cy;
	z = cz;
	w = cw;
}

void CQuaternion268::Set(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

CEuler268 CQuaternion268::ToEuler()
{
	CEuler268 tool;
	this->Normalize();
	tool.p = 180 / 3.1415926*asin(2 * w*x - 2 * y*z);
	if (fabs(cos(tool.p)) < 1e-6)
	{
		tool.h = 180 / 3.1415926*atan2(2 * w*y - 2 * z*x, 1 - 2 * y*y - 2 * z*z);
		tool.b = 0;
	}
	else
	{
		tool.h = 180 / 3.1415926*atan2(2 * w*y + 2 * z*x, 1 - 2 * y*y - 2 * x*x);
		tool.b = 180 / 3.1415926*atan2(2 * x*y + 2 * w*z, 1 - 2 * z*z - 2 * x*x);
	}

	return tool; 
}

CMatrix268 CQuaternion268::ToMatrix()
{
	CMatrix268 tool;
	this->Normalize();
	tool.m00 = 1 - 2 * y*y - 2 * z*z;
	tool.m10 = 2 * x*y + 2 * w*z;
	tool.m20 = 2 * x*z - 2 * w*y;
	tool.m30 = 0;

	tool.m01 = 2 * x*y - 2 * w*z;
	tool.m11 = 1 - 2 * x*x - 2 * z*z;
	tool.m21 = 2 * y*z + 2 * w*x;
	tool.m31 = 0;

	tool.m02 = 2 * x*z + 2 * w*y;
	tool.m12 = 2 * y*z - 2 * w*x;
	tool.m22 = 1 - 2 * x*x - 2 * y*y;
	tool.m32 = 0;

	tool.m03 = 0;
	tool.m13 = 0;
	tool.m23 = 0;
	tool.m33 = 1;
	return tool;
}

void CQuaternion268::SetAngle(float angle, CVector268 axis)
{
	w = angle;
	x = axis.x;
	y = axis.y;
	z = axis.z;
}

CQuaternion268 & CQuaternion268::operator=(const CQuaternion268 & p)
{
	// TODO: 在此处插入 return 语句
	CQuaternion268 temp(p.x,p.y,p.z,p.w);
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
	this->w = p.w;

	return temp;
}

CQuaternion268 CQuaternion268::operator+(const CQuaternion268 & p)
{
	CQuaternion268 temp;
	temp.x = this->x + p.x;
	temp.y = this->y + p.y;
	temp.z = this->z + p.z;
	temp.w = this->w + p.w;

	return temp;
}

CQuaternion268 CQuaternion268::operator*(float data)
{
	CQuaternion268 tool;
	tool.x = x*data;
	tool.y = y*data;
	tool.z = z*data;
	tool.w = w*data;

	return tool;
}

CQuaternion268 CQuaternion268::operator*(const CQuaternion268 & p)
{
	CQuaternion268 tool;
	tool.w = w*p.w - x*p.x - y*p.y - z*p.z;
	tool.x = w*p.x + x*p.w - y*p.z + z*p.y;
	tool.y = w*p.y + y*p.w - z*p.x + x*p.z;
	tool.z = w*p.z + z*p.w - x*p.y + y*p.x;


	return tool;
}

float CQuaternion268::dotMul(const CQuaternion268 & p)
{
	float res = 0.0f;
	res = w*p.w + x*p.x + y*p.y + z*p.z;

	return res;
}

float CQuaternion268::len()
{
	float res = 0.0f;
	res = sqrt(w*w + x*x + y*y + z*z);
	return res;
}

bool CQuaternion268::Normalize()
{
	float le = this->len();
	if (le == 0) return false;
	else
	{
		w /= le;
		x /= le;
		y /= le;
		z /= le;
		return true;
	}
	
}

CQuaternion268 & CQuaternion268::Inverse()
{
	float len2 = w*w + x*x + y*y + z*z;
	x *= -1;
	y *= -1;
	z *= -1;

	for (int i = 0; i < 4; i++)
	{
		(*this)[i] /= len2;
	}

	return *this;
}

CQuaternion268 CQuaternion268::GetInverse()
{
	CQuaternion268 tool;
	float len2 = w*w + x*x + y*y + z*z;
	for (int i = 0; i < 4; i++)
	{
		tool[i] = (*this)[i];
		tool[i] /= len2;
	}
	
	tool.x *= -1;
	tool.y *= -1;
	tool.z *= -1;

	return tool;
}

CQuaternion268 CQuaternion268::Div(const CQuaternion268 & b)
{
	CQuaternion268 tool;
	CQuaternion268 c1 = b;

	tool = c1.GetInverse() * (*this);
	return tool;
}

void CQuaternion268::GetAngle(float & angle, CVector268 & axis)
{
	CQuaternion268 temp = *this;

	temp.Normalize();
	float seta = acos(temp.w);

	angle = seta*2*180 / 3.14159;
	CVector268 tool(temp.x/sin(seta), temp.y / sin(seta), temp.z / sin(seta));
	axis = tool;
}

CQuaternion268 CQuaternion268::Slerp(const CQuaternion268 & Vend, float t)
{
	CQuaternion268 tool;
	CQuaternion268 tar = Vend;
	CQuaternion268 beg = *this;
	beg.Normalize();
	tar.Normalize();
	CQuaternion268 mid = beg.GetInverse() * tar;
	CQuaternion268 mindt = mid.getPower(t);

	tool = beg * mindt;
	return tool;
}

void CQuaternion268::Slerp(const CQuaternion268 & Vend, int n, float * t, CQuaternion268 * Result)
{
	for (int i = 0; i < n; i++)
	{
		Result[i] = Slerp(Vend, t[i]);

	}


}

CQuaternion268 CQuaternion268::getPower(float t)
{
	CQuaternion268 tool;
	float seta = acos(w);
	tool.w = cos(seta * t);
	if (fabs(sin(seta) - 0) < 1e-4)
	{
		tool.x = 0.;
		tool.y = 0.;
		tool.z = 0.;
	}
	else
	{
		tool.x = sin(t*seta)*x / sin(seta);
		tool.y = sin(t*seta)*y / sin(seta);
		tool.z = sin(t*seta)*z / sin(seta);
	}
	


	return tool;
}
