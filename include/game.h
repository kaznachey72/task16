#ifndef GAME_H
#define GAME_H

#include "algo.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// SDL 
bool initSDL(SDL_Window **window, SDL_Renderer **renderer);
void closeSDL(SDL_Window **window);

// draw text 
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color);
void draw_rect_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect);
void show_text(SDL_Renderer *renderer, const char *text, int size);

// mouse and key handling 
void key_handler(SDL_Event e, Board board, SDL_Renderer *renderer);
void mouse_handler(SDL_Event e, Board board);

// render game 
void draw_board(SDL_Renderer *renderer, Board board, TTF_Font *font);
void draw_panel(SDL_Renderer *renderer, Board board, TTF_Font *font);
void clear_screen(SDL_Renderer *renderer);
void render_game(SDL_Renderer *renderer, Board board, TTF_Font *font);

// game 
void game_loop(Board board, SDL_Renderer *renderer);
void game_start();

#endif // GAME_H
