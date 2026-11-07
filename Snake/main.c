#include "game.h"
#include <conio.h>


int main() {

	Snake snake = { 0 };
	int score = 0;
	int frameCount = 0;
	const int SPEED = 2;

	HideCursor();
	system("cls");

	GameMap();
	GameSnake(&snake);

	snake.nDir = RIGHT;
	snake.state = NORMAL;

	//先显示蛇
	DrawSnake(&snake);
	Sleep(500);

	while (snake.state == NORMAL)
	{
		GetInput(&snake);

		frameCount++;
		if (frameCount > SPEED)  //改为 > 而非 >=，这样第一帧不会立即移动
		{
			frameCount = 1;

			ClearSnake(&snake);

			UpdateGame(&snake);

			DrawSnake(&snake);

			if (gFoodx != -1 && gFoody != -1)
			{
				SetConColor(1);
				SetPosOutput(gFoodx * 2, gFoody);
				printf("*");
				SetConColor(7);
			}

			if (CheckFood(&snake, gFoodx, gFoody))
			{
				score += 10;
				gFoodx = -1;
				gFoody = -1;
			}
		}

		ShowGameInfo(&snake, score);

		Sleep(50);
	}

	SetPosOutput(10, GAMEROW + 2);
	SetConColor(4);
	printf("Game Over! Score: %d", score);

	if (snake.state == DEATH_WALL)
	{
		printf(" (Hit Wall)");
	}
	else if (snake.state == DEATH_SELF)
	{
		printf(" (Self Collision)");
	}

	SetPosOutput(0, GAMEROW + 3);
	SetConColor(7);
	printf("Press any key to exit...");
	_getch();  //等待单个按键，不会拦截游戏输入

	PSnakenode tmp = snake.psnake;
	while (tmp)
	{
		PSnakenode next = tmp->Pnext;
		free(tmp);
		tmp = next;
	}

	return 0;
}