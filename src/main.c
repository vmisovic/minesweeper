#include <stdio.h>
#include <raylib.h>
#include "game.h"
#include "draw.h"
#include "handle.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

typedef enum {
	WELCOME,
	MENU,
	GAME
} scene;

int main(void)
{
	// window
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "test");
	SetTargetFPS(60);
	// camera
	Camera2D camera;
	ResetCamera2D(&camera);
	// game
	int w = 20;
	int h = 15;
	int m = 50;
	scene s = WELCOME;
	InitGame(w, h, m);
	// loop
	while (!WindowShouldClose()){
		switch (s)
		{
		case WELCOME:
			if (IsKeyPressed(KEY_M))
				s = MENU;
			if (IsKeyPressed(KEY_SPACE))
				s = GAME;
			BeginDrawing();
				ClearBackground(VIOLET);
				DrawTextCenteredX("WELCOME", 200, 100, BLACK);
			EndDrawing();
			break;
		case MENU:
			if (IsKeyPressed(KEY_M))
				s = GAME;
			BeginDrawing();
				ClearBackground(VIOLET);
				DrawTextCenteredX("Menu", 200, 20, BLACK);
			EndDrawing();
			break;
		case GAME:
			if (IsKeyPressed(KEY_M))
				s = MENU;
			HandleZoom(&camera);
			HandleMoveMap(&camera);
			HandleClickOnField(&camera);
			if (GetGameState() == SOLVED || GetGameState() == LOST)
				// new game
				if (IsKeyPressed(KEY_SPACE)){
					if (IsGameSolved())
						m++;
					else
						m--;
					CloseGame();
					InitGame(w, h, m);
					ResetCamera2D(&camera);
				}
			// update camera to valid position
			UpdateCamera2D(&camera);
			// drawing
			BeginDrawing();
				ClearBackground(VIOLET);
				// if (RUNNING)
				BeginMode2D(camera);
					DrawMatrix();
				EndMode2D();
				if (GetGameState() == WAIT)
					DrawTextCenteredX("Click to start", 200, 20, BLACK);
				DrawStats();
				if (GetGameState() == SOLVED)
					DrawEndMessage("You won!");
				if (GetGameState() == LOST)
					DrawEndMessage("Game over!");
			EndDrawing();
			break;
		}
	}
	// cleanup
	CloseGame();
	CloseWindow();
	return 0;
}
