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
scene s = WELCOME;

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
			DrawTextCenteredXUnderlined("MINESWEEPER", 200, 100, RAYWHITE, PURPLE);
			DrawTextCenteredXUnderlined("Press [space] to start solving!", 300, 30, RAYWHITE, PURPLE);
			DrawTextCenteredX("Press [space] to start solving!", 300, 30, RAYWHITE);
			DrawTextCenteredX("Written in C by Vladimir.", 400, 20, PURPLE);
			DrawTextCenteredX("github.com/vmisovic/minesweeper", 420, 20, PURPLE);
		EndDrawing();
		break;
	case MENU:
		if (IsKeyPressed(KEY_M))
			s = GAME;
		if (IsKeyPressed(KEY_R)){
			if (GetGameState() == RUNNING){
				CloseGame();
				InitGame(w, h, m);
			}
			ResetCamera2D(&camera);
			s = GAME;
		}
		BeginDrawing();
			ClearBackground(VIOLET);
			DrawTextCenteredXUnderlined("Menu", 100, 70, RAYWHITE, PURPLE);
			if (GetGameState() == RUNNING){
				int sec = GameElapsedTimeSec();
				DrawTextCenteredX(TextFormat("Clock is ticking! (%02d:%02d)", sec / 60, sec % 60), 170, 20, PURPLE);
			}
			DrawTextCenteredXUnderlined("Close menu [M]", 203, 40, WHITE, PURPLE);
			DrawTextCenteredXUnderlined("Reset [R]", 250, 40, WHITE, PURPLE);
			DrawTextCenteredXUnderlined("Custom game", 310, 40, WHITE, PURPLE);
			Rectangle cus = { 100, 305, 600, 200 };
			Rectangle wc = { 175, 375, 100, 100 };
			Rectangle hc = { 350, 375, 100, 100 };
			Rectangle mc = { 525, 375, 100, 100 };
			DrawRectangleLinesEx(cus, 3, PURPLE);
			DrawRectangleLinesEx(wc, 3, PURPLE);
			DrawRectangleLinesEx(hc, 3, PURPLE);
			DrawRectangleLinesEx(mc, 3, PURPLE);
			DrawText("Width", 175, 355, 20, PURPLE);
			DrawText("Height",350, 355, 20, PURPLE);
			DrawText("Mines", 525, 355, 20, PURPLE);
			DrawTextCenteredX("Press [enter] to start custom game.", 480, 20, PURPLE);
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
			if (GetGameState() == WAIT){
				DrawTextCenteredX("Click to start", 200, 20, BLACK);
				DrawTextCenteredX("Press [M] for menu", 220, 20, BLACK);
				DrawTextCenteredX("Left click opens field.", 400, 20, BLACK);
				DrawTextCenteredX("Right click places flag, middle mouse click removes flag.", 420, 20, BLACK);
				DrawTextCenteredX("Use scrollwheel to zoom.", 440, 20, BLACK);
				DrawTextCenteredX("Hold [left-control] and right mouse button to move.", 460, 20, BLACK);
			}
			DrawStats();
			if (GetGameState() == SOLVED)
				DrawEndMessage("You won!");
			if (GetGameState() == LOST)
				DrawEndMessage("Game over!");
		EndDrawing();
		break;
	}
}
