#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef enum {
	WAIT,
	RUNNING,
	SOLVED,
	LOST
} state;

#define FLAGED_BIT_MASK 0b100000
#define OPENED_BIT_MASK 0b010000
#define VALUE_BIT_MASK  0b001111
enum {
	MINE = 9,
	UNSOLVED,
	FLAG,
	BOOM,
	FALSEF
};

typedef uint8_t field;

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

#define MAX(a, b) ((a > b)? a: b)
#define MIN(a, b) ((a < b)? a: b)
#define ABS(a) ((a > 0)? a: -a)

void InitGame(int width, int height);
void StartGame(int mines, int seed);
void CloseGame();

bool FlagField(int x, int y);
bool UnflagField(int x, int y);
int OpenField(int x, int y);
int GetValueOfField(int x, int y);
bool IsFieldOpened(int x, int y);
bool IsFieldFlaged(int x, int y);

int GetGameWidth();
int GetGameHeight();
int GetNumberOfFlags();
int GetNumberOfMines();
int GameElapsedTimeSec();
bool IsGameSolved();
bool IsGameRunning();

#endif
