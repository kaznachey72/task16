#ifndef ALGO_H
#define ALGO_H

#include <stdio.h>
#include <stdbool.h>

#define SIZE 4
#define BASE 2

typedef unsigned char Board[][SIZE];

unsigned long pow_int(int base, int exponent);

void clear_board(Board board);
unsigned long calc_score(Board board);
void add_random(Board board);
bool is_game_over(Board board);

void move_x(Board board, bool opp);
void move_y(Board board, bool opp);

#endif // ALGO_H
