#include "stdafx.h"
#include "CVector268.h"
#include "CMatrix268.h"
#include "CEuler268.h"
#include "CQuaternion268.h"
#include "math.h"

//默认构造器
CEuler268::CEuler268()
{
	CEuler268(0, 0, 0);
}

//含参构造器
CEuler268::CEuler268(float ch, float cp, float cb)
{
	h = ch;
	p = cp;
	b = cb;
}

void CEuler268::Set(float fh, float fp, float fb)
{
	h = fh;
	p = fp;
	b = fb;
}

CVector268 CEuler268::ToVector()
{
	CVector268 tool;
	tool = this->ToMatrix().vecMul(CVector268(0, 0, -1));
	return tool;
}

CMatrix268 CEuler268::ToMatrix()
{
	CMatrix268 toolx;
	CMatrix268 tooly;
	CMatrix268 toolz;
	CMatrix268 tool;

	tooly.SetRotate(h, CVector268(0, 1, 0));
	toolx.SetRotate(p, CVector268(1, 0, 0));
	toolz.SetRotate(b, CVector268(0, 0, 1));

	tool = tooly*toolx;
	tool = tool*toolz;

	return tool;
}

CQuaternion268 CEuler268::ToQuaternion()
{
	CQuaternion268 tool;
	this->Normal();
	
	tool.w = cos((3.1415926 / 180)  * h / 2)*cos(3.1415926 / 180 * p / 2)*cos(3.1415926 / 180 * b / 2) +
		sin(3.1415926 / 180 * h / 2)*sin(3.1415926 / 180 * p / 2)*sin(3.1415926 / 180 * b / 2);

	tool.x = cos(3.1415926 / 180 * h / 2)*sin(3.1415926 / 180 * p / 2)*cos(3.1415926 / 180 * b / 2) +
		sin(3.1415926 / 180 * h / 2)*cos(3.1415926 / 180 * p / 2)*sin(3.1415926 / 180 * b / 2);

	tool.y = sin(3.1415926 / 180 * h / 2)*cos(3.1415926 / 180 * p / 2)*cos(3.1415926 / 180 * b / 2) -
		cos(3.1415926 / 180 * h / 2)*sin(3.1415926 / 180 * p / 2)*sin(3.1415926 / 180 * b / 2);

	tool.z = cos(3.1415926 / 180 * h / 2)*cos(3.1415926 / 180 * p / 2)*sin(3.1415926 / 180 * b / 2) -
		sin(3.1415926 / 180 * h / 2)*sin(3.1415926 / 180 * p / 2)*cos(3.1415926 / 180 * b / 2);

	return tool;

}

void CEuler268::Normal()
{

	while (p > 180) p -= 360;
	while (p < -180) p += 360;
	if (p > 90)
	{
		p = 180 - p;
		h += 180;
		b += 180;
	}
	else if (p < -90)
	{
		p = -180 - p;
		h += 180;
		b += 180;
	}


	
	//万向锁
	if (fabs(p) - 90 > 1e-5)
	{
		h += b;
		b = 0;
	}
	
	while (h > 180) h -= 360;
	while (h < -180) h += 360;
	while (b > 180) b -= 360;
	while (b < -180) b += 360;

}

CEuler268::~CEuler268()
{
}

