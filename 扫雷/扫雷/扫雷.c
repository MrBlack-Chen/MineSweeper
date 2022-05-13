#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>

#define ROW 9
#define COL 9
#define ROWS 11
#define COLS 11
#define Easy 15
#define Middle 30
#define Difficult 45
#define Most 55

double tmp_time;

void GotoXY(int x, int y)
{
	COORD pos;
	pos.X = x- 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//创建菜单函数
void menu()
{
	int x = 5, y = 10;
	GotoXY(x, y);
	printf("***********************************");
	GotoXY(x, y + 1);
	printf("***********    扫雷    ************");
	GotoXY(x, y + 2);
	printf("**** 1.开始游戏     0.退出游戏 ****");
	GotoXY(x, y + 3);
	printf("***********************************\n");
}

//创建初始化数组的函数(要用此初始化两个不同数组)
void InitBoard(char board[ROWS][COLS], int rows, int cols, char set)
{
	int i = 0;  int j = 0;
	for (i = 0; i<rows; i++)
	{
		for (j = 0; j<cols; j++)
		{
			board[i][j] = set;
		}
	}
}

//创建设置雷的函数    
void SetBoard(char board[ROWS][COLS], int row, int col,int Landmine)
{   //将雷设置在9*9的棋盘中,但为了保持一致
	//(传过来的是ran数组11*11的),仍写成ROWS
	//可以在后面代码的实现中灵活处理应对
	int x = 0;  int y = 0;  int cnt = 0;
	while (cnt<Landmine)
	{ //雷是设置在9*9的棋盘中,但在11*11的棋盘
      //下标范围为1～9,则%模上row(余数为0～8)再+1
		x = rand() % row + 1;  y = rand() % col + 1;
		if (board[x][y] == '0')
		{    //用0代表没雷，用1代表雷
			board[x][y] = '1';
			cnt++;
		}
	}
}

//创建打印棋盘的函数
void PrintBoard(char board[ROWS][COLS], int row, int col)
{
	int i = 0;  int j = 0;
	printf("标号 ");
	for (i = 1; i <= row; i++)//打印列号要在打印行之前
	{               
		printf("%d ", i);
	}       
	printf("\n");
	for (i = 1; i <= row; i++)
	{
		printf("  %d  ", i);//打印行号
		for (j = 1; j <= col; j++)
		{
			printf("%c ", board[i][j]);
		}  //数组里的元素对应的就是一个字符
		printf("\n");
	}
}

//计算九宫格有多少雷
int Getran(char board[ROWS][COLS], int x, int y)
{
	return board[x - 1][y - 1] + board[x][y - 1]
		+ board[x + 1][y - 1] + board[x - 1][y] + board[x + 1][y]
		+ board[x - 1][y + 1] + board[x][y + 1] + board[x + 1][y + 1]
		- 8 * '0';
}//字符+-字符变成数字,因为实际操作的为ASCII码值

//创建展开信息函数(点击以后可以展开一片信息)
void Develop(char ran[ROWS][COLS], char show[ROWS][COLS], int x, int y)
{
	if (x >= 1 && x <= ROW&&y >= 1 && y <= COL)
	{
		if (ran[x][y] == '0')
		{
			if (ran[x - 1][y - 1] == '0'&&show[x - 1][y - 1] == '*')
			{
				int cnt = Getran(ran, x - 1, y - 1);
				if (cnt == 0)
				{
					show[x - 1][y - 1] = ' ';
				}
				if (cnt != 0)
				{
					show[x - 1][y - 1] = cnt + '0';
				}
				Develop(ran, show, x - 1, y - 1);
			}
			if (ran[x - 1][y] == '0'&&show[x - 1][y] == '*')
			{
				int cnt = Getran(ran, x - 1, y);
				if (cnt == 0)
				{
					show[x - 1][y] = ' ';
				}
				if (cnt != 0)
				{
					show[x - 1][y] = cnt + '0';
				}
				Develop(ran, show, x - 1, y);
			}
			if (ran[x - 1][y + 1] == '0'&&show[x - 1][y + 1] == '*')
			{
				int cnt = Getran(ran, x - 1, y + 1);
				if (cnt == 0)
				{
					show[x - 1][y + 1] = ' ';
				}
				if (cnt != 0)
				{
					show[x - 1][y + 1] = cnt + '0';
				}
				Develop(ran, show, x - 1, y + 1);
			}
		}
	}
}

//创建判断扫雷成功的函数
char Iswin(char show[ROWS][COLS], int row, int col,int Landmine)
{
	int i = 0;  int j = 0; int cnt = 0;
	for (i = 1; i <= row; i++)
	{
		for (j = 1; j <= col; j++)
		{
			if (show[i][j] == '*')
				cnt++;
		}
	}
	if (cnt>Landmine)//通过计算剩下的'*'有多少,与Landmine(15个雷都没有显示,为'*')比较
	{   
		return 'C';//若cnt>20则说明还有不是雷的,则返回'C',游戏继续
	} 
	else           //否则说明剩下没点的都是雷,则扫雷成功
	{ 
		return 'W';
	}
}

//创建扫雷主体函数
void Findran(char ran[ROWS][COLS], char show[ROWS][COLS], int row, int col,int Landmine)
{
	int x = 0;  int y = 0;  int win = 0;
	char ret = 0;
	while (win<row*col - Landmine)
	{
		printf("请输入坐标：>");
		scanf("%d%d", &x, &y);
		if (x >= 1 && x <= row && y >= 1 && y <= col)
		{
			if (ran[x][y] == '1')
			{
				printf("很遗憾，你被雷炸死\n");
				PrintBoard(ran, ROW, COL);
				printf("\n");
				break;
			}
			else    
			{   
				int cnt = Getran(ran, x, y);//不是雷,则要显示信息,利用getran函数计算周围有多少个雷
				show[x][y] = cnt + '0';//cnt+'0'变成字符(因为show数组为字符数组)
				Develop(ran, show, x, y);
				PrintBoard(show, ROW, COL);
				ret = Iswin(show, ROW, COL,Landmine);
				if (ret != 'C')
				{
					break;
				}
			}
		}
		else
		{
			printf("坐标非法，请重新输入\n");
		}
	}
	if (ret == 'W')
	{
		printf("恭喜你，扫雷成功\n");
		printf("所用时间：%.2lfs\n", (clock() - tmp_time) / CLK_TCK);
	}
}

//创建游戏主体函数
void Game()
{ 
	int input = 0; int Landmine = 0;
	printf("*********  1.Easy         2.Middle  *********\n");
	printf("*********  3.Difficult    4.Most    *********\n");
	printf("请选择:>");
	scanf("%d", &input);
	switch (input)
	 {
	   case 1:
		    Landmine= Easy;
		   break;
	   case 2:
		    Landmine = Middle;
		   break;
	   case 3:
		    Landmine = Difficult;
		   break;
	   case 4:
		    Landmine = Most;
		   break;
	   default:
		   printf("选择错误,请重新选择\n");
		   break;
	 }
	system("cls");
	char ran[ROWS][COLS] = { 0 };//存储雷的信息
	char show[ROWS][COLS] = { 0 };//存储排雷信息
	InitBoard(ran, ROWS, COLS, '0');
	InitBoard(show, ROWS, COLS, '*');
	SetBoard(ran, ROW, COL,Landmine);
	PrintBoard(show, ROW, COL);
	double tmp_time = clock();
	Findran(ran, show, ROW, COL,Landmine);
}//当两个都要用同一个函数初始化时
//且初始化内容不同,可以将初始化内容作为实参传过去,将其初始化内容设置为对应的形参即可


int main()
{
	system("mode con cols=45");
	system("color 30");
	int input = 0;
	srand((unsigned int)time(NULL));
	do
	{
		system("cls");
		menu();
		printf("请选择：>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			Game();
			system("pause");
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			printf("选择错误,请重新选择\n");
			break;
		}
	} while (input);
	return 0;
}







