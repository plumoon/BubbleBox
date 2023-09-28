// GraphicsWork1.cpp : �������̨Ӧ�ó������ڵ㡣
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
//char ch = '	'; //test.txt ��������֮��ķָ���Ϊ��	���� ��

//����ͼ�����ȫ�ֱ���/����

//��������Ϣ�ڳ�ʼʱ�������һ��
//�ӵ�λ�úͷ���
int flag = 0,		ifbond=0,bi=-1,ifdynView=0;
//�Ƿ�������е�ŷ�� �Ƿ�󶨵����� ����ı��	�Ƿ��ڶ�̬��ֵ������ֵ���� //����0 �ϵ۵���С��0 ���ϵ�
float mspeed = 0.5, rspeed = 0.3;
bool keydown[16];
char keyb[16] = { 'a','d','w','s','q','e','j','l','i','k','u','o','[',']','{','}' };
CVector268 mymove(50, 120, 200);//ƽ��
CEuler268 myrotate(0, -25, 0);//��ת
CVector268 ballmove(0, 0, 0);//���ڲ�ƽ��
CEuler268 ballro(0, 0, 0);//���ڲ���ת
CMatrix268 ballIe(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//���ڲ������
CMatrix268 g_IEyeMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), g_EyeMat(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 );
CMatrix268 myview(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//���ڲ���ת��������
int mode = 1;//0 Ϊ��������ģʽ��1Ϊŷ����ģʽ
CVector268 ball[100][2]; //������������Ϣ ��һ�д���λ�ã��ڶ��д����ٶ�
float ballColro[100][3] = { 0.,0.,0. };//���������ɫ��Ϣ
//������ӵ����
CMatrix268 bball;
CQuaternion268 qbeg,qmid,qtar;//�ӵ㶯̬�仯������Ԫ��
CVector268 pbeg, pmid,ptar;//�ӵ㶯̬�仯����λ��
CEuler268 ebeg, emid, etar;//�ӵ㶯̬�仯����ŷ����
CMatrix268 cmid;//�ӵ㶯̬�仯����
const GLfloat Pi = 3.1415926536f; //��ֵ

								  //float seta = 0.f;//��ʱ���йر���
float xseta = 0.f, yseta = 0.f;//����ת���Ƕ�

							   //��������
							   //�������
void stringSplit(string str, const const char split, vector<string> &result);//�ַ����ָ��
CVector268 strToV(string str);// �������������ַ���ʵ����Ϊ����
CMatrix268 strToM(string str);// �����������ַ���ʵ����Ϊ����
CEuler268 strToE(string str);// �������������ַ���ʵ����Ϊŷ����
CQuaternion268 strToQ(string str);// �����������ַ���ʵ����Ϊ��Ԫ��
double viewmatrix[16], modelviewmatrix[16]; //���ȡ�����

void numericalOpr(ifstream &p, ofstream &q);  //�����ļ����ݽ��д���
void priEu(ofstream &q, CEuler268 p);//���ŷ����
void priQu(ofstream &q, CQuaternion268 p);//�����Ԫ��
void priMa(ofstream &q, CMatrix268 p);//�������
void priVe(ofstream &q, CVector268 p);//�������
						 
										   //��ͼ���
void dropCoordinate(float length);//����������	
void ballelements();//��������ʱ������������Ϣ����ִ��һ��
void dropball();  //������		
void drawBox();//�������������
void myTimerFunc(int val);//ʱ�亯��
void myKeymove(char key);//����ִ�к���
void myKeyboard();//������Ӧ����
void myKeyboardFunc(unsigned char key, int x, int y);//���̰�����Ϣ����
void myKeyboardUpFunc(unsigned char key, int x, int y);//�����ͷ���Ϣ����
void mySpecialFunc(int key, int x, int y);//������̿���
void myMouseFunc(int button, int state, int x, int y);//���ȡ�㣬�ӵ��
void bonBall();//�ӵ�󶨵���
void SetView();//�ӵ��������		  
void dynView(); //�ӵ㶯̬�仯
void SetRC();//����ǰ׼�� ��ʼ������OpenGL����״̬
void myDisplay(void);//����
void myReshape(int w, int h);//������ͼ��С��ͶӰ��ʽ

							 //��������

							 //�ַ����ָ��
void stringSplit(string str, const const char split, vector<string> &result)
{
	//����stringstreamʵ�ָ����ض��ַ��ָ��ַ����Ĺ���
	istringstream iss1(str);
	string buffer;
	while (getline(iss1, buffer, split))
	{
		result.push_back(buffer);
	}
}

//�������������ַ���ʵ����Ϊ����
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

//�����������ַ���ʵ����Ϊ����
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

// �������������ַ���ʵ����Ϊŷ����
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

// �������������ַ���ʵ����Ϊ��Ԫ��
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

//���ŷ����
void priEu(ofstream &q, CEuler268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2];
}

//�����Ԫ��
void priQu(ofstream &q, CQuaternion268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2] << ','<< p[3];
}

//�������
void priMa(ofstream &q,CMatrix268 p)
{
	q << p[0];
	for (int i = 1; i < 16; i++)
	{
		q << ',' << p[i];
	}
}

//�������
void priVe(ofstream &q,CVector268 p)
{
	q << p[0] << ',' << p[1] << ',' << p[2];
}

//�ú���ʵ����ҵҪ�����Ԫ����ŷ���Ǵ���
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
			if (!str1.compare("ŷ����ת������"))
			{
				flag = '1';
				break;
			}
			if (!str1.compare("����ת��ŷ����"))
			{
				flag = '2';
				break;
			}
			if (!str1.compare("ŷ����ת����Ԫ��"))
			{
				flag = '3';
				break;
			}
			if (!str1.compare("��Ԫ��ת��ŷ����"))
			{
				flag = '4';
				break;
			}
			if (!str1.compare("ŷ����ת������"))
			{
				flag = '5';
				break;
			}
			if (!str1.compare("����ת��ŷ����"))
			{
				flag = '6';
				break;
			}
			if (!str1.compare("����ת����Ԫ��"))
			{
				flag = '7';
				break;
			}
			if (!str1.compare("��Ԫ��ת������"))
			{
				flag = '8';
				break;
			}
			if (!str1.compare("ŷ���Ǳ�׼��"))
			{
				flag = '9';
				break;
			}
			if (!str1.compare("��Ԫ����λ��"))
			{
				flag = 'a';
				break;
			}
			if (!str1.compare("��Ԫ�����"))
			{
				flag = 'b';
				break;
			}
			if (!str1.compare("��Ԫ�����"))
			{
				flag = 'c';
				break;
			}
			if (!str1.compare("��Ԫ�����"))
			{
				flag = 'd';
				break;
			}
			if (!str1.compare("��Ԫ������"))
			{
				flag = 'e';
				break;
			}
			if (!str1.compare("��Ԫ����ǶȺ���ת��"))
			{
				flag = 'f';
				break;
			}
			if (!str1.compare("��Ԫ����ֵ"))
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
			q << "ŷ����ת������" << endl;
			//���ŷ����
			priEu(q, cetool1);q << '\t';
			//����������
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
			q << "����ת��ŷ����" << endl;
			//�������
			priVe(q, cvtool1);q << '\t';
			//������ŷ����
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
			q << "ŷ����ת����Ԫ��" << endl;
			//���ŷ����
			priEu(q, cetool1);q <<'\t';
			//��������Ԫ��
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
			q << "��Ԫ��ת��ŷ����" << endl;
			//�����Ԫ��
			priQu(q, cqtool1);
			q <<'\t';
			//������ŷ����
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

			q << "ŷ����ת������" << endl;
			//���ŷ����
			priEu(q, cetool1); q << '\t';
			//����������
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
			q << "����ת��ŷ����" << endl;

			//�������
			priMa(q, cmtool1); q << '\t'; 
			//������ŷ����
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
			q << "����ת����Ԫ��" << endl;

			//�������
			priMa(q, cmtool1);q << '\t';
			//�����Ԫ��
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
			q << "��Ԫ��ת������" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			//�������
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
			q << "ŷ���Ǳ�׼��" << endl;

			//���ԭŷ����
			priEu(q, cetool1); q << '\t';
			//������
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
			q << "��Ԫ����λ��" << endl;

			//���ԭ��Ԫ��
			priQu(q, cqtool1); q << '\t';
			//������
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
			q << "��Ԫ�����" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//������
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
			q << "��Ԫ�����" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//������
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
			q << "��Ԫ�����" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			//������
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
			q << "��Ԫ������" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			//��������Ԫ��
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
			q << "��Ԫ����ǶȺ���ת��" << endl;

			//�����Ԫ��
			priQu(q, cqtool1); q << '\t';
			//������
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
			q << "��Ԫ����ֵ" << endl;

			//�����Ԫ���Ͳ�ֵ����
			priQu(q, cqtool1); q << '\t';
			priQu(q, cqtool2); q << '\t';
			q << res << '\t';
			//������
			priQu(q, cqr); q << endl;

			flag = '0';
			break;
		}
	}


}

//����������
void dropCoordinate(float length)
{

	// x ���� ��ɫ
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(length, 1.f, 1.f);

	// x��ͷ
	glVertex3f(length, 1.f, 1.f);
	glVertex3f((0.98f)*length, (0.01f)*length + 1.f, 1.f);

	glVertex3f(length, 1.f, 1.f);
	glVertex3f((0.98f)*length, (-0.01)*length + 1.f, 1.f);
	glEnd();


	// y ���� ��ɫ
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, length, 1.f);

	// y��ͷ
	glVertex3f(1.f, length, 1.f);
	glVertex3f((0.01f)*length + 1.f, (0.98f)*length, 1.f);

	glVertex3f(1.f, length, 1.f);
	glVertex3f((-0.01f)*length + 1.f, (0.98f)*length, 1.f);
	glEnd();


	// z ������ɫ
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(1.f, 1.f, 1.f);
	glVertex3f(1.f, 1.f, length);

	// z ��ͷ
	glVertex3f(1.f, 1.f, length);
	glVertex3f(1.f, (0.01f)*length + 1.f, (0.98f)*length);

	glVertex3f(1.f, 1.f, length);
	glVertex3f(1.f, (-0.01f)*length + 1.f, (0.98f)*length);
	glEnd();

}

//�������������
void drawBox()
{
	//�������ĵ������ ��ʱ������
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

	//����ǽ����ɫ��ͬ
	GLfloat	wallcolor[6][3] = { { 1.0, 0.85, 0.73 }, {1.0, 0.55, 0.73},{ 1.0, 0.85, 0.33 },
	{ 0.8, 0.85, 0.73 },{ 0.6, 0.85, 0.73 },{ 0.4, 0.85, 0.73 } };


	glPushMatrix();
	//����ǰ׼��
								//glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT, GL_FILL); // ��������Ϊ���ģʽ
	glPolygonMode(GL_BACK, GL_LINE);   // ���÷���Ϊ����ģʽ
	glFrontFace(GL_CCW);      // ������ʱ�뷽��Ϊ����


	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++)
	{
		glColor3f(wallcolor[i][0], wallcolor[i][1], wallcolor[i][2]);//���ò�ͬ����ɫ
		glVertex3f(box[i][0][0], box[i][0][1], box[i][0][2]);
		glVertex3f(box[i][1][0], box[i][1][1], box[i][1][2]);
		glVertex3f(box[i][2][0], box[i][2][1], box[i][2][2]);
		glVertex3f(box[i][3][0], box[i][3][1], box[i][3][2]);

	}
	glEnd();

	glPopMatrix();
}

//������������Ϣ
void ballelements()
{
	//��������Ϣ��ʼ��Ϊfalse
	for(int i =0;i<16;i++) keydown[i] = false;
	//����100������� ÿ��6������λ��ȡǰ����,0~100����ɫȡ2��3��4���ٶ�ȡ��������0~0.1,��뾶ȡ4��С��13
	
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
		while (w > Rmax / 1000) { w -= Rmax / 20000; };//ͨ�����ư뾶������֤�߽���ֵ��򲻻���һ���ֳ�����ǽ��

		ballColro[i][0] = y / 100.f;
		ballColro[i][1] = z / 100.f;
		ballColro[i][2] = w / 0.1f;

	}

}

//������
void dropball()
{
	//������
	
	for (int i = 0; i < 100; i++)
	{
		//���λ�ñ仯
		ball[i][0] = ball[i][0] + ball[i][1];
		float r = ballColro[i][2] * 13 / 0.8f;//��İ뾶
											  //�ж����Ƿ񵽴�߽�
		for (int j = 0; j < 3; j++)
		{
			if ((ball[i][0][j] + r) >= 100 || (ball[i][0][j] - r) <= 0)
			{
				ball[i][1][j] *= -1;//��ĳ��������ǽ�ڣ���˷����ٶȷ�������
			}
		}

		glColor3f(ballColro[i][0] * 1.2, ballColro[i][2] * 2, ballColro[i][1] * 0.9);
		glPushMatrix();
		glTranslatef(ball[i][0].x, ball[i][0].y, ball[i][0].z);
		glPolygonMode(GL_FRONT, GL_FILL); // ��������Ϊ���ģʽ
		glPolygonMode(GL_BACK, GL_LINE);   // ���÷���Ϊ����ģʽ
		glutSolidSphere(r, 25, 25);
		glPopMatrix();

	}
}

//ʱ�亯��
void myTimerFunc(int val)
{
	myDisplay();
	myKeyboard();//�������Ӧִ�к���
	
	glutTimerFunc(1, myTimerFunc, 0);
	
}

//����ִ�к���
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
		flag = 1;//���ڽ���������״̬���ӵ����ת�Ƶ�ŷ����
		break;

	}


	if (bChange)//�����ӵ����������
	{
		glPushMatrix();
		glLoadIdentity();
		//�ӵ�������
		if (ifbond)
		{
			glRotatef(ballro.h, 0, 1, 0);
			glRotatef(ballro.p, 1, 0, 0);
			glRotatef(ballro.b, 0, 0, 1);
			glGetFloatv(GL_MODELVIEW_MATRIX, ballIe);
			
		}
		//�ӵ㲻������
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

//������Ӧ����
void myKeyboard()
{
	for (int i = 0; i < 16; i++)
	{
		if (keydown[i]) myKeymove(keyb[i]);
	}
}

//���̰�����Ϣ����
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

			cout << "ȡ����" << endl;
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

//�����ͷ���Ϣ����
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
//������̿���

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

//���ȡ�㣬�ӵ��
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
	//�ж��Ƿ�ȡ�����ڵĵ�
	for (int i = 0; i < 100; i++)
	{
	
		//�жϵ��Ƿ�������
		float r = ballColro[i][2] * 13 / 0.8f;//��İ뾶
		float dis = 0;
		for (int j = 0; j < 3; j++) dis += ((pos[j] - ball[i][0][j])*(pos[j] - ball[i][0][j]));
		dis = sqrt(dis);
		//cout << pos[0] << '\t' << pos[1] << '\t' << pos[2]<<'\t'; cout << ball[i][0][0] << '\t' << ball[i][0][1] << '\t' << ball[i][0][2]<<'\t';
		
		if (dis < r)
		{
			//��̬�仯������ֵ
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
	if (ifbond) cout << "�ɹ���" << endl;
	else cout << "δ��" << endl;
}

//�ӵ�󶨵�����
void bonBall()
{
	if (ifbond)
	{
		//��������ӵ����
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

//�ӵ㶯̬�仯
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
		glTranslatef(-pmid.x, -pmid.y, -pmid.z);//λ�óɹ�
	
	}
	else if (ifdynView < 0)
	{
		qmid = qbeg.Slerp(qtar, (301 + ifdynView)/300.0);
		
		pmid = pbeg+(ptar - pbeg)*((301.0 + ifdynView) / 300.0);
		glRotatef(-qmid.ToEuler().b, 0, 0, 1);
		glRotatef(-qmid.ToEuler().p, 1, 0, 0);
		glRotatef(-qmid.ToEuler().h, 0, 1, 0);
		glTranslatef(-pmid.x, -pmid.y, -pmid.z);//λ�óɹ�
		//����ʹ�ô���
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

//����ǰ׼�� ��ʼ������OpenGL����״̬
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
	}  //�ӵ㶯̬�仯������ͣ�ô˺���
	//��������
	 if (mode == 0)
	{
		 //�ӵ�������
		 if (ifbond)
		 {
			 glLoadMatrixf(myview);
			 glMultMatrixf(bball);
		 }
		 //�ӵ㲻������
		else glLoadMatrixf(g_EyeMat);
		
	}
	//ŷ����
	else
	{
		//�л���ŷ����
		if (flag)
		{
			//�ӵ�������
			if (ifbond)
			{
				//ͬ���������ε��ƶ�����
				ballro = myview.GetInverse().ToEuler();
				ballmove.x = myview.GetInverse().m03;
				ballmove.y = myview.GetInverse().m13;
				ballmove.z = myview.GetInverse().m23;
				flag = 0;
				//���������
				glPushMatrix();
				glLoadIdentity();
				glRotatef(ballro.h, 0, 1, 0);
				glRotatef(ballro.p, 1, 0, 0);
				glRotatef(ballro.b, 0, 0, 1);
				glGetFloatv(GL_MODELVIEW_MATRIX, ballIe);
				//ballIe.Inverse();
				glPopMatrix();

				//�����ӵ�
				CVector268 cvr = ball[bi][1];

				glRotatef(-ballro.b, 0, 0, 1);
				glRotatef(-ballro.p, 1, 0, 0);
				glRotatef(-ballro.h, 0, 1, 0);
				glTranslatef(-ballmove.x, -ballmove.y, -ballmove.z);

				glRotatef(-cvr.ToEuler().b, 0, 0, 1);
				glRotatef(-cvr.ToEuler().p, 1, 0, 0);
				glRotatef(-cvr.ToEuler().h, 0, 1, 0);
				glTranslatef(-ball[bi][0].x, -ball[bi][0].y, -ball[bi][0].z);

				
				//��ȡ����������ӵ���������л�����������
				glGetFloatv(GL_MODELVIEW_MATRIX, myview);
				myview = myview*bball.GetInverse();
			}
			//�ӵ㲻������
			else
			{
				//ͬ��
				myrotate = g_EyeMat.GetInverse().ToEuler();
				mymove.x = g_EyeMat.GetInverse().m03;
				mymove.y = g_EyeMat.GetInverse().m13;
				mymove.z = g_EyeMat.GetInverse().m23;
				flag = 0;
				//���������
				glPushMatrix();
				glLoadIdentity();
				glRotatef(myrotate.h, 0, 1, 0);
				glRotatef(myrotate.p, 1, 0, 0);
				glRotatef(myrotate.b, 0, 0, 1);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
				glPopMatrix();

				//�����ӵ�
				glRotatef(-myrotate.b, 0, 0, 1);
				glRotatef(-myrotate.p, 1, 0, 0);
				glRotatef(-myrotate.h, 0, 1, 0);
				glTranslatef(-mymove.x, -mymove.y, -mymove.z);
				glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			}
			
			
		}
		//����ŷ����
		else
		{
			//�ӵ�������
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

				//��ȡ����������ӵ���������л�����������
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
			//�ӵ㲻������
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
//����
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   // �����Ļ����Ȼ���							
	glEnable(GL_DEPTH_TEST);	 // ��Ȳ��Կ�����ʵ���ڵ���ϵ
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

	//������ɫ������
	dropCoordinate(110.f);

	//�������������
	drawBox();

	//������
	dropball();
	glPopMatrix();
	glPopMatrix();
	
	glutSwapBuffers();

	glFlush();

	if (ifdynView > 0) ifdynView--;
	else if (ifdynView < 0) ifdynView++;

}

//������ͼ��С��ͶӰ��ʽ
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
	//������ֵ��ʼ
	ifstream ifs1;
	ifs1.open("test.txt", ios::in);

	ofstream ofs1;
	ofs1.open("out.txt", ios::out);

	if (!ifs1.is_open())
	{
		printf("�ļ���ʧ��\n");
		return 0;
	}
	//������ֵ
	numericalOpr(ifs1, ofs1);

	ifs1.close();
	ofs1.close();
	//������ֵ����

	//���������Ϣ
	ballelements();

	//openGle��ͼ��ʼ
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("ͼ��ѧ��ҵ3����2");
	glutDisplayFunc(&myDisplay);
	glutTimerFunc(1, myTimerFunc, 0);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutKeyboardUpFunc(&myKeyboardUpFunc);
	glutSpecialFunc(&mySpecialFunc);
	glutMouseFunc(&myMouseFunc);
	SetRC();
	glutMainLoop();
	
	//��ͼ��ش������
	
	
	getchar();
	return 0;
}