#include "algo.h"

#include <stdlib.h>
#include <time.h>

unsigned long pow_int(int base, int exponent)
{
	if (base == 2) {
		return 1 << exponent;
	}
	else {
		unsigned long u = 1;
		for (int x = 0; x < exponent; ++x) {
			u *= base;
        }
		return u;
	}
}

void clear_board(Board board)
{
	for (unsigned int x = 0; x < SIZE; ++x) {
	    for (unsigned int y = 0; y < SIZE; ++y) {
			board[x][y] = 0;
		}
	}
}

unsigned long calc_score(Board board)
{
	unsigned long score = 0;
	for (unsigned int x = 0; x < SIZE; ++x) {
		for (unsigned int y = 0; y < SIZE; ++y) {
			if (board[x][y] != 0) {
				score += pow_int(BASE, board[x][y]);
			}
		}
	}
	return score;
}

void add_random(Board board)
{
	unsigned int pos[SIZE * SIZE];
	unsigned int len = 0;
	for (unsigned int x = 0; x < SIZE; ++x) {
		for (unsigned int y = 0; y < SIZE; ++y) {
			if (board[x][y] == 0) {
				pos[len] = x * SIZE + y;
				++len;
			}
		}
	}
	unsigned int i = rand() % len;
    unsigned int row = pos[i] / SIZE;
	unsigned int col = pos[i] % SIZE;
    board[row][col] = 1;
}

bool is_game_over(Board board)
{
	for (unsigned int x = 0; x < SIZE - 1; ++x) {
        bool check_last_row = board[x][SIZE - 1] == board[x + 1][SIZE - 1] || board[x][SIZE - 1] == 0;
        bool check_last_col = board[SIZE - 1][x] == board[SIZE - 1][x + 1] || board[SIZE - 1][x] == 0;
        if (check_last_row || check_last_col) {
            return false;
        }

		for (unsigned int y = 0; y < SIZE - 1; ++y) {
			if (board[x][y] == board[x][y + 1] || board[x][y] == board[x + 1][y] ||	board[x][y] == 0) {
				return false;
			}
		}
	}
	return true;
}

static bool shift_x(Board board, bool opp)
{
	bool is_shifted = false;
	int start = opp ? SIZE - 1 : 0;
    int end   = opp ? -1       : SIZE;
    int inc   = opp ? -1       : 1;
	
    for (int x = 0; x < SIZE; ++x) {
		int i = start;
		for (int y = start; y != end; y += inc) {
			if (board[x][y] != 0) {
				board[x][i] = board[x][y];
				if (i != y) {
					board[x][y] = 0;
					is_shifted = true;
				}
				i += inc;
			}
		}
	}
	return is_shifted;
}

static bool shift_y(Board board, bool opp)
{
    bool is_shifted = false;
    int start = opp ? SIZE - 1 : 0;
    int end   = opp ? -1       : SIZE;
    int inc   = opp ? -1       : 1;

    for (int y = 0; y < SIZE; ++y) {
        int i = start;
        for (int x = start; x != end; x += inc) {
            if (board[x][y] != 0) {
                board[i][y] = board[x][y];

                if (i != x) {
                    board[x][y] = 0;
                    is_shifted = true;
                }
                i += inc;
            }
        }
    }
    return is_shifted;
}

static bool merge_x(Board board, bool opp)
{
	bool is_merged = false;
	int start = opp ? SIZE - 1 : 0;
    int end   = opp ?  0       : SIZE - 1;
    int inc   = opp ? -1       : 1;
	
    for (int x = 0; x < SIZE; ++x) {
		int i = start;
		for (int y = start; y != end; y += inc) {
			if (board[x][y] != 0) {
				if (board[x][y] == board[x][y + inc]) {
					board[x][i] = board[x][y] + 1;
					board[x][y + inc] = 0;
					if (i != y) {
						board[x][y] = 0;
                    }
					is_merged = true;
					i += inc;
				}
				else {
					board[x][i] = board[x][y];
					if (i != y) {
						board[x][y] = 0;
                    }
					i += inc;
				}
			}
		}

		if (board[x][end] != 0) {
			board[x][i] = board[x][end];
			if (i != end) {
				board[x][end] = 0;
            }
		}
	}
	return is_merged;
}

static bool merge_y(Board board, bool opp)
{
	bool is_merged = false;
	int start = opp ? SIZE - 1 : 0;
    int end   = opp ?  0       : SIZE - 1;
    int inc   = opp ? -1       : 1;
	
    for (int y = 0; y < SIZE; ++y) {
		int i = start;
		for (int x = start; x != end; x += inc) {
			if (board[x][y] != 0) {
				if (board[x][y] == board[x + inc][y]) {
					board[i][y] = board[x][y] + 1;
					board[x + inc][y] = 0;
					if (i != x) {
						board[x][y] = 0;
                    }
					i += inc;
					is_merged = true;
				}
				else {
					board[i][y] = board[x][y];
					if (i != x) {
						board[x][y] = 0;
                    }
					i += inc;
				}
			}
		}
		if (board[end][y] != 0) {
			board[i][y] = board[end][y];
			if (i != end)
				board[end][y] = 0;
		}
	}
	return is_merged;
}


void move_x(Board board, bool opp)
{
    bool a = shift_x(board, opp);
    bool b = merge_x(board, opp);
    if (a || b) {
        add_random(board);
    }
}

void move_y(Board board, bool opp)
{
	bool a = shift_y(board, opp);
    bool b = merge_y(board, opp);
	if (a || b) {
		add_random(board);
    }
}
