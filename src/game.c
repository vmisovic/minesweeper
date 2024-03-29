#include "game.h"

game g = { 0 };

int GetValue(field f)
{
	return (f & VALUE_BIT_MASK);
}

bool IsMine(field f)
{
	return (f & MINED_BIT_MASK);
}

bool IsOpened(field f)
{
	return (f & OPENED_BIT_MASK);
}

bool IsFlaged(field f)
{
	return (f & FLAGED_BIT_MASK);
}

void SetField(field *f, uint8_t mask, bool val)
{
	if (val)
		*f |= mask;
	else
		*f &= ~mask;
}

void InitGame(int width, int height, int mines)
{
	g.w = width;
	g.h = height;
	g.m = mines;
	g.f = 0;
	g.o = 0;
	CloseGame();
	g.M = (field **)malloc(g.h * sizeof(field *));
	if (g.M == NULL) {
		exit(1);
	}
	for (int j = 0; j < g.h; j++){
		g.M[j] = (field *)malloc(g.w * sizeof(field));
		if (g.M[j] == NULL){
			exit(1);
		}
		for (int i = 0; i < g.w; i++)
			g.M[j][i] = 0;
	}
	printf("GAME: Initalized %d * %d map.\n", g.w, g.h);
	g.s = WAIT;
}

void GenerateGame(int x, int y, int seed)
{
	g.f = 0;
	g.o = 0;
	printf("GAME: Placing %d mines (seed: %d)\n", g.m, seed);
	srand(seed);
	for (int j = 0; j < g.h; j++)
		for (int i = 0; i < g.w; i++)
			g.M[j][i] = 0;
	int mines = g.m;
	while (mines > 0){
		int rx = rand() % g.w;
		int ry = rand() % g.h;
		if (!IsMine(g.M[ry][rx]) && !((rx >= (x - 1)) && (rx <= (x + 1)) && (ry >= (y - 1)) && (ry <= (y + 1)))){
			SetField(&g.M[ry][rx], MINED_BIT_MASK, 1);
			for (int i = MAX(rx - 1, 0); i <= MIN(rx + 1, g.w - 1); i++)
				for (int j = MAX(ry - 1, 0); j <= MIN(ry + 1, g.h - 1); j++)
					g.M[j][i]++;
			mines--;
		}
	}
	g.s = RUNNING;
	g.t1 = time(0);
	printf("GAME: Timer started.\n");
}

void CloseGame()
{
	if (g.M == NULL)
		return;
	g.s = WAIT;
	for (int i = 0; i < g.h; i++)
		free(g.M[i]);
	free(g.M);
	g.M = NULL;
	printf("GAME: Closed %d * %d map.\n", g.w, g.h);
}

void FlagField(int x, int y)
{
	if (g.s != RUNNING)
		return;
	field *f = &g.M[y][x];
	if (!IsOpened(*f) && !IsFlaged(*f)){
		// flags++
		g.f++;
		SetField(f, FLAGED_BIT_MASK, 1);
	}
}

void UnflagField(int x, int y)
{
	if (g.s != RUNNING)
		return;
	field *f = &g.M[y][x];
	if (IsFlaged(*f)){
		// flags--
		g.f--;
		SetField(f, FLAGED_BIT_MASK, 0);
	}
}

void OpenField(int x, int y)
{
	field *f = &g.M[y][x];
	switch (g.s)
	{
	case WAIT:
		GenerateGame(x, y, time(0));
	case RUNNING:
		UnflagField(x, y);
		if (IsMine(*f)){
			// game lost
			g.s = LOST;
			g.t2 = time(0);
			printf("GAME: Mine (%d, %d) exploded!\n", x, y);
			printf("GAME: Elapsed time: %ds\n", GameElapsedTimeSec());
			SetField(f, OPENED_BIT_MASK, 1);
			return;
		}
		int val = GetValue(*f);
		if (!IsOpened(*f)){
			// dig field
			g.o++;
			SetField(f, OPENED_BIT_MASK, 1);
			IsGameSolved();
			// dig all zeroes
			if (val == 0)
				for (int i = MAX(x - 1, 0); i <= MIN(x + 1, g.w - 1); i++)
					for (int j = MAX(y - 1, 0); j <= MIN(y + 1, g.h - 1); j++)
						if (!IsOpened(g.M[j][i]) && !(i == x && j == y))
							OpenField(i, j);
		}
		break;
	default:
		break;
	}
}

int GetValueOfField(int x, int y)
{
	field f = g.M[y][x];
	switch (g.s)
	{
	case SOLVED:
		if (!IsFlaged(f) && IsMine(f))
			return MINE;
	case RUNNING:
		if (IsFlaged(f))
			return FLAG;
		if (IsOpened(f))
			return GetValue(f);
		return UNSOLVED;
	case LOST:
		if (IsFlaged(f)){
			if (IsMine(f))
				return FLAG;
			return FALSEF;
		}
		if (IsOpened(f)){
			if (IsMine(f))
				return BOOM;
			return GetValue(f);
		}
		if (IsMine(f))
			return MINE;
	default:
		return UNSOLVED;
	}
}

bool IsFieldOpened(int x, int y)
{
	return IsOpened(g.M[y][x]);
}

bool IsFieldFlaged(int x, int y)
{
	return IsFlaged(g.M[y][x]);
}

int GetGameWidth()
{
	return g.w;
}

int GetGameHeight()
{
	return g.h;
}

int GetNumberOfFlags()
{
	return g.f;
}

int GetNumberOfMines()
{
	return g.m;
}

int GameElapsedTimeSec()
{
	switch (g.s)
	{
	default:
	case WAIT:
		return 0.0f;
	case RUNNING:
		g.t2 = time(0);
	case SOLVED:
	case LOST:
		return g.t2 - g.t1;
	}
}

bool IsGameSolved()
{
	switch (g.s)
	{
	case RUNNING:
		if (g.o + g.m != g.w * g.h)
			return 0;
		for (int i = 0; i < g.w; i++)
			for (int j = 0; j < g.h; j++)
				if (!IsOpened(g.M[j][i]) && !IsMine(g.M[j][i]))
					return 0;
		g.s = SOLVED;
		g.t2 = time(0);
		printf("GAME: Solved in %ds.\n", GameElapsedTimeSec());
	case SOLVED:
		return 1;
	case WAIT:
	case LOST:
	default:
		return 0;
	}
}

state GetGameState()
{
	return g.s;
}
