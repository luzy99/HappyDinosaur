/*****************************************
HappyDinosaur by LZY
compiled by VS2015 using EasyX(20180727)
*****************************************/
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include"resource.h"
#pragma comment(lib, "Msimg32.lib")

IMAGE backimg, dino[4];
char keydown = '`';


class dinosaur
{
private:
	int y = 0;			//�߶�
	int v = 240;		//�ٶ�
	int t = 0;			//����ʱ��
	int t1 = 0;			//����ʱ��
	int width;			//��
	int height;			//��
public:

	int gety() { return y; }
	int geth() { return height; }
	int getw() { return width; }
	void sety(int a){y = a;}
	void sett(int a){t = a;}

	dinosaur()
	{
		loadimage(&dino[0], _T("IMAGE"), _T("dino0"));
		loadimage(&dino[1], _T("IMAGE"), _T("dino1"));
		loadimage(&dino[2], _T("IMAGE"), _T("dino2"));
		loadimage(&dino[3], _T("IMAGE"), _T("dino3"));
		width = dino[0].getwidth();
		height = dino[0].getheight();
	}
	void jump(int &n)
	{
		if (y <= 0&&t1==0)
		{
			y = (2 * t*t - v*t) / 50;
			t++;
		}
		else
		{
			keydown = '`';	//�����ֵ
			y = 0;
			t = 0;
			t1 = 0;
		}
		n = 0;			//��������޸���ͼ

	}
	void down(int &n)
	{
		if (y < 0)
		{
			y += t1*t1 / 30;
			t1++;
			n = 0;			//��������޸���ͼ
		}
		else
		{
			keydown = '`';	//�����ֵ
			y = 0;
			t1 = 0;
			t = 0;
		}
		
	}
};

class barrier
{
private:
	
	double left;
	int right;
	int height;
	int v;
	int type;
	IMAGE barri[3];
	IMAGE img;

	int r;
public:
	
	barrier(int x)
	{
		rebuild(x);
 	}
	void rebuild(int x)
	{
		loadimage(&barri[0], _T("IMAGE"), _T("barrier0"));
		loadimage(&barri[1], _T("IMAGE"), _T("barrier1"));
		loadimage(&barri[2], _T("IMAGE"), _T("barrier2"));
		type = rand() % 3;
		img = barri[type];
		left = 640+x;
		r = rand() % 60;
		height = img.getheight();
		x += 60;
	}
	void create(double speed)
	{

		HDC srcDC = GetImageHDC();           //���ھ��
		HDC barDC = GetImageHDC(&img);
		TransparentBlt(srcDC, left, 330 - img.getheight(), img.getwidth(), img.getheight(), barDC, 0, 0, img.getwidth(), img.getheight(), RGB(17, 17, 102));//����͸��ɫ
		left -= speed;
		right = left + img.getwidth();
		if (left <= -60)rebuild(r-30);
	}
	void crash(dinosaur d1)
	{
		if (110 - d1.gety() - d1.geth() < height)
			if (25 < right&&left < d1.getw()-20)
				_getch();

	}
};
/************************************************************************************/
void game();

void score(double s)
{
	settextcolor(DARKGRAY);
	outtextxy(10, 10, (int)(s*100));
}

void init()        //��ʼ��
{

	initgraph(640, 480);							// ���ڳ�ʼ����С
	SetWindowText(GetHWnd(), _T("HappyDinosaur"));	// ���ô��ڱ�������
	setbkcolor(WHITE);								// ���ñ�����ɫ
	cleardevice();
	srand(time(0));	// �����������
	loadimage(&backimg, _T("IMAGE"), _T("background"));//���ر���ͼ

	settextstyle(25, 0, _T("����"));	// ������ʽ��С
	settextcolor(BLUE);					// ����������ɫ

}


void login()		//��¼����
{
	dinosaur d1;
	HDC srcDC = GetImageHDC();           //���ھ��
	HDC aaaDC = GetImageHDC(&dino[1]);   //ͼƬ���
	MOUSEMSG m;                          //����ƶ�

	while (true)
	{

		BeginBatchDraw();
		for (int n = 1, i = 0;i >= -1280;i--)
		{
			putimage(i, 0, &backimg);
			aaaDC = GetImageHDC(&dino[n]);
			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//����͸��ɫ
			n = -i / 40 % 2 + 1;
			solidrectangle(240, 100, 380, 150);
			solidrectangle(240, 160, 380, 210);

			outtextxy(260, 113, _T("��ʼ��Ϸ"));
			outtextxy(260, 173, _T("�˳���Ϸ"));
			outtextxy(260, 233, _T("��Ϸ˵��"));
			if (MouseHit())						//�ж�����Ƿ��ƶ�
				m = GetMouseMsg();				// ��¼���λ��
			if (m.x >= 240 && m.x <= 380 && m.y >= 100 && m.y <= 150)//[��ʼ��Ϸ]
			{
				setlinecolor(RED);			// ѡ�п��������ɫ
				rectangle(235, 95, 385, 155);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					cleardevice();
					game();
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 160 && m.y <= 210)//[�˳���Ϸ]
			{
				setlinecolor(RED);			// ѡ�п��������ɫ
				rectangle(235, 155, 385, 215);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					exit(1);
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 210 && m.y <= 260)//[��Ϸ˵��]
			{
				setlinecolor(RED);			// ѡ�п��������ɫ
				rectangle(235, 215, 385, 275);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					exit(1);
				}
			}
			else
			{
				setlinecolor(WHITE);
				rectangle(235, 95, 385, 155);
				rectangle(235, 155, 385, 215);
			}

			FlushBatchDraw();
			Sleep(5);
		}
	}
}


void game()
{
	clock_t start;
	dinosaur d1;
	double speed = 1.5;
	double nowtime;

	HDC srcDC = GetImageHDC();           //���ھ��
	HDC aaaDC = GetImageHDC(&dino[1]);   //ͼƬ���
	barrier bar1(0);
	barrier bar2(350);
	start = clock();
	BeginBatchDraw();
	
	while (true)
	{
		int n = 1;
		int i = 0;
		for (double k = 0.0;k >= -1280;k -= speed)
		{
			i = (int)k;
			putimage(i, 0, &backimg);

			if (_kbhit())
			switch (_getch())			//���˵���������
			{
			case 'w':
			case ' ':
				keydown = 'w';
				break;
			case 's':
				keydown = 's';
				break;
			default:
				break;
			}
			if (keydown == 'w')			//��W��ո���Ծ
			{
				d1.jump(n);
			}
			else if (keydown == 's')	//��S�����½�
			{
				d1.down(n);
			}

			aaaDC = GetImageHDC(&dino[n]);

			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//����͸��ɫ
			n = -i / 40 % 2 + 1;		//С������ͼ�л�
			bar1.create(speed);
			bar2.create(speed);
			bar1.crash(d1);
			bar2.crash(d1);
			nowtime = (clock() - start) / CLOCKS_PER_SEC;
			score(nowtime);
			FlushBatchDraw();
			Sleep(5);
			

		}
		if ((int)(nowtime * 10) % 30 == 0)
				speed += 0.3;
	}

}



int main()
{
	init();
	login();

}
