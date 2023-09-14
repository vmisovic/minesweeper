#include "draw.h"

#define W GetGameWidth()
#define H GetGameHeight()
#define M GetNumberOfMines()

void HandleClickOnField(int x, int y)
{
	if (x >= 0 && x < W && y >= 0 && y < H){
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			FlagField(x, y);
		if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
			UnflagField(x, y);
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !IsFieldFlaged(x, y))
			OpenField(x, y);
	}
}

void HandleZoom(Camera2D *camera)
{
	float wheel = GetMouseWheelMove();
	if (wheel != 0)
	{
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
		camera->offset = GetMousePosition();
		camera->target = mouseWorldPos;
		const float zoomIncrement = 0.25f;
		camera->zoom += (wheel * zoomIncrement);
	}
}
