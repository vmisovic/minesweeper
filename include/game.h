#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef uint8_t field;
#define FLAGED_BIT_MASK 0b01000000
#define OPENED_BIT_MASK 0b00100000
#define MINED_BIT_MASK  0b00010000
#define VALUE_BIT_MASK  0b00001111
enum {
	MINE = 9,
	UNSOLVED,
	FLAG,
	BOOM,
	FALSEF
};

typedef enum {
	WAIT,
	RUNNING,
	SOLVED,
	LOST
} state;

typedef struct {
	int w;
	int h;
	int m;
	field **M;
	int f;
	int o;
	state s;
	time_t t1, t2;
} game;

#define MAX(a, b) (((a) > (b))? (a): (b))
#define MIN(a, b) (((a) < (b))? (a): (b))
#define ABS(a) (((a) > 0)? (a): -(a))

void InitGame(int width, int height, int mines);
void GenerateGame(int x, int y, int seed);
void CloseGame();

void FlagField(int x, int y);
void UnflagField(int x, int y);
void OpenField(int x, int y);
int GetValueOfField(int x, int y);
bool IsFieldOpened(int x, int y);
bool IsFieldFlaged(int x, int y);

int GetGameWidth();
int GetGameHeight();
int GetNumberOfFlags();
int GetNumberOfMines();
int GameElapsedTimeSec();
bool IsGameSolved();
state GetGameState();

#endif
