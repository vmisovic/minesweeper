#ifndef DRAW_H
#define DRAW_H

#include "game.h"
#include <raylib.h>

#define CELL 20

Vector2 GetFieldPos(int x, int y);
Rectangle GetFieldRec(int x, int y);
void DrawTextCenteredX(const char *text, int posY, int fontSize, Color color);
void DrawMatrix();
void DrawStats();
void DrawEndMessage(const char *message);
void UpdateCamera2D(Camera2D *camera);
void ResetCamera2D(Camera2D *camera);

#endif
