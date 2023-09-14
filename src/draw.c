#include "draw.h"

#define W GetGameWidth()
#define H GetGameHeight()
#define M GetNumberOfMines()

const float zoomMax = 5.0f;
const float zoomMin = 0.5f;
const float zoomIncrement = 0.02f;
const int space = 100;
const int step = 10;

Vector2 GetFieldCoord(Vector2 mouseWorldPos)
{
	Vector2 coord;
	coord.x = (int)(mouseWorldPos.x + (float)W * 0.5f * CELL) / CELL;
	coord.y = (int)(mouseWorldPos.y + (float)H * 0.5f * CELL) / CELL;
	return coord;
}

Vector2 GetFieldPos(int x, int y)
{
	Vector2 p;
	p.x = ((float)W * -0.5f + x) * CELL;
	p.y = ((float)H * -0.5f + y) * CELL;
	return p;
}

Rectangle GetFieldRec(int x, int y)
{
	Vector2 p = GetFieldPos(x, y);
	Rectangle r = { p.x, p.y, CELL, CELL };
	return r;
}

void DrawMatrix()
{
	Rectangle r;
	Color c;
	for (int i = 0; i < W; i++){
		for (int j = 0; j < H; j++){
			int val = GetValueOfField(i, j);
			r = GetFieldRec(i, j);
			if (IsFieldOpened(i, j)){
				if ((i + j) % 2)
					c = WHITE;
				else
					c = RAYWHITE;
				if (val == BOOM)
					c = RED;
			}
			else{
				if ((i + j) % 2)
					c = GREEN;
				else
					c = LIME;
			}
			DrawRectangleRec(r, c);
			const char *text;
			switch (val){
			case MINE:
			case BOOM:
				text = "*";
				break;
			case FLAG:
				text = "F";
				break;
			case FALSEF:
				text = "X";
				break;
			default:
				text = TextFormat("%d", val);
				break;
			}
			Color color_array[15] = {BLANK, BLUE, GREEN, RED, YELLOW, SKYBLUE, GOLD, PURPLE, DARKBROWN, BLACK, BLANK, RED, WHITE, RED };
			DrawText(text, r.x + CELL / 4, r.y + CELL / 16, CELL, color_array[val]);
		}
	}
}

void DrawStats()
{
	int fontSize[] = { 30, 20 };
	int y[] = { 10, 45 };
	int sec = GameElapsedTimeSec();
	const char *text[] = {
		TextFormat("%02d / %02d", GetNumberOfFlags(), M),
		TextFormat("%02d:%02d", sec / 60, sec % 60)
	};
	for (int i = 0; i < 2; i++){
		int x = (GetScreenWidth() - MeasureText(text[i], fontSize[i])) / 2;
		DrawText(text[i], x, y[i], fontSize[i], BLACK);
	}
}

void ResetCamera2D(Camera2D *camera)
{
	camera->target.x = 0.0f;
	camera->target.y = 0.0f;
	camera->offset.x = GetScreenWidth() / 2.0f;
	camera->offset.y = GetScreenHeight() / 2.0f;
	camera->zoom = zoomMin;
}

void UpdateCamera2D(Camera2D *camera)
{
	// Checks if zoom is valid
	if (camera->zoom > zoomMax)
		camera->zoom = zoomMax;
	if (camera->zoom < zoomMin)
		camera->zoom = zoomMin;
	// Checks if position is valid
	Vector2 p1 = GetWorldToScreen2D(GetFieldPos(0, 0), *camera);
	Vector2 p2 = GetWorldToScreen2D(GetFieldPos(W, H), *camera);
	float sw = GetScreenWidth();
	float sh = GetScreenHeight();
	float sx, sy, whr = (float)W / (float)H;
	if (sw / sh > whr){
		sy = space;
		sx = sw / 2 - whr * sh / 2 + sy * whr;
	}
	else {
		sx = space;
		sy = sh / 2 - sw / 2 / whr + sx / whr;
	}
	// center if zoomed out
	float cell = (sw - sx * 2) / W;
	if (cell * W > p2.x - p1.x){
		camera->offset.x = sw / 2;
		camera->target.x = 0.0f;
		camera->zoom += zoomIncrement;
	}
	if (cell * H > p2.y - p1.y){
		camera->offset.y = sh / 2;
		camera->target.y = 0.0f;
		camera->zoom += zoomIncrement;
		return;
	}
	// do not move it to mutch
	if (sx + step < p1.x)
		camera->offset.x -= step;
	if (sy + step < p1.y)
		camera->offset.y -= step;
	if (sw - sx - step > p2.x)
		camera->offset.x += step;
	if (sh - sy - step > p2.y)
		camera->offset.y += step;
}
