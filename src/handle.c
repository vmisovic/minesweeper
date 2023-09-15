#include "handle.h"

#define W GetGameWidth()
#define H GetGameHeight()

void HandleClickOnField(Camera2D *camera)
{
	if (IsKeyDown(KEY_LEFT_SHIFT))
		return;
	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
	int x = (int)(mouseWorldPos.x + (float)W * 0.5f * CELL) / CELL;
	int y = (int)(mouseWorldPos.y + (float)H * 0.5f * CELL) / CELL;
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

void HandleMoveMap(Camera2D *camera)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_SHIFT)){
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / camera->zoom);
		camera->target = Vector2Add(camera->target, delta);
	}
}
