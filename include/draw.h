#ifndef DRAW_H
#define DRAW_H

#include "game.h"
#include <raylib.h>

#define CELL 16

Vector2 GetFieldCoord(Vector2 mouseWorldPos);
Vector2 GetFieldPos(int x, int y);
Rectangle GetFieldRec(int x, int y);
void DrawMatrix();
void DrawStats();
void UpdateCamera2D(Camera2D *camera);
void ResetCamera2D(Camera2D *camera);

#endif
