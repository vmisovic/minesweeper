#include <stdio.h>
#include <raylib.h>
#include "game.h"
#include "draw.h"
#include "handle.h"

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
	#define WIN_WIDTH 1920
	#define WIN_HEIGHT 1080
#else
	#define WIN_WIDTH 800
	#define WIN_HEIGHT 600
#endif

void UpdateDrawFrame(void);

typedef enum {
	WELCOME,
	MENU,
	GAME
} scene;

Camera2D camera;
int w = 20;
int h = 15;
int m = 50;
scene s = GAME;

int main(void)
{
	// window
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "test");
	// camera
	ResetCamera2D(&camera);
	// game
	InitGame(w, h, m);
	// loop
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
	SetTargetFPS(60);
	while (!WindowShouldClose()){
		UpdateDrawFrame();
	}
#endif
	// cleanup
	CloseGame();
	CloseWindow();
	return 0;
}

void UpdateDrawFrame()
{
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
			DrawTextCenteredX("Menu", 200, 50, BLACK);
			if (GetGameState() == RUNNING){
				int sec = GameElapsedTimeSec();
				DrawTextCenteredX(TextFormat("Clock is ticking! (%02d:%02d)", sec / 60, sec % 60), 250, 20, BLACK);
				DrawTextCenteredX("Reset [R]", 300, 30, BLACK);
				if (IsKeyPressed(KEY_R)){
					CloseGame();
					InitGame(w, h, m);
					ResetCamera2D(&camera);
					s = GAME;
				}
			}
			DrawTextCenteredX("Close menu [M]", 270, 30, BLACK);
			DrawTextCenteredX("Custom game", 330, 30, BLACK);
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
