/*****************************************
HappyDinosaur by LZY
compiled by VS2015 using EasyX(20180727)
Last edited:  2018/12/03
*****************************************/
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <fstream>
#include"resource.h"
#pragma comment(lib, "Msimg32.lib")


IMAGE backimg, dino[4];
char keydown;		//键值
char keydown2;		//键值
int ss;				//分数
int	max_s = 0;		//最高分
int fire_cnt = 3;

void game();
void game_over();
/********************************** 类 ***********************************************/
//小恐龙
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
	void sety(int a) { y = a; }
	void sett(int a) { t = a; }

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
		if (y <= 0 && t1 == 0)
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

//障碍物
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
		left = 640 + x;

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
		r = rand() % 80;
		if (left <= -460)rebuild(r - 30);
	}
	void crash(dinosaur d1)
	{
		if (120 - d1.gety() - d1.geth() < height)
			if (25 < right&&left < d1.getw() - 20)
			{
				HDC srcDC = GetImageHDC();
				HDC aaaDC = GetImageHDC(&dino[3]);
				TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
				game_over();
			}

	}
};

//火球
class fireball
{
private:
	IMAGE img;
	int x;
	int y;
	int right;
	int down;
	
public:
	bool flag;
	fireball()
	{
		y = -100;
		x = 80;
		loadimage(&img, _T("IMAGE"), _T("ball"));
		flag = 0;
	}
	void fire()
	{
		
		HDC srcDC = GetImageHDC();           //窗口句柄
		HDC ballDC = GetImageHDC(&img);
		TransparentBlt(srcDC, x, y, img.getwidth(), img.getheight(), ballDC, 0, 0, img.getwidth(), img.getheight(), RGB(17, 17, 102));//设置透明色
		x += 2;
		if (x >= 1080)
		{
			flag = 0;
			y = -100;
			x = 80;
		}
		right = x + img.getheight();
		down = y + img.getheight();
	}
	void sety(int dy) { y = dy; }
	void setx(int dx) { x = dx; }
	int getx() { return x; }
	int gety() { return y; }
	int getright() { return right; }
	int getdown() { return down; }
};

//鸟
class bird
{
private:
	IMAGE img;
	int left;
	int up;
	int h;
	int w;
	int right;
	int down;
	int bonus_x = 0;
	int bonus_y = -30;
	int cnt;
public:
	bird()
	{
		left = 1000;
		up = 110;
		loadimage(&img, _T("IMAGE"), _T("bird"));
		h = img.getheight();
		w = img.getwidth();
	}
	void show()
	{
		HDC srcDC = GetImageHDC();           //窗口句柄
		HDC birdDC = GetImageHDC(&img);
		TransparentBlt(srcDC, left, up, img.getwidth(), img.getheight(), birdDC, 0, 0, img.getwidth(), img.getheight(), RGB(17, 17, 102));//设置透明色
		left -= 1;
		right = left + w;
		down = up + h;
		if (left <= -500)
		{
			left = 640;
			up = 110 + rand() % 100;
		}
	}
	void crash(dinosaur d1)
	{
		if (left < 0 + d1.getw() && right>30 && down > 240 + d1.gety() && up < 220 + d1.gety() + d1.geth())
		{
			HDC srcDC = GetImageHDC();
			HDC aaaDC = GetImageHDC(&dino[3]);
			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
			game_over();
		}
	}

	void kill(fireball &f1)
	{
		if (f1.getdown() > up&&f1.gety() < down&&f1.getright() > left + 10)
		{
			f1.sety(-99);
			f1.fire();
			bonus_x = left;
			bonus_y = up;
			left = -100 - rand() % 300;
			up = -500;
			show();
			ss += 200;
			cnt = 0;
		}
		if (bonus_y > -30)					//击中显示+10分
		{
			settextcolor(YELLOW);
			settextstyle(40, 0, _T("Hobo Std"));
			outtextxy(bonus_x, bonus_y, _T("+10"));
			bonus_y--;
			cnt++;

		}
	}
};

/*********************************** 函数 *********************************************/

//显示分数
void score(double s)
{
	TCHAR m[10] = { 0 };
	TCHAR ms[10] = { 0 };
	TCHAR cnt[2] = { 0 };
	settextcolor(DARKGRAY);
	settextstyle(25, 0, _T("黑体"));
	_stprintf_s(m, _T("%d"), int(s / 20));
	outtextxy(500, 10, _T("Score:"));
	outtextxy(590, 10, m);
	_stprintf_s(ms, _T("%d"), max_s);
	outtextxy(20, 10, _T("HI:"));
	outtextxy(80, 10, ms);
	_stprintf_s(cnt, _T("%d"), fire_cnt);
	settextstyle(20, 0, _T("微软雅黑"));
	outtextxy(35, 379, _T("剩余弹药："));
	outtextxy(100, 380, cnt);
}

//初始化
void init()
{
	initgraph(640, 480);							// 窗口初始化大小
	SetWindowText(GetHWnd(), _T("HappyDinosaur"));	// 设置窗口标题文字
	setbkcolor(WHITE);								// 设置背景颜色
	cleardevice();
	srand(time(0));									// 设置随机种子
	loadimage(&backimg, _T("IMAGE"), _T("background"));//加载背景图

	settextstyle(25, 0, _T("黑体"));	// 字体样式大小
	settextcolor(BLUE);					// 设置字体颜色
	ss = 0;
	keydown = '`';
	keydown2 = '`';
	FlushMouseMsgBuffer();		//清空鼠标缓存区
}

//登录界面
void login()
{
	dinosaur d1;
	HDC srcDC = GetImageHDC();           //窗口句柄
	HDC aaaDC = GetImageHDC(&dino[1]);   //图片句柄
	MOUSEMSG m;                          //鼠标移动
	bool flag = 0;

	BeginBatchDraw();
	while (true)
	{
		for (int n = 1, i = 0;i >= -1280;i--)
		{
			putimage(i, 0, &backimg);
			aaaDC = GetImageHDC(&dino[n]);
			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
			n = -i / 40 % 2 + 1;
			solidrectangle(240, 100, 380, 150);
			solidrectangle(240, 160, 380, 210);

			settextcolor(BLUE);
			outtextxy(260, 113, _T("开始游戏"));
			outtextxy(260, 173, _T("退出游戏"));
			outtextxy(260, 233, _T("游戏说明"));

			if (_kbhit())keydown = _getch();
			if (keydown == ' ')
			{
				cleardevice();
				game();
			}
			if (MouseHit())						//判断鼠标是否移动
				m = GetMouseMsg();				// 记录鼠标位置
			if (m.x >= 240 && m.x <= 380 && m.y >= 100 && m.y <= 150)		//[开始游戏]
			{
				setlinecolor(RED);			// 选中框框线条颜色
				rectangle(235, 95, 385, 155);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					cleardevice();
					game();
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 160 && m.y <= 210)	//[退出游戏]
			{
				setlinecolor(RED);			// 选中框框线条颜色
				rectangle(235, 155, 385, 215);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					exit(0);
				}
			}
			else if (m.x >= 240 && m.x <= 380 && m.y >= 210 && m.y <= 260)	//[游戏说明]
			{
				setlinecolor(RED);			// 选中框框线条颜色
				rectangle(235, 215, 385, 275);
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					flag = flag ? 0 : 1;
					m.uMsg = 0;
				}
			}
			else
			{
				setlinecolor(WHITE);
				rectangle(235, 95, 385, 155);
				rectangle(235, 155, 385, 215);
			}
			settextcolor(WHITE);
			if (flag)
			{
				settextcolor(CYAN);
			}

			outtextxy(110, 380, _T("按W或空格跳跃，按S快速下降，按J攻击"));
			FlushBatchDraw();
			Sleep(5);
		}
	}
}

//开始游戏
void game()
{
	clock_t start;
	dinosaur d1;

	double speed = 2;
	double nowtime;

	fireball f1;
	fireball f2;
	fireball f3;
	bird bird1;
	HDC srcDC = GetImageHDC();           //窗口句柄
	HDC aaaDC = GetImageHDC(&dino[1]);   //图片句柄
	barrier bar1(0);
	barrier bar2(500);
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
				case 'j':
					keydown2 = 'j';
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
			if (keydown2 == 'j')		//按J发射火球
			{
				if (!f1.flag)
				{
					f1.sety(265 + d1.gety());
					f1.flag = 1;
				}
				else if (!f2.flag)
					{
						f2.sety(265 + d1.gety());
						f2.flag = 1;
					}
				else if (!f3.flag)
				{
					f3.sety(265 + d1.gety());
					f3.flag = 1;
				}
				keydown2 = '`';
			}
			if (f1.flag)f1.fire();
			if (f2.flag)f2.fire();
			if (f3.flag)f3.fire();
			aaaDC = GetImageHDC(&dino[n]);

			TransparentBlt(srcDC, 10, 230 + d1.gety(), dino[1].getwidth(), dino[1].getheight(), aaaDC, 0, 0, dino[1].getwidth(), dino[1].getheight(), RGB(17, 17, 102));//设置透明色
			n = -i / 40 % 2 + 1;		//小恐龙贴图切换
			bar1.create(speed);
			bar2.create(speed);
			bird1.show();

			bar1.crash(d1);				//障碍物碰撞检测
			bar2.crash(d1);				//障碍物碰撞检测
			bird1.crash(d1);			//小鸟碰撞检测
			bird1.kill(f1);				//小鸟击中检测
			bird1.kill(f2);
			bird1.kill(f3);

			fire_cnt = !f1.flag + !f2.flag + !f3.flag;

			nowtime = (clock() - start) / CLOCKS_PER_SEC;
			ss += 1;
			score(ss);					//显示分数


			FlushBatchDraw();
			Sleep(5);


		}
		if ((int)(nowtime * 10) % 30 == 0)	//每三秒加速0.3
			speed += 0.3;
	}

}

//游戏结束
void game_over()
{
	EndBatchDraw();
	keydown2 = '`';
	
	if (ss / 20 > max_s)
	{
		max_s = ss / 20;
		settextstyle(25, 0, _T("黑体"));
		outtextxy(250, 240, _T("NEW RECORD!"));
	}
	score(ss);					//显示分数
	settextstyle(25, 0, _T("黑体"));
	outtextxy(260, 113, _T("GAME OVER"));
	settextstyle(20, 0, _T("黑体"));
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));	//清除按键缓存
	while (true)
	{
		settextcolor(DARKGRAY);
		outtextxy(190, 183, _T("PRESS ANY KEY TO TRY AGAIN"));
		Sleep(400);
		settextcolor(WHITE);
		outtextxy(190, 183, _T("PRESS ANY KEY TO TRY AGAIN"));
		Sleep(200);
		if (_kbhit())break;
	}
	
	init();
	login();
}



void main()
{
	init();
	login();

}
