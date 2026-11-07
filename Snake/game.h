#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

//游戏墙体
#define GAMEROW  30
#define GAMECOL 30

extern struct Game game;

//食物位置
extern int gFoodx;
extern int gFoody;

//游戏主体
struct Game
{
	int state[GAMEROW][GAMECOL];

};
//游戏位置
enum rectstate {
	EMPTY = 1,
	WALL,
	FOOD,
	SNAKE
};
//蛇的方向
enum Direction {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
    
};
//蛇的状态
enum SnakeState {
	NORMAL = 1,
	DEATH_SELF ,
	DEATH_WALL,

};
//蛇的位置
typedef struct _Snakenode
{
	int x;
	int y;
	struct  _Snakenode * Pnext;
}Snakenode,*PSnakenode;
//蛇的信息
typedef struct  _Snake
{
	PSnakenode psnake;
	int  nDir;
	int state;

}Snake,*PSnake;
//隐藏光标
void HideCursor();
//定点输出
void SetPosOutput(int x, int y);
//颜色设置
void SetConColor(int rgb);
//地图绘制
void GameMap();
//玩家设置
void GameSnake(PSnake pSnake);
//食物设置
void GameFood(PSnake pSnakestate);
//蛇移动
int MoveSnake(PSnake pSnake);
//碰撞检测
int CheckCollision(PSnake pSnake);
//获取键盘输入
void GetInput(PSnake pSnake);
//更新游戏状态
void UpdateGame(PSnake pSnake);
//清空蛇身体
void ClearSnake(PSnake pSnake);
//绘制蛇
void DrawSnake(PSnake pSnake);
//获取随机食物位置
void GenerateFood(int* foodx, int* foody, PSnake pSnake);
//检查是否吃到食物
int CheckFood(PSnake pSnake, int foodx, int foody);
//蛇增长
void GrowSnake(PSnake pSnake, int foodx, int foody);
//游戏状态显示
void ShowGameInfo(PSnake pSnake, int score);








