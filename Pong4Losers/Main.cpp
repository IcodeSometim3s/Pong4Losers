#include "raylib.h"
#include <iostream>

struct Ball {
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw() {
		DrawCircle((int)x, (int)y, radius, RED);
	}
};

struct Paddle {
	float x, y;
	float speed;
	float width, height;
	Color Col;

	Rectangle GetRect() {
		return Rectangle{ x - width / 2, y - height / 2 , 10, 100 };
	}

	void Draw() {
		DrawRectangleRec(GetRect(), Col);
	}
};


int main() 
{
	InitWindow(800, 600, "Pong4Losers");
	SetWindowState(FLAG_VSYNC_HINT);

	bool gamePlaying = false;
		
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;
	leftPaddle.Col = SKYBLUE;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;
	rightPaddle.Col = VIOLET;

	const char* winnerText = nullptr;

	const char* Title = "PONG4LOSERS";

	int leftPoints = 0;
	int rightPoint = 0;

	while (!WindowShouldClose()) {
		//checks if game is playing
		if(gamePlaying == true){
		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		//ball collision with walls

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		//controls for paddles
		if (IsKeyDown(KEY_W)) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S)) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		 if (leftPaddle.y < 0 + leftPaddle.height / 2) {
			leftPaddle.y = 0 + leftPaddle.height/2;
		}

		 if (leftPaddle.y > GetScreenHeight() - leftPaddle.height / 2) {
			 leftPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
		 }

		 if (rightPaddle.y < 0 + leftPaddle.height / 2) {
			 rightPaddle.y = 0 + leftPaddle.height / 2;
		 }

		 if (rightPaddle.y > GetScreenHeight() - leftPaddle.height / 2) {
			 rightPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
		 }

		 //ball collision with paddles
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
			
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (leftPaddle.height / 2) * -ball.speedX;

			}
			
		}
		
		//win conditions
		if (ball.x < 0) {
			winnerText = "Right Player Wins !";
		}
		
		if (ball.x > GetScreenWidth()) {
			winnerText = "Left Player Wins !";
		}


		//restarts game, and adds points
		if (winnerText && IsKeyPressed(KEY_SPACE)) {
			if (winnerText == "Left Player Wins !") {
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 300;
				ball.speedY = 300;
				winnerText = nullptr;
				leftPoints++;
			}
			else {
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = -300;
				ball.speedY = 300;
				winnerText = nullptr;
				rightPoint++;
			}
		}

		}


		BeginDrawing(); 

		if (gamePlaying == false) {

			//draws title screen and instructions
			ClearBackground(BLACK);

			int titleWidth = MeasureText(Title, 60);
			DrawText(Title, GetScreenWidth() / 2 - titleWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			DrawText("Press Enter to Play", GetScreenWidth() / 2 - MeasureText("Press Enter to Play", 30) / 2, 0 + ((GetScreenHeight()/2)  + (GetScreenHeight() / 4) - 15), 30, WHITE);

			DrawText("W and D", GetScreenWidth()/3 - MeasureText("W and D", 30), GetScreenHeight()/4, 30, SKYBLUE);

			DrawText("Up and Down", (GetScreenWidth() - (GetScreenWidth() / 5)) - MeasureText("Up and Down", 30), GetScreenHeight() / 4, 30, VIOLET);
			DrawText("Arrows", (GetScreenWidth() - (GetScreenWidth() / 5)) - MeasureText("Arrows", 30), GetScreenHeight() / 4 + 40, 30, VIOLET);
			if (gamePlaying == false && IsKeyPressed(KEY_ENTER)) {
				gamePlaying = true;
			}
		} else if (gamePlaying == true) {
				
			//draws paddles, ball, and center line
				ClearBackground(BLACK);
				DrawRectangle(GetScreenWidth() / 2, 0, 10, GetScreenHeight(), WHITE);
				ball.Draw();
				leftPaddle.Draw();
				rightPaddle.Draw();

				

				//draws left point
				DrawText(TextFormat("%d", leftPoints), (GetScreenWidth() / 4), GetScreenHeight() / 4, 60, SKYBLUE);

				//draws right point
				DrawText(TextFormat("%d", rightPoint), (GetScreenWidth() / 4 + (GetScreenWidth() / 2)), GetScreenHeight() / 4, 60, VIOLET);


				//prints winscreen
				if (winnerText) {
					int winWidth = MeasureText(winnerText, 60);
					DrawText(winnerText, GetScreenWidth() / 2 - winWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
				}
			}
			//prints fps
			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}