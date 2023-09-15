#ifndef HANDLE_H
#define HANDLE_H

#include "game.h"
#include "draw.h"
#include <raylib.h>
#include <raymath.h>

void HandleClickOnField(Camera2D *camera);
void HandleZoom(Camera2D *camera);
void HandleMoveMap(Camera2D *camera);

#endif
