#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <raymath.h>
#include "game.h"
#include "draw.h"
#include "handle.h"
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

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
	InitGame(w, h);
	StartGame(m, time(0));
	// loop
	while (!WindowShouldClose()){
		// check if game is solved
		IsGameSolved();
		// handle user input
		// handel zoom
		HandleZoom(&camera);
		// handle move
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT)){
			//HandleMoveMap();
			Vector2 delta = GetMouseDelta();
			delta = Vector2Scale(delta, -1.0f / camera.zoom);
			camera.target = Vector2Add(camera.target, delta);
		}
		else {
			if (IsGameRunning()){
				Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
				Vector2 coord = GetFieldCoord(mouseWorldPos);
				HandleClickOnField(coord.x, coord.y);
			}
			else {
				// new game
				if (IsKeyPressed(KEY_SPACE)){
					if (IsGameSolved())
						m++;
					else
						m--;
					StartGame(m, time(0));
					ResetCamera2D(&camera);
				}
			}
		}
		// update camera to valid position
		UpdateCamera2D(&camera);
		// drawing
		BeginDrawing();
			ClearBackground(VIOLET);
			BeginMode2D(camera);
				DrawMatrix();
			EndMode2D();
			if (!IsGameRunning()){
				if (IsGameSolved()){
					// DrawWinMessage()
					DrawText("You won!", 190, 240, 100, BLACK);
					DrawText("You won!", 210, 260, 100, BLACK);
					DrawText("You won!", 210, 240, 100, BLACK);
					DrawText("You won!", 190, 260, 100, BLACK);
					DrawText("You won!", 200, 250, 100, YELLOW);
				}
				else {
					// DrawLosMessage()
					DrawText("Game over!", 130, 250, 100, BLACK);
				}
				// click left to continue
				DrawText("click [space] to continue", 250, 350, 20, BLACK);
			}
			DrawStats();
		EndDrawing();
	}
	// cleanup
	CloseGame();
	CloseWindow();
	return 0;
}
