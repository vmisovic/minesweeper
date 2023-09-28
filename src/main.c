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
int c_w, c_h, c_m;
scene s = WELCOME;

int main(void)
{
	// window
	InitWindow(WIN_WIDTH, WIN_HEIGHT, "test");
	// camera
	ResetCamera2D(&camera);
	// game
	InitGame(w, h, m);
	c_w = w;
	c_h = h;
	c_m = m;
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
	int y;
	switch (s)
	{
	case WELCOME:
		if (IsKeyPressed(KEY_M))
			s = MENU;
		if (IsKeyPressed(KEY_SPACE))
			s = GAME;
		BeginDrawing();
			ClearBackground(VIOLET);
			// name of game / main title
			y = GetScreenHeight() / 3;
			DrawTextCenteredXUnderlined("MINESWEEPER", y, 100, RAYWHITE, PURPLE);
			// ?
			DrawTextCenteredXUnderlined("Press [space] to start solving!", y += 100, 30, RAYWHITE, PURPLE);
			// tip / info
			y = GetScreenHeight() * 2 / 3;
			DrawTextCenteredX("Written in C by Vladimir.", y, 20, PURPLE);
			DrawTextCenteredX("github.com/vmisovic/minesweeper", y += 20, 20, PURPLE);
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
		if (IsKeyPressed(KEY_ENTER)){
			w = c_w;
			h = c_h;
			m = c_m;
			CloseGame();
			InitGame(w, h, m);
			ResetCamera2D(&camera);
			s = GAME;
		}
		BeginDrawing();
			ClearBackground(VIOLET);
			// sub title / menu title
			y = (GetScreenHeight() - 400) / 2;
			DrawTextCenteredXUnderlined("Menu", y, 70, RAYWHITE, PURPLE);
			y += 70;
			if (GetGameState() == RUNNING){
				int sec = GameElapsedTimeSec();
				// info
				DrawTextCenteredX(TextFormat("Clock is ticking! (%02d:%02d)", sec / 60, sec % 60), y, 20, PURPLE);
			}
			// menu options
			y += 30;
			DrawTextCenteredXUnderlined("Close menu [M]", y, 40, WHITE, PURPLE);
			y += 50;
			DrawTextCenteredXUnderlined("Reset [R]", y, 40, WHITE, PURPLE);
			y += 50;
			// start of custom game options
				int x = (GetScreenWidth() - 600) / 2;
				Rectangle cus = { x, y, 600, 200 };
				DrawRectangleLinesEx(cus, 3, PURPLE);
				y += 5;
				DrawTextCenteredXUnderlined("Custom game [enter]", y, 40, WHITE, PURPLE);
				y += 45;
				int dx = (600 - 3 * 100) / 4;
				// width option
				x += dx;
				DrawText("Width", x, y, 20, PURPLE);
				Rectangle wc = { x, y + 20, 100, 100 };
				DrawRectangleLinesEx(wc, 3, PURPLE);
				c_w += GetScrolingOnRec(wc);
				c_w = MAX(c_w, 5);
				c_w = MIN(c_w, 100);
				DrawText(TextFormat("%d", c_w), x + 10, y + 45, 50, WHITE);
				// height option
				x += dx + 100;
				DrawText("Height", x, y, 20, PURPLE);
				Rectangle hc = { x, y + 20, 100, 100 };
				DrawRectangleLinesEx(hc, 3, PURPLE);
				c_h += GetScrolingOnRec(hc);
				c_h = MAX(c_h, 5);
				c_h = MIN(c_h, 100);
				DrawText(TextFormat("%d", c_h), x + 10, y + 45, 50, WHITE);
				// number of mines option
				x += dx + 100;
				DrawText("Mines", x, y, 20, PURPLE);
				Rectangle mc = { x, y + 20, 100, 100 };
				DrawRectangleLinesEx(mc, 3, PURPLE);
				if (c_m < 100)
					c_m += GetScrolingOnRec(mc);
				else
					c_m = c_m + GetScrolingOnRec(mc) * 5;
				c_m = MAX(c_m, c_w * c_h / 20);
				c_m = MIN(c_m, c_w * c_h / 5);
				if (c_m > 100)
					c_m = (c_m / 5) * 5;
				DrawText(TextFormat("%d", c_m), x + 10, y + 45, 50, WHITE);
			// end of custom game options
			// info
			y += 125;
			DrawTextCenteredX("Hover mouse over an option and scroll to change value.", y, 20, PURPLE);
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
				// info
				y = GetScreenHeight() / 3;
				DrawTextCenteredX("Click to start", y, 20, BLACK);
				DrawTextCenteredX("Press [M] for menu", y += 20, 20, BLACK);
				// info on the bottom
				y = GetScreenHeight() * 2 / 3;
				DrawTextCenteredX("Left click opens field.", y, 20, BLACK);
				DrawTextCenteredX("Right click places flag, middle mouse click removes flag.", y += 20, 20, BLACK);
				DrawTextCenteredX("Use scrollwheel to zoom.", y += 20, 20, BLACK);
				DrawTextCenteredX("Hold [left-control] and right mouse button to move.", y += 20, 20, BLACK);
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
