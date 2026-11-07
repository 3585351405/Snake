#include "game.h"

struct Game game = { 0 };

int gFoodx = -1;
int gFoody = -1;


//隐藏光标
void HideCursor() {
	CONSOLE_CURSOR_INFO cursor = { 0 };
	cursor.dwSize = 1;
	cursor.bVisible = FALSE;
	
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

}

void SetPosOutput(int x, int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetConColor(int rgb)
{
	//0 = 黑色       8 = 灰色
	//1 = 蓝色       9 = 淡蓝色
	//2 = 绿色       A = 淡绿色
	//3 = 浅绿色     B = 淡浅绿色
	//4 = 红色       C = 淡红色
	//5 = 紫色       D = 淡紫色
	//6 = 黄色       E = 淡黄色
	//7 = 白色       F = 亮白色
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rgb);
}

void GameMap()
{
	
	SetConColor(4);   
	for (size_t i = 0; i < GAMEROW; i++)
	{
		for (size_t j = 0; j < GAMECOL; j++)
		{
			if(i == 0){
				
				game.state[i][j] = WALL;
				SetPosOutput(j * 2, i);
				printf("■");
			}
			if (i == GAMEROW - 1) {
				SetPosOutput(j * 2, i);
				printf("■");
			}
			if (j == 0) {
				SetPosOutput(j * 2, i);
				printf("■");
			}
			if (j == GAMECOL - 1) {
				SetPosOutput(j * 2, i);
				printf("■");
			}
			
		}

	}
	SetConColor(7);


	SetPosOutput(35 * 2, 4);
	printf("Game Info");


	SetPosOutput(35 * 2, 12);

	printf("W/S/A/D to move or arrow keys");

}

void GameSnake(PSnake pSnakestate)
{
	PSnakenode tmp = NULL;
	for (size_t i = 0; i < 3; i++)
	{
		tmp = malloc(sizeof(Snakenode));
		if (!tmp)return;

		tmp->x = GAMECOL / 2 - i;
		tmp->y = GAMEROW / 2;
		tmp->Pnext = NULL;

		if(pSnakestate->psnake == NULL){
			pSnakestate->psnake = tmp;
		}
		else {
			tmp->Pnext = pSnakestate->psnake;
			pSnakestate->psnake = tmp;
		}



	}
	SetConColor(2);
	PSnakenode Ddraw = pSnakestate->psnake;
	while (Ddraw)
	{
		SetPosOutput(Ddraw->x * 2, Ddraw->y);
		printf("■");
		Ddraw = Ddraw->Pnext;
	}
	SetConColor(7);
}

void GameFood(PSnake pSnakestate)
{
	int foodx;
	int foody;
	


	//随机数种子
	srand(time(NULL));

	while (1) {
		foodx = rand() % 25 + 2;
		foody = rand() % 25 + 2;


		PSnakenode tmp = pSnakestate->psnake;
		int istrue = 1;
		while (tmp) {
			if (foodx == tmp->x && foody == tmp->y) {
				istrue = 0;
				break;
			}


			tmp = tmp->Pnext;
		}
		if (istrue) {
			break;
		}

		
	

	}
	SetConColor(1);
	SetPosOutput(foodx * 2, foody);
	printf("*");
}

//清空蛇身体的显示
void ClearSnake(PSnake pSnake)
{
	PSnakenode tmp = pSnake->psnake;
	while (tmp)
	{
		SetPosOutput(tmp->x * 2, tmp->y);
		printf("  ");
		tmp = tmp->Pnext;
	}
}

//绘制蛇
void DrawSnake(PSnake pSnake)
{
	SetConColor(2);
	PSnakenode tmp = pSnake->psnake;
	while (tmp)
	{
		SetPosOutput(tmp->x * 2, tmp->y);
		printf("■");
		tmp = tmp->Pnext;
	}
	SetConColor(7);
}

//蛇移动
int MoveSnake(PSnake pSnake)
{
	if (!pSnake || !pSnake->psnake) return 0;

	PSnakenode head = pSnake->psnake;
	int newX = head->x;
	int newY = head->y;

	//根据方向更新新的头部位置
	switch (pSnake->nDir)
	{
	case UP:
		newY--;
		break;
	case DOWN:
		newY++;
		break;
	case LEFT:
		newX--;
		break;
	case RIGHT:
		newX++;
		break;
	default:
		//默认向右移动
		newX++;
		break;
	}

	//创建新的头部节点
	PSnakenode newHead = malloc(sizeof(Snakenode));
	if (!newHead) return 0;

	newHead->x = newX;
	newHead->y = newY;
	newHead->Pnext = pSnake->psnake;

	pSnake->psnake = newHead;

	//移除尾部节点（如果没有吃到食物）
	if (gFoodx == -1 || gFoody == -1 || !CheckFood(pSnake, gFoodx, gFoody))
	{
		PSnakenode current = pSnake->psnake;
		while (current->Pnext && current->Pnext->Pnext)
		{
			current = current->Pnext;
		}
		PSnakenode tail = current->Pnext;
		current->Pnext = NULL;
		free(tail);
	}
	else
	{
		//吃到食物，蛇增长
		gFoodx = -1;
		gFoody = -1;
	}

	return 1;
}

//碰撞检测
int CheckCollision(PSnake pSnake)
{
	if (!pSnake || !pSnake->psnake) return NORMAL;

	PSnakenode head = pSnake->psnake;

	//检查墙壁碰撞（墙壁在第0和GAMECOL-1列，第0和GAMEROW-1行）
	if (head->x < 1 || head->x > GAMECOL - 2 || head->y < 1 || head->y > GAMEROW - 2)
	{
		return DEATH_WALL;
	}

	//检查自身碰撞
	PSnakenode body = head->Pnext;
	while (body)
	{
		if (head->x == body->x && head->y == body->y)
		{
			return DEATH_SELF;
		}
		body = body->Pnext;
	}

	return NORMAL;
}

//获取键盘输入
void GetInput(PSnake pSnake)
{
	if (_kbhit())
	{
		int key = _getch();

		//处理方向键（方向键会发送224作为首字符）
		if (key == 224 || key == 0)
		{
			key = _getch();  //获取方向键的实际代码
			switch (key)
			{
			case 72:  //上
				if (pSnake->nDir != DOWN)
					pSnake->nDir = UP;
				break;
			case 80:  //下
				if (pSnake->nDir != UP)
					pSnake->nDir = DOWN;
				break;
			case 75:  //左
				if (pSnake->nDir != RIGHT)
					pSnake->nDir = LEFT;
				break;
			case 77:  //右
				if (pSnake->nDir != LEFT)
					pSnake->nDir = RIGHT;
				break;
			default:
				break;
			}
		}
		else
		{
			//处理普通字符
			switch (key)
			{
			case 'w':
			case 'W':
				if (pSnake->nDir != DOWN)
					pSnake->nDir = UP;
				break;
			case 's':
			case 'S':
				if (pSnake->nDir != UP)
					pSnake->nDir = DOWN;
				break;
			case 'a':
			case 'A':
				if (pSnake->nDir != RIGHT)
					pSnake->nDir = LEFT;
				break;
			case 'd':
			case 'D':
				if (pSnake->nDir != LEFT)
					pSnake->nDir = RIGHT;
				break;
			default:
				break;
			}
		}
	}
}

//获取随机食物位置
void GenerateFood(int* foodx, int* foody, PSnake pSnake)
{
	if (foodx == NULL || foody == NULL) return;

	while (1)
	{
		*foodx = rand() % (GAMECOL - 3) + 2;
		*foody = rand() % (GAMEROW - 3) + 2;

		//检查食物是否与蛇重叠
		PSnakenode tmp = pSnake->psnake;
		int isFree = 1;
		while (tmp)
		{
			if (*foodx == tmp->x && *foody == tmp->y)
			{
				isFree = 0;
				break;
			}
			tmp = tmp->Pnext;
		}

		if (isFree)
		{
			break;
		}
	}
}

//检查是否吃到食物
int CheckFood(PSnake pSnake, int foodx, int foody)
{
	if (!pSnake || !pSnake->psnake) return 0;

	PSnakenode head = pSnake->psnake;
	return (head->x == foodx && head->y == foody) ? 1 : 0;
}

//更新游戏状态
void UpdateGame(PSnake pSnake)
{
	if (!pSnake) return;

	//移动蛇
	MoveSnake(pSnake);

	//检查碰撞
	pSnake->state = CheckCollision(pSnake);

	//如果没有食物，生成新的食物
	if (gFoodx == -1 || gFoody == -1)
	{
		GenerateFood(&gFoodx, &gFoody, pSnake);
	}
}

//游戏状态显示
void ShowGameInfo(PSnake pSnake, int score)
{
	SetConColor(7);
	SetPosOutput(35 * 2, 6);
	printf("Score: %d   ", score);

	SetPosOutput(35 * 2, 8);
	printf("Dir: ");
	switch (pSnake->nDir)
	{
	case UP:
		printf("UP   ");
		break;
	case DOWN:
		printf("DOWN ");
		break;
	case LEFT:
		printf("LEFT ");
		break;
	case RIGHT:
		printf("RIGHT");
		break;
	default:
		break;
	}

	SetPosOutput(35 * 2, 10);
	printf("State: ");
	if (pSnake->state == NORMAL)
	{
		printf("Normal   ");
	}
	else if (pSnake->state == DEATH_WALL)
	{
		printf("Hit Wall");
	}
	else if (pSnake->state == DEATH_SELF)
	{
		printf("Self Hit");
	}
}


