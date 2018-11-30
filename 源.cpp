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
	int y = 0;			//高度
	int v = 240;		//速度
	int t = 0;			//起跳时间
	int t1 = 0;			//下落时间
	int width;			//长
	int height;			//宽
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
			keydown = '`';	//清除键值
			y = 0;
			t = 0;
			t1 = 0;
		}
		n = 0;			//传入参数修改贴图

	}
	void down(int &n)
	{
		if (y < 0)
		{
			y += t1*t1 / 30;
			t1++;
			n = 0;			//传入参数修改贴图
		}
		else
		{
			keydown = '`';	//清除键值
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

		HDC srcDC = GetImageHDC();           //窗口句柄
		HDC barDC = GetImageHDC(&img);
		TransparentBlt(srcDC, left, 330 - img.getheight(), img.getwidth(), img.getheight(), barDC, 0, 0, img.getwidth(), img.getheight(), RGB(17, 17, 102));//设置透明色
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

void init()        //初始化
{

	initgraph(640, 480);							// 窗口初始化大小
	SetWindowText(GetHWnd(), _T("HappyDinosaur"));	// 设置窗口标题文字
	setbkcolor(WHITE);								// 设置背景颜色
	cleardevice();
	srand(time(0));	// 设置随机种子
	loadimage(&backimg, _T("IMAGE"), _T("background"));//加载背景图

	settextstyle(25, 0, _T("黑体"));	// 字体样式大小
	settextcolor(BLUE);					// 设置字体颜色

}


void login()		//登录界面
{
	dinosaur d1;
	HDC srcDC = GetImageHDC();           //窗口句柄
	HDC aaaDC = GetImageHDC(&dino[1]);   //图片句柄
	MOUSEMSG m;                          //鼠标移动

	while (true)
	{

		BeginBatchDraw();
		for (int n = 1, i = 0;i >= -1280;i--)
		{
			putimage(i, 0, &backimg);
			aaaDC = GetImageHDC(&dino[n]);
			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
			n = -i / 40 % 2 + 1;
			solidrectangle(240, 100, 380, 150);
			solidrectangle(240, 160, 380, 210);

			outtextxy(260, 113, _T("开始游戏"));
			outtextxy(260, 173, _T("退出游戏"));
			outtextxy(260, 233, _T("游戏说明"));
			if (MouseHit())						//判断鼠标是否移动
				m = GetMouseMsg();				// 记录鼠标位置
			if (m.x >= 240 && m.x <= 380 && m.y >= 100 && m.y <= 150)//[开始游戏]
			{
				setlinecolor(RED);			// 选中框框线条颜色
				rectangle(235, 95, 385, 155);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					cleardevice();
					game();
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 160 && m.y <= 210)//[退出游戏]
			{
				setlinecolor(RED);			// 选中框框线条颜色
				rectangle(235, 155, 385, 215);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					exit(1);
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 210 && m.y <= 260)//[游戏说明]
			{
				setlinecolor(RED);			// 选中框框线条颜色
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

	HDC srcDC = GetImageHDC();           //窗口句柄
	HDC aaaDC = GetImageHDC(&dino[1]);   //图片句柄
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
			switch (_getch())			//过滤掉其他按键
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
			if (keydown == 'w')			//按W或空格跳跃
			{
				d1.jump(n);
			}
			else if (keydown == 's')	//按S快速下降
			{
				d1.down(n);
			}

			aaaDC = GetImageHDC(&dino[n]);

			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
			n = -i / 40 % 2 + 1;		//小恐龙贴图切换
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
