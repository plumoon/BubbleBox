// GraphicsWork1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "math.h"
#include "ctime"

#include "glut.h"

#include "string.h"
#include "sstream"
#include "fstream"
#include "vector"

#include "CMatrix268.h"
#include "CVector268.h"
#include "CEuler268.h"
#include "CQuaternion268.h"

using namespace std;
//char ch = '	'; //test.txt 中两向量之间的分隔符为‘	’或‘ ’

//绘制图形相关全局变量/常量

//球的相关信息在初始时随机产生一次
//视点位置和方向
int flag = 0,		ifbond=0,bi=-1,ifdynView=0;
//是否从自由切到欧拉 是否绑定到球上 绑定球的编号	是否在动态插值，及插值参数 //大于0 上帝到球，小于0 球到上帝
float mspeed = 0.5, rspeed = 0.3;
bool keydown[16];
char keyb[16] = { 'a','d','w','s','q','e','j','l','i','k','u','o','[',']','{','}' };
CVector268 mymove(50, 120, 200);//平移
CEuler268 myrotate(0, -25, 0);//旋转
CVector268 ballmove(0, 0, 0);//球内部平移
CEuler268 ballro(0, 0, 0);//球内部旋转
CMatrix268 ballIe(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//球内部逆矩阵
CMatrix268 g_IEyeMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), g_EyeMat(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 );
CMatrix268 myview(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//球内部旋转自由漫游
int mode = 1;//0 为自由漫游模式，1为欧拉角模式
CVector268 ball[100][2]; //储存球的相关信息 第一列代表位置，第二列代表速度
float ballColro[100][3] = { 0.,0.,0. };//储存球的颜色信息
//绑定球的视点矩阵
CMatrix268 bball;
CQuaternion268 qbeg,qmid,qtar;//视点动态变化过程四元数
CVector268 pbeg, pmid,ptar;//视点动态变化过程位置
CEuler268 ebeg, emid, etar;//视点动态变化过程欧拉角
CMatrix268 cmid;//视点动态变化矩阵
const GLfloat Pi = 3.1415926536f; //π值

								  //float seta = 0.f;//与时间有关变量
float xseta = 0.f, yseta = 0.f;//绕轴转动角度

							   //函数声明
							   //计算相关
void stringSplit(string str, const const char split, vector<string> &result);//字符串分割函数
CVector268 strToV(string str);// 将代表向量的字符串实例化为向量
CMatrix268 strToM(string str);// 将代表矩阵的字符串实例化为矩阵
CEuler268 strToE(string str);// 将代表向量的字符串实例化为欧拉角
CQuaternion268 strToQ(string str);// 将代表矩阵的字符串实例化为四元数
double viewmatrix[16], modelviewmatrix[16]; //鼠标取点矩阵

void numericalOpr(ifstream &p, ofstream &q);  //依据文件内容进行处理
void priEu(ofstream &q, CEuler268 p);//输出欧拉角
void priQu(ofstream &q, CQuaternion268 p);//输出四元数
void priMa(ofstream &q, CMatrix268 p);//输出矩阵
void priVe(ofstream &q, CVector268 p);//输出向量
						 
										   //绘图相关
void dropCoordinate(float length);//绘制坐标轴	
void ballelements();//启动程序时随机产生球的信息，仅执行一次
void dropball();  //绘制球		
void drawBox();//绘制六面体盒子
void myTimerFunc(int val);//时间函数
void myKeymove(char key);//键盘执行函数
void myKeyboard();//键盘响应函数
void myKeyboardFunc(unsigned char key, int x, int y);//键盘按下信息更新
void myKeyboardUpFunc(unsigned char key, int x, int y);//键盘释放信息更新
void mySpecialFunc(int key, int x, int y);//特殊键盘控制
void myMouseFunc(int button, int state, int x, int y);//鼠标取点，视点绑定
void bonBall();//视点绑定到球
void SetView();//视点矩阵设置		  
void dynView(); //视点动态变化
void SetRC();//绘制前准备 初始化设置OpenGL绘制状态
void myDisplay(void);//绘制
void myReshape(int w, int h);//设置视图大小和投影方式

							 //函数定义

							 //字符串分割函数
void stringSplit(string str, const const char split, vector<string> &result)
{
	//利用stringstream实现根据特定字符分割字符串的功能
	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, split))
	{
		result.push_back(buffer);
	}
}

//将代表向量的字符串实例化为向量
CVector268 strToV(string str)
{
	CVector268 cv1;
	int flag = 0;

	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, ','))
	{
		cv1[flag] = (float)stof(buffer);
		flag++;
	}
	return cv1;
}

//将代表矩阵的字符串实例化为矩阵
CMatrix268 strToM(string str)
{
	CMatrix268 cm1;
	int flag = 0;

	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, ','))
	{
		cm1[flag] = (float)stof(buffer);
		flag++;
	}
	return cm1;
}

// 将代表向量的字符串实例化为欧拉角
CEuler268 strToE(string str)
{
	CEuler268 ce1;
	int flag = 0;

	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, ','))
	{
		ce1[flag] = (float)stof(buffer);
		flag++;
	}
	return ce1;
}

// 将代表向量的字符串实例化为四元数
CQuaternion268 strToQ(string str)
{
	CQuaternion268 cq1;
	int flag = 0;

	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, ','))
	{
		cq1[flag] = (float)stof(buffer);
		flag++;
	}
	return cq1;
}

//输出欧拉角
void priEu(ofstream &q, CEuler268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2];
}

//输出四元数
void priQu(ofstream &q, CQuaternion268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2] << ','<< p[3];
}

//输出矩阵
void priMa(ofstream &q,CMatrix268 p)
{
	q << p[0];
	for (int i = 1; i < 16; i++)
	{
		q << ',' << p[i];
	}
}

//输出向量
void priVe(ofstream &q,CVector268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2];
}

//用函数实现作业要求的四元数和欧拉角处理
void numericalOpr(ifstream &p, ofstream &q)
{

	CVector268 cvtool1, cvtool2, cvr;
	CMatrix268 cmtool1, cmtool2, cmr;
	CEuler268 cetool1, cetool2, cer;
	CQuaternion268 cqtool1, cqtool2, cqr;

	float res = 0.f;
	char flag = '0';
	char ch = '	';
	char ch2 = ' ';
	vector<string> result, result1, result2;
	//char str[256]= "\0";
	//p.getline(str,256);
	string str1 = "\0", str2 = "\0", str3 = "\0";

	while (getline(p, str1))
	{
		result.clear();
		result1.clear();
		result2.clear();
		q.setf(ios::fixed);
		q.precision(2);
		
			
		switch (flag)
		{
		case '0':
			if (!str1.compare("欧拉角转换向量"))
			{
				flag = '1';
				break;
			}
			if (!str1.compare("向量转换欧拉角"))
			{
				flag = '2';
				break;
			}
			if (!str1.compare("欧拉角转换四元数"))
			{
				flag = '3';
				break;
			}
			if (!str1.compare("四元数转换欧拉角"))
			{
				flag = '4';
				break;
			}
			if (!str1.compare("欧拉角转换矩阵"))
			{
				flag = '5';
				break;
			}
			if (!str1.compare("矩阵转换欧拉角"))
			{
				flag = '6';
				break;
			}
			if (!str1.compare("矩阵转换四元数"))
			{
				flag = '7';
				break;
			}
			if (!str1.compare("四元数转换矩阵"))
			{
				flag = '8';
				break;
			}
			if (!str1.compare("欧拉角标准化"))
			{
				flag = '9';
				break;
			}
			if (!str1.compare("四元数单位化"))
			{
				flag = 'a';
				break;
			}
			if (!str1.compare("四元数相乘"))
			{
				flag = 'b';
				break;
			}
			if (!str1.compare("四元数求差"))
			{
				flag = 'c';
				break;
			}
			if (!str1.compare("四元数点乘"))
			{
				flag = 'd';
				break;
			}
			if (!str1.compare("四元数求逆"))
			{
				flag = 'e';
				break;
			}
			if (!str1.compare("四元数求角度和旋转轴"))
			{
				flag = 'f';
				break;
			}
			if (!str1.compare("四元数插值"))
			{
				flag = 'g';
				break;
			}
			break;
		case '1':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cetool1 = strToE(str2);
			cvr = cetool1.ToVector();
			q << "欧拉角转换向量" << endl;
			//输出欧拉角
			priEu(q, cetool1);q << '\t';
			//输出结果向量
			priVe(q, cvr);q << endl;

			flag = '0';
			break;

		case '2':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cvtool1 = strToV(str2);
			cer = cvtool1.ToEuler();
			q << "向量转换欧拉角" << endl;
			//输出向量
			priVe(q, cvtool1);q << '\t';
			//输出结果欧拉角
			priEu(q, cer); q << endl;
		
			flag = '0';
			break;

		case '3':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cetool1 = strToE(str2);
			cqr = cetool1.ToQuaternion();
			q << "欧拉角转换四元数" << endl;
			//输出欧拉角
			priEu(q, cetool1);q <<'\t';
			//输出结果四元数
			priQu(q, cqr); q << endl;

			flag = '0';
			break;

		case '4':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);
		
			cqtool1 = strToQ(str2);
			cer = cqtool1.ToEuler();
			q << "四元数转换欧拉角" << endl;
			//输出四元数
			priQu(q, cqtool1);
			q <<'\t';
			//输出结果欧拉角
			priEu(q, cer); q << endl;

			flag = '0';
			break;

		case '5':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cetool1 = strToE(str2);
			cmr = cetool1.ToMatrix();

			q << "欧拉角转换矩阵" << endl;
			//输出欧拉角
			priEu(q, cetool1); q << '\t';
			//输出结果矩阵
			priMa(q, cmr);q << endl;

			flag = '0';
			break;

		case '6':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cmtool1 = strToM(str2);
			cer = cmtool1.ToEuler();
			q << "矩阵转换欧拉角" << endl;

			//输出矩阵
			priMa(q, cmtool1); q << '\t'; 
			//输出结果欧拉角
			priEu(q, cer); q << endl;

			flag = '0';
			break;

		case '7':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cmtool1 = strToM(str2);
			cqr = cmtool1.ToQuaternion();
			q << "矩阵转换四元数" << endl;

			//输出矩阵
			priMa(q, cmtool1);q << '\t';
			//输出四元数
			priQu(q, cqr);q << endl;

			flag = '0';
			break;

		case '8':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cqtool1 = strToQ(str2);
			cmr = cqtool1.ToMatrix();
			q << "四元数转换矩阵" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			//输出矩阵
			priMa(q, cmr); q << endl;

			flag = '0';
			break;

		case '9':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cetool1 = strToE(str2);
			cer = cetool1;
			cer.Normal();
			q << "欧拉角标准化" << endl;

			//输出原欧拉角
			priEu(q, cetool1); q << '\t';
			//输出结果
			priEu(q, cer); q << endl;

			flag = '0';
			break;

		case 'a':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cqtool1 = strToQ(str2);
			cqr = cqtool1;
			cqr.Normalize();
			q << "四元数单位化" << endl;

			//输出原四元数
			priQu(q, cqtool1); q << '\t';
			//输出结果
			priQu(q, cqr); q << endl;

			flag = '0';
			break;

		case 'b':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);
			str3 = result.at(1);

			cqtool1 = strToQ(str2);
			cqtool2 = strToQ(str3);
			cqr = cqtool1*cqtool2;
			q << "四元数相乘" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//输出结果
			priQu(q, cqr); q << endl;

			flag = '0';
			break;

		case 'c':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);
			str3 = result.at(1);

			cqtool1 = strToQ(str2);
			cqtool2 = strToQ(str3);
			cqr = cqtool2.Div(cqtool1);
			q << "四元数求差" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//输出结果
			priQu(q, cqr); q << endl;

			flag = '0';
			break;

		case 'd':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);
			str3 = result.at(1);

			cqtool1 = strToQ(str2);
			cqtool2 = strToQ(str3);
			res = cqtool1.dotMul(cqtool2);
			q << "四元数点乘" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//输出结果
			q << res << endl;

			flag = '0';
			break;

		case 'e':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cqtool1 = strToQ(str2);
			cqr = cqtool1.GetInverse();
			q << "四元数求逆" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			//输出结果四元数
			priQu(q, cqr); q << endl;

			flag = '0';
			break;

		case 'f':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);

			cqtool1 = strToQ(str2);
			cqtool1.GetAngle(res, cvr);
			q << "四元数求角度和旋转轴" << endl;

			//输出四元数
			priQu(q, cqtool1); q << '\t';
			//输出结果
			q << res << '\t';
			priVe(q, cvr); q << endl;

			flag = '0';
			break;

		case 'g':
			stringSplit(str1, ch, result1);
			stringSplit(str1, ch2, result2);
			(result1.size() > result2.size()) ? result.swap(result1) : result.swap(result2);
			str2 = result.at(0);
			str3 = result.at(1);
			res = (float)stof(result.at(2));

			cqtool1 = strToQ(str2);
			cqtool2 = strToQ(str3);
			cqr = cqtool1.Slerp(cqtool2, res);
			q << "四元数插值" << endl;

			//输出四元数和插值参数
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			q << res << '\t';
			//输出结果
			priQu(q, cqr); q << endl;

			flag = '0';
			break;
		}
	}


}

//绘制坐标轴
void dropCoordinate(float length)
{

	// x 轴线 红色
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(length, 1.f, 1.f);

	// x箭头
	glVertex3f(length, 1.f, 1.f);
	glVertex3f((0.98f)*length, (0.01f)*length + 1.f, 1.f);

	glVertex3f(length, 1.f, 1.f);
	glVertex3f((0.98f)*length, (-0.01)*length + 1.f, 1.f);
	glEnd();


	// y 轴线 绿色
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, length, 1.f);

	// y箭头
	glVertex3f(1.f, length, 1.f);
	glVertex3f((0.01f)*length + 1.f, (0.98f)*length, 1.f);

	glVertex3f(1.f, length, 1.f);
	glVertex3f((-0.01f)*length + 1.f, (0.98f)*length, 1.f);
	glEnd();


	// z 轴线蓝色
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, length);

	// z 箭头
	glVertex3f(1.f, 1.f, length);
	glVertex3f(1.f, (0.01f)*length + 1.f, (0.98f)*length);

	glVertex3f(1.f, 1.f, length);
	glVertex3f(1.f, (-0.01f)*length + 1.f, (0.98f)*length);
	glEnd();

}

//绘制六面体盒子
void drawBox()
{
	//六个面四点的坐标 逆时针排列
	GLfloat box[6][4][3];
	box[0][0][0] = 0.; box[0][0][1] = 0.; box[0][0][2] = 0.;
	box[0][1][0] = 0.; box[0][1][1] = 100.; box[0][1][2] = 0.;
	box[0][2][0] = 0.; box[0][2][1] = 100.; box[0][2][2] = 100.;
	box[0][3][0] = 0.; box[0][3][1] = 0.; box[0][3][2] = 100.;

	box[1][0][0] = 0.; box[1][0][1] = 0.; box[1][0][2] = 0.;
	box[1][1][0] = 0.; box[1][1][1] = 0.; box[1][1][2] = 100.;
	box[1][2][0] = 100.; box[1][2][1] = 0.; box[1][2][2] = 100.;
	box[1][3][0] = 100.; box[1][3][1] = 0.; box[1][3][2] = 0.;

	box[2][0][0] = 100.; box[2][0][1] = 0.; box[2][0][2] = 0.;
	box[2][1][0] = 100.; box[2][1][1] = 0.; box[2][1][2] = 100.;
	box[2][2][0] = 100.; box[2][2][1] = 100.; box[2][2][2] = 100.;
	box[2][3][0] = 100.; box[2][3][1] = 100.; box[2][3][2] = 0.;

	box[3][0][0] = 0.; box[3][0][1] = 0.; box[3][0][2] = 0.;
	box[3][1][0] = 100.; box[3][1][1] = 0.; box[3][1][2] = 0.;
	box[3][2][0] = 100.; box[3][2][1] = 100.; box[3][2][2] = 0.;
	box[3][3][0] = 0.; box[3][3][1] = 100.; box[3][3][2] = 0.;

	box[4][0][0] = 0.; box[4][0][1] = 0.; box[4][0][2] = 100.;
	box[4][1][0] = 0.; box[4][1][1] = 100.; box[4][1][2] = 100.;
	box[4][2][0] = 100.; box[4][2][1] = 100.; box[4][2][2] = 100.;
	box[4][3][0] = 100.; box[4][3][1] = 0.; box[4][3][2] = 100.;

	box[5][0][0] = 0.; box[5][0][1] = 100.; box[5][0][2] = 0.;
	box[5][1][0] = 100.; box[5][1][1] = 100.; box[5][1][2] = 0.;
	box[5][2][0] = 100.; box[5][2][1] = 100.; box[5][2][2] = 100.;
	box[5][3][0] = 0.; box[5][3][1] = 100.; box[5][3][2] = 100.;

	//六个墙的颜色不同
	GLfloat	wallcolor[6][3] = { { 1.0, 0.85, 0.73 }, {1.0, 0.55, 0.73},{ 1.0, 0.85, 0.33 },
	{ 0.8, 0.85, 0.73 },{ 0.6, 0.85, 0.73 },{ 0.4, 0.85, 0.73 } };


	glPushMatrix();
	//绘制前准备
								//glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
	glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式
	glFrontFace(GL_CCW);      // 设置逆时针方向为正面


	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		glColor3f(wallcolor[i][0], wallcolor[i][1], wallcolor[i][2]);//设置不同的颜色
		glVertex3f(box[i][0][0], box[i][0][1], box[i][0][2]);
		glVertex3f(box[i][1][0], box[i][1][1], box[i][1][2]);
		glVertex3f(box[i][2][0], box[i][2][1], box[i][2][2]);
		glVertex3f(box[i][3][0], box[i][3][1], box[i][3][2]);

	}
	glEnd();

	glPopMatrix();
}

//随机产生球的信息
void ballelements()
{
	//将键盘信息初始化为false
	for(int i =0;i<16;i++) keydown[i] = false;
	//产生100组随机数 每组6个数，位置取前三个,0~100，颜色取2、3、4，速度取后三个，0~0.1,球半径取4，小于13
	
	srand(unsigned(time(nullptr)));

	for (int i = 0; i < 100; i++)
	{
		float x = (rand() % 100)*1.0 + 0.5;
		float y = (rand() % 100)*1.0 + 0.5;
		float z = (rand() % 100)*1.0 + 0.5;
		float w = (rand() % 200)*1.0 / 2000.f;
		float u = (rand() % 200)*1.0 / 2000.f;
		float v = (rand() % 200)*1.0 / 2000.f;

		ball[i][0].x = x;
		ball[i][0].y = y;
		ball[i][0].z = z;
		/*ball[i][1].x = w;
		ball[i][1].y = u;
		ball[i][1].z = v;*/
		ball[i][1].x = w/2;
		ball[i][1].y = u/2;
		ball[i][1].z = v/2;
		/*ball[i][1].x = 0;
		ball[i][1].y = 0;
		ball[i][1].z = 0;*/

		GLfloat Rmax = (50 - fabsf(x - 50)) < (50 - fabsf(y - 50)) ? (50 - fabsf(x - 50)) : (50 - fabsf(y - 50));
		Rmax = Rmax > (50 - fabsf(z - 50)) ? (50 - fabsf(z - 50)) : Rmax;
		while (w > Rmax / 1000) { w -= Rmax / 20000; };//通过限制半径基本保证边界出现的球不会有一部分出现在墙外

		ballColro[i][0] = y / 100.f;
		ballColro[i][1] = z / 100.f;
		ballColro[i][2] = w / 0.1f;

	}

}

//绘制球
void dropball()
{
	//绘制球
	
	for (int i = 0; i < 100; i++)
	{
		//球的位置变化
		ball[i][0] = ball[i][0] + ball[i][1];
		float r = ballColro[i][2] * 13 / 0.8f;//球的半径
											  //判断球是否到达边界
		for (int j = 0; j < 3; j++)
		{
			if ((ball[i][0][j] + r) >= 100 || (ball[i][0][j] - r) <= 0)
			{
				ball[i][1][j] *= -1;//若某方向碰到墙壁，则此方向速度分量反向
			}
		}

		glColor3f(ballColro[i][0] * 1.2, ballColro[i][2] * 2, ballColro[i][1] * 0.9);
		glPushMatrix();
		glTranslatef(ball[i][0].x, ball[i][0].y, ball[i][0].z);
		glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
		glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式
		glutSolidSphere(r, 25, 25);
		glPopMatrix();

	}
}

//时间函数
void myTimerFunc(int val)
{
	myDisplay();
	myKeyboard();//多键盘响应执行函数
	
	glutTimerFunc(1, myTimerFunc, 0);
	
}

//键盘执行函数
void myKeymove(char key)
{
	bool bChange = false;
	switch (key)
	{
	case 'w':

		if (mode == 0)
		{
			
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, -mspeed, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
			
		}
		else
		{
			if (ifbond)
			{
				ballmove.x += ballIe[4] * mspeed;
				ballmove.y += ballIe[5] * mspeed;
				ballmove.z += ballIe[6] * mspeed;
			}
			else
			{
				mymove.x += g_IEyeMat[4] * mspeed;
				mymove.y += g_IEyeMat[5] * mspeed;
				mymove.z += g_IEyeMat[6] * mspeed;
			}
			
		}
		break;
	case 's':
		//my-=mspeed;	
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, mspeed, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballmove.x -= ballIe[4] * mspeed;
				ballmove.y -= ballIe[5] * mspeed;
				ballmove.z -= ballIe[6] * mspeed;
			}
			else
			{
				mymove.x -= g_IEyeMat[4] * mspeed;
				mymove.y -= g_IEyeMat[5] * mspeed;
				mymove.z -= g_IEyeMat[6] * mspeed;
			}
		}

		break;
	case 'a':
		//mx-=mspeed;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(mspeed, 0, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballmove.x -= ballIe[0] * mspeed;
				ballmove.y -= ballIe[1] * mspeed;
				ballmove.z -= ballIe[2] * mspeed;
			}
			else
			{
				mymove.x -= g_IEyeMat[0] * mspeed;
				mymove.y -= g_IEyeMat[1] * mspeed;
				mymove.z -= g_IEyeMat[2] * mspeed;
			}
			
		}

		break;
	case 'd':
		//mx+=mspeed;
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(-mspeed, 0, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballmove.x += ballIe[0] * mspeed;
				ballmove.y += ballIe[1] * mspeed;
				ballmove.z += ballIe[2] * mspeed;
			}
			else
			{
				mymove.x += g_IEyeMat[0] * mspeed;
				mymove.y += g_IEyeMat[1] * mspeed;
				mymove.z += g_IEyeMat[2] * mspeed;
			}
			
		}

		break;
	case 'q':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, 0, mspeed);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballmove.x -= ballIe[8] * mspeed;
				ballmove.y -= ballIe[9] * mspeed;
				ballmove.z -= ballIe[10] * mspeed;
			}
			else
			{
				mymove.x -= g_IEyeMat[8] * mspeed;
				mymove.y -= g_IEyeMat[9] * mspeed;
				mymove.z -= g_IEyeMat[10] * mspeed;
			}
			
		}
		//mz-=mspeed;

		break;
	case 'e':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(0, 0, -mspeed);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballmove.x += ballIe[8] * mspeed;
				ballmove.y += ballIe[9] * mspeed;
				ballmove.z += ballIe[10] * mspeed;
			}
			else
			{
				mymove.x += g_IEyeMat[8] * mspeed;
				mymove.y += g_IEyeMat[9] * mspeed;
				mymove.z += g_IEyeMat[10] * mspeed;
			}
			
		}


		break;
	case 'i':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 1, 0, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.p += rspeed;
			}
			else myrotate.p += rspeed;
			bChange = true;
		}
		break;
	case 'k':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 1, 0, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.p -= rspeed;
			}
			else myrotate.p -= rspeed;
			bChange = true;
		}

		break;
	case 'j':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 0, 1, 0);
			if (ifbond)
			{
				
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.h += rspeed;
			}
			else myrotate.h += rspeed;
			bChange = true;
		}

		break;
	case 'l':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 0, 1, 0);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.h -= rspeed;
			}
			else myrotate.h -= rspeed;
			bChange = true;
		}

		break;
	case 'u':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(rspeed, 0, 0, 1);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.b += rspeed;
			}
			else myrotate.b += rspeed;
			bChange = true;
		}

		break;
	case 'o':
		if (mode == 0)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(-rspeed, 0, 0, 1);
			if (ifbond)
			{
				glMultMatrixf(myview);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
			}
			else
			{
				glMultMatrixf(g_EyeMat);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			glPopMatrix();
		}
		else
		{
			if (ifbond)
			{
				ballro.b -= rspeed;
			}
			else myrotate.b -= rspeed;
			bChange = true;
		}

		break;

	case '[':
		mspeed *= 1.1;

		break;
	case ']':
		mspeed *= 0.9;

		break;
	case '{':
		rspeed *= 1.1;

		break;
	case '}':
		rspeed *= 0.9;

		break;
	case '1':
		mode = 1;
		break;
	case '2':
		mode = 0;
		flag = 1;//用于将自由漫游状态下视点矩阵转移到欧拉角
		break;

	}


	if (bChange)//计算视点矩阵的逆矩阵
	{
		glPushMatrix();
		glLoadIdentity();
		//视点在球上
		if (ifbond)
		{
			glRotatef(ballro.h, 0, 1, 0);
			glRotatef(ballro.p, 1, 0, 0);
			glRotatef(ballro.b, 0, 0, 1);
			glGetFloatv(GL_MODELVIEW_MATRIX, ballIe);
			
		}
		//视点不在球上
		else
		{
			glRotatef(myrotate.h, 0, 1, 0);
			glRotatef(myrotate.p, 1, 0, 0);
			glRotatef(myrotate.b, 0, 0, 1);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
		}
		glPopMatrix();
	}

}

//键盘响应函数
void myKeyboard()
{
	for (int i = 0; i < 16; i++)
	{
		if (keydown[i]) myKeymove(keyb[i]);
	}
}

//键盘按下信息更新
void myKeyboardFunc(unsigned char key, int x, int y) 
{
	if (key == ' ') {
		xseta = 0.;
		yseta = 0.;
	}
	if (key == 27)
	{
		if (ifbond == true)
		{
			ifbond = false;
			myview.SetTrans(CVector268(0, 0, 0));
			ifdynView = -300;
			CVector268 cvtool1 = ball[bi][1];
			CMatrix268 cmtoool = g_EyeMat;
			qbeg = cvtool1.ToEuler().ToQuaternion();
			pbeg = ball[bi][0];

			etar = g_EyeMat.GetInverse().ToEuler();
			qtar = etar.ToQuaternion();

			ptar.x = g_EyeMat.GetInverse().m03;
			ptar.y = g_EyeMat.GetInverse().m13;
			ptar.z = g_EyeMat.GetInverse().m23;

			cout << "取消绑定" << endl;
		}
	}

	switch (key)
	{
	case 'a':
		keydown[0] = true;
		break;

	case 'd':
		keydown[1] = true;
		break;

	case 'w':
		keydown[2] = true;
		break;

	case 's':
		keydown[3] = true;
		break;

	case 'q':
		keydown[4] = true;
		break;

	case 'e':
		keydown[5] = true;
		break;

	case 'j':
		keydown[6] = true;
		break;

	case 'l':
		keydown[7] = true;
		break;
			
	case 'i':
		keydown[8] = true;
		break;
		
	case 'k':
		keydown[9] = true;
		break;

	case 'u':
		keydown[10] = true;
		break;

	case 'o':
		keydown[11] = true;
		break;

	case '[':
		keydown[12] = true;
		break;
		
	case ']':
		keydown[13] = true;
		break;

	case '{':
		keydown[14] = true;
		break;

	case '}':
		keydown[15] = true;
		break;
		
	case '1':
		myKeymove('1');
		break;

	case '2':
		myKeymove('2');
		break;

	}

	

}

//键盘释放信息更新
void myKeyboardUpFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		keydown[0] = false;
		break;

	case 'd':
		keydown[1] = false;
		break;

	case 'w':
		keydown[2] = false;
		break;

	case 's':
		keydown[3] = false;
		break;

	case 'q':
		keydown[4] = false;
		break;

	case 'e':
		keydown[5] = false;
		break;

	case 'j':
		keydown[6] = false;
		break;

	case 'l':
		keydown[7] = false;
		break;

	case 'i':
		keydown[8] = false;
		break;

	case 'k':
		keydown[9] = false;
		break;

	case 'u':
		keydown[10] = false;
		break;

	case 'o':
		keydown[11] = false;
		break;

	case '[':
		keydown[12] = false;
		break;

	case ']':
		keydown[13] = false;
		break;

	case '{':
		keydown[14] = false;
		break;

	case '}':
		keydown[15] = false;
		break;
	}
}
//特殊键盘控制

void mySpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		xseta -= 1.0;
		break;
	case GLUT_KEY_DOWN:
		xseta += 1.0;
		break;
	case GLUT_KEY_LEFT:
		yseta -= 1.0;
		break;
	case GLUT_KEY_RIGHT:
		yseta += 1.0;
		break;
	}
}

//鼠标取点，视点绑定
void myMouseFunc(int button, int state, int x, int y)
{
	if (state)return;
	double projectmatrix[16], pos[3];
	int viewport[4];
	glGetDoublev(GL_PROJECTION_MATRIX, projectmatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
	float val;
	y = viewport[3] - y;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &val);
	gluUnProject(x, y, val, viewmatrix, projectmatrix, viewport, &pos[0], &pos[1], &pos[2]);
	//printf("%d:%d\t(%d:%d)\t%f\t(%.2f,%.2f,%.2f)\n", button, state, x, y, val, pos[0], pos[1], pos[2]);
	
	
	
	//cout << "hello" << endl;
	//判断是否取到球内的点
	for (int i = 0; i < 100; i++)
	{
	
		//判断点是否在球内
		float r = ballColro[i][2] * 13 / 0.8f;//球的半径
		float dis = 0;
		for (int j = 0; j < 3; j++) dis += ((pos[j] - ball[i][0][j])*(pos[j] - ball[i][0][j]));
		dis = sqrt(dis);
		//cout << pos[0] << '\t' << pos[1] << '\t' << pos[2]<<'\t'; cout << ball[i][0][0] << '\t' << ball[i][0][1] << '\t' << ball[i][0][2]<<'\t';
		
		if (dis < r)
		{
			//动态变化变量赋值
			if (ifbond)
			{
				CVector268 cvt = ball[bi][1];
				bball = cvt.ToEuler().ToMatrix();
				bball.m03 = ball[bi][0].x;
				bball.m13 = ball[bi][0].y;
				bball.m23 = ball[bi][0].z;
				bball.Inverse();
				pbeg = ball[bi][0];
				
				ebeg = (bball*myview).GetInverse().ToEuler();
				qbeg = etar.ToQuaternion();
				
			}
			else
			{
				qbeg = g_EyeMat.GetInverse().ToEuler().ToQuaternion();
				pbeg.x = g_EyeMat.GetInverse().m03;
				pbeg.y = g_EyeMat.GetInverse().m13;
				pbeg.z = g_EyeMat.GetInverse().m23;
			}
			//cout << dis << "\t" << r*r << endl;
			ifbond = true;
			bi = i;
			myview.SetTrans(CVector268(0, 0, 0));
			ballmove.Set(0, 0, 0);
			ballro.Set(0, 0, 0);
			ifdynView = 300;
			
			
			
			break;
		}
	}
	if (ifbond) cout << "成功绑定" << endl;
	else cout << "未绑定" << endl;
}

//视点绑定到球上
void bonBall()
{
	if (ifbond)
	{
		//求出物体视点矩阵
		CVector268 cvt = ball[bi][1];
		bball = cvt.ToEuler().ToMatrix();
		bball.m03 = ball[bi][0].x;
		bball.m13 = ball[bi][0].y;
		bball.m23 = ball[bi][0].z;
		bball.Inverse();
		ptar = ball[bi][0];

		etar = (bball*myview).GetInverse().ToEuler();
		qtar = etar.ToQuaternion();

	}
}

//视点动态变化
void dynView()
{
	if (ifdynView > 0)
	{
		qmid = qbeg;
		if (ifdynView < 100)
		{
			qmid = qbeg.Slerp(qtar, (101.0 - ifdynView) / 100.0);
		}
		
		pmid = pbeg + (ptar - pbeg)*((301.0 - ifdynView) / 300.0);

		glRotatef(-qmid.ToEuler().b, 0, 0, 1);
		glRotatef(-qmid.ToEuler().p, 1, 0, 0);
		glRotatef(-qmid.ToEuler().h, 0, 1, 0);
		/*glRotatef(-myrotate.b, 0, 0, 1);
		glRotatef(-myrotate.p, 1, 0, 0);
		glRotatef(-myrotate.h, 0, 1, 0);*/
		glTranslatef(-pmid.x, -pmid.y, -pmid.z);//位置成功
	
	}
	else if (ifdynView < 0)
	{
		qmid = qbeg.Slerp(qtar, (301 + ifdynView)/300.0);
		
		pmid = pbeg+(ptar - pbeg)*((301.0 + ifdynView) / 300.0);
		glRotatef(-qmid.ToEuler().b, 0, 0, 1);
		glRotatef(-qmid.ToEuler().p, 1, 0, 0);
		glRotatef(-qmid.ToEuler().h, 0, 1, 0);
		glTranslatef(-pmid.x, -pmid.y, -pmid.z);//位置成功
		//调试使用代码
		/*glRotatef(-qmid.ToEuler().b, 0, 0, 1);
		glRotatef(-qmid.ToEuler().p, 1, 0, 0);
		glRotatef(-qmid.ToEuler().h, 0, 1, 0);*/
		//glTranslatef(-mymove.x, -mymove.y, -mymove.z);
		
		//glLoadMatrixf(g_EyeMat);
		
	/*	glRotatef(-myrotate.b, 0, 0, 1);
		glRotatef(-myrotate.p, 1, 0, 0);
		glRotatef(-myrotate.h, 0, 1, 0);
		glTranslatef(-mymove.x, -mymove.y, -mymove.z);*/
		//glLoadMatrixf(g_EyeMat);
	

	}
	

}

//绘制前准备 初始化设置OpenGL绘制状态
void SetRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);

}

void SetView()
{
	if (ifdynView != 0)
	{
		return;
	}  //视点动态变化过程中停用此函数
	//自由漫游
	 if (mode == 0)
	{
		 //视点在球上
		 if (ifbond)
		 {
			 glLoadMatrixf(myview);
			 glMultMatrixf(bball);
		 }
		 //视点不在球上
		else glLoadMatrixf(g_EyeMat);
		
	}
	//欧拉角
	else
	{
		//切换回欧拉角
		if (flag)
		{
			//视点在球上
			if (ifbond)
			{
				//同步自由漫游的移动数据
				ballro = myview.GetInverse().ToEuler();
				ballmove.x = myview.GetInverse().m03;
				ballmove.y = myview.GetInverse().m13;
				ballmove.z = myview.GetInverse().m23;
				flag = 0;
				//更新逆矩阵
				glPushMatrix();
				glLoadIdentity();
				glRotatef(ballro.h, 0, 1, 0);
				glRotatef(ballro.p, 1, 0, 0);
				glRotatef(ballro.b, 0, 0, 1);
				glGetFloatv(GL_MODELVIEW_MATRIX, ballIe);
				//ballIe.Inverse();
				glPopMatrix();

				//设置视点
				CVector268 cvr = ball[bi][1];

				glRotatef(-ballro.b, 0, 0, 1);
				glRotatef(-ballro.p, 1, 0, 0);
				glRotatef(-ballro.h, 0, 1, 0);
				glTranslatef(-ballmove.x, -ballmove.y, -ballmove.z);

				glRotatef(-cvr.ToEuler().b, 0, 0, 1);
				glRotatef(-cvr.ToEuler().p, 1, 0, 0);
				glRotatef(-cvr.ToEuler().h, 0, 1, 0);
				glTranslatef(-ball[bi][0].x, -ball[bi][0].y, -ball[bi][0].z);

				
				//获取相对球的左乘视点矩阵用于切换到自由漫游
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
				myview = myview*bball.GetInverse();
			}
			//视点不在球上
			else
			{
				//同步
				myrotate = g_EyeMat.GetInverse().ToEuler();
				mymove.x = g_EyeMat.GetInverse().m03;
				mymove.y = g_EyeMat.GetInverse().m13;
				mymove.z = g_EyeMat.GetInverse().m23;
				flag = 0;
				//更新逆矩阵
				glPushMatrix();
				glLoadIdentity();
				glRotatef(myrotate.h, 0, 1, 0);
				glRotatef(myrotate.p, 1, 0, 0);
				glRotatef(myrotate.b, 0, 0, 1);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
				glPopMatrix();

				//设置视点
				glRotatef(-myrotate.b, 0, 0, 1);
				glRotatef(-myrotate.p, 1, 0, 0);
				glRotatef(-myrotate.h, 0, 1, 0);
				glTranslatef(-mymove.x, -mymove.y, -mymove.z);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			
			
		}
		//正常欧拉角
		else
		{
			//视点在球上
			if (ifbond)
			{
				CVector268 cvr = ball[bi][1];

				glRotatef(-ballro.b, 0, 0, 1);
				glRotatef(-ballro.p, 1, 0, 0);
				glRotatef(-ballro.h, 0, 1, 0);
				glTranslatef(-ballmove.x, -ballmove.y, -ballmove.z);

				glRotatef(-cvr.ToEuler().b, 0, 0, 1);
				glRotatef(-cvr.ToEuler().p, 1, 0, 0);
				glRotatef(-cvr.ToEuler().h, 0, 1, 0);
				glTranslatef(-ball[bi][0].x, -ball[bi][0].y, -ball[bi][0].z);

				//获取相对球的左乘视点矩阵用于切换到自由漫游
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
				myview = myview*bball.GetInverse();
				/*glPushMatrix();
				glRotatef(-ballro.b, 0, 0, 1);
				glRotatef(-ballro.p, 1, 0, 0);
				glRotatef(-ballro.h, 0, 1, 0);
				glTranslatef(-ballmove.x, -ballmove.y, -ballmove.z);
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
				glPopMatrix();*/

			}
			//视点不在球上
			else
			{
				glRotatef(-myrotate.b, 0, 0, 1);
				glRotatef(-myrotate.p, 1, 0, 0);
				glRotatef(-myrotate.h, 0, 1, 0);
				glTranslatef(-mymove.x, -mymove.y, -mymove.z);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);

			}
			
		}

	}
	glGetDoublev(GL_MODELVIEW_MATRIX, viewmatrix);
}
//绘制
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // 清除屏幕及深度缓存							
	glEnable(GL_DEPTH_TEST);	 // 深度测试开启，实现遮挡关系
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLoadIdentity();
	glPushMatrix();
	SetView();
	bonBall();

	//glPushMatrix();
	dynView();
	//glPopMatrix();


	glPushMatrix();
	
	//glTranslatef(-50, 400, 800);

	glRotatef(yseta, 0, 1, 0);
	glRotatef(xseta, 1, 0, 0);

	//绘制三色坐标轴
	dropCoordinate(110.f);

	//绘制六面体盒子
	drawBox();

	//绘制球
	dropball();
	glPopMatrix();
	glPopMatrix();
	
	glutSwapBuffers();

	glFlush();

	if (ifdynView > 0) ifdynView--;
	else if (ifdynView < 0) ifdynView++;

}

//设置视图大小和投影方式
void myReshape(int w, int h)
{

	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

int main(int argc, char *argv[])
{
	//处理数值开始
	ifstream ifs1;
	ifs1.open("test.txt", ios::in);

	ofstream ofs1;
	ofs1.open("out.txt", ios::out);

	if (!ifs1.is_open())
	{
		printf("文件打开失败\n");
		return 0;
	}
	//处理数值
	numericalOpr(ifs1, ofs1);

	ifs1.close();
	ofs1.close();
	//处理数值结束

	//产生球的信息
	ballelements();

	//openGle绘图开始
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("图形学作业3任务2");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	glutSpecialFunc(&mySpecialFunc);
	glutMouseFunc(&myMouseFunc);
	SetRC();
	glutMainLoop();
	
	//绘图相关代码结束
	
	
	getchar();
	return 0;
}