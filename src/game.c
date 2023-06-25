#include "config.h"
#include "game.h"
#include "record.h"

#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>

Mix_Music *g_background_music;
Mix_Chunk *g_mix_music;

//=== SDL ======================================================================

bool initSDL(SDL_Window **window, SDL_Renderer **renderer)
{
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return false;
    }
    
    *window = SDL_CreateWindow(
        "task16: 2048", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "window create error: %s\n", SDL_GetError());
        return false;
    }
    
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "renderer create error: %s\n", SDL_GetError());
        closeSDL(window);
        return false;
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }
    return true;
}

void closeSDL(SDL_Window **window)
{
    SDL_DestroyWindow(*window);
    *window = NULL;
    TTF_Quit();
    SDL_Quit();
}

//=== draw text ================================================================

void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect, SDL_Color color)
{
    SDL_Surface *msg_surface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture *msg_texture = SDL_CreateTextureFromSurface(renderer, msg_surface);
    SDL_Rect msg_rect;

    TTF_SizeText(font, text, &msg_rect.w, &msg_rect.h);
    msg_rect.x = rect.x + rect.w / 2 - msg_rect.w / 2;
    msg_rect.y = rect.y + rect.h / 2 - msg_rect.h / 2;

    SDL_RenderCopy(renderer, msg_texture, NULL, &msg_rect);
    SDL_DestroyTexture(msg_texture);
    SDL_FreeSurface(msg_surface);
}

void draw_rect_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Rect rect)
{
    SDL_Color white = {255, 255, 255, 255};
    draw_text(renderer, font, text, rect, white);
}

void show_text(SDL_Renderer *renderer, const char *text, int size)
{
    TTF_Font *font = TTF_OpenFont(FONT_PATH, size);
    if (!font) {
        fprintf(stderr, "font not found: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Color black = {g_fg.r, g_fg.g, g_fg.b, g_fg.a};
    clear_screen(renderer);
    SDL_Rect rect = {
        SCREEN_PAD, 
        SCREEN_HEIGHT / 4, 
        SCREEN_WIDTH - 2 * SCREEN_PAD, 
        SCREEN_HEIGHT / 2
    };
    draw_text(renderer, font, text, rect, black);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    TTF_CloseFont(font);
}

//=== mouse and key handling ===================================================

void key_handler(SDL_Event e, Board board, SDL_Renderer *renderer)
{
    if (is_game_over(board)) {
        unsigned long score = calc_score(board);
        save_record(score);

        show_text(renderer, "Game Over", GOVER_FONT_SIZE);
        clear_board(board);
        add_random(board);
        return;
    }

    switch (e.key.keysym.sym) {
        case SDLK_UP:
            Mix_PlayChannel(-1, g_mix_music, 0);
            move_y(board, 0);
            break;
        case SDLK_DOWN:
            Mix_PlayChannel(-1, g_mix_music, 0);
            move_y(board, 1);
            break;
        case SDLK_LEFT:
            Mix_PlayChannel(-1, g_mix_music, 0);
            move_x(board, 0);
            break;
        case SDLK_RIGHT:
            Mix_PlayChannel(-1, g_mix_music, 0);
            move_x(board, 1);
            break;
        default:;
    }
}

void mouse_handler(SDL_Event e, Board board)
{
    if (e.button.button != SDL_BUTTON_LEFT) {
        return;
    }

    SDL_Rect btn_rect = {
        SCREEN_PAD,
        SCREEN_PAD,
        (SCREEN_WIDTH - SCREEN_PAD) / 2 - SCREEN_PAD,
        (SCREEN_HEIGHT - SCREEN_WIDTH) - 2 * SCREEN_PAD,
    };
    bool is_btn_click = e.button.x >= (btn_rect.x)               && 
                        e.button.x <= (btn_rect.x + btn_rect.w)  &&
                        e.button.y >= (btn_rect.y)               && 
                        e.button.y <= (btn_rect.y + btn_rect.h);

    if (is_btn_click) {
        clear_board(board);
        add_random(board);
    }
}

//=== render game ==============================================================

void draw_board(SDL_Renderer *renderer, Board board, TTF_Font *font)
{
    int cell_size = (SCREEN_WIDTH - SCREEN_PAD) / SIZE - SCREEN_PAD;
    int panel_offset = (SCREEN_HEIGHT - SCREEN_WIDTH);

    for (int x = 0; x < SIZE; ++x) {
        for (int y = 0; y < SIZE; ++y) {
            SDL_Rect rect = {
                SCREEN_PAD + x * (cell_size + SCREEN_PAD),
                SCREEN_PAD + y * (cell_size + SCREEN_PAD) + panel_offset,
                cell_size,
                cell_size,
            };
            struct COLOR s = g_COLORS[board[y][x]];
            SDL_SetRenderDrawColor(renderer, s.r, s.g, s.b, s.a);
            SDL_RenderFillRect(renderer, &rect);
            char str[15];
            sprintf(str, "%lu", pow_int(BASE, board[y][x]));

            if (board[y][x] == 0) {
                str[0] = ' ';
                str[1] = '\0';
            }
            draw_rect_text(renderer, font, str, rect);
        }
    }
}

void draw_panel(SDL_Renderer *renderer, Board board, TTF_Font *font)
{
    int item_width   = (SCREEN_WIDTH - SCREEN_PAD) / 2 - SCREEN_PAD;
    int panel_height = (SCREEN_HEIGHT - SCREEN_WIDTH) - 2 * SCREEN_PAD;

    { // draw button
        char txt[] = "New Game";
        SDL_Rect rect = {
            SCREEN_PAD,
            SCREEN_PAD,
            item_width,
            panel_height,
        };
        SDL_SetRenderDrawColor(renderer, g_button_bg.r, g_button_bg.g, g_button_bg.b, g_button_bg.a);
        SDL_RenderFillRect(renderer, &rect);
        draw_rect_text(renderer, font, txt, rect);
    }
    
    // draw score

    { // draw record score
        char score_text[30] = "record: ";
        char score[15];
        sprintf(score, "%5lu", get_record());
        strncat(score_text, score, 15);

        SDL_Rect rect = {
            SCREEN_PAD * 2 + item_width,
            SCREEN_PAD,
            item_width,
            panel_height / 2,
        };
        SDL_SetRenderDrawColor(renderer, g_score_bg.r, g_score_bg.g, g_score_bg.b, g_score_bg.a);
        SDL_RenderFillRect(renderer, &rect);
        draw_rect_text(renderer, font, score_text, rect);
    }
    { // draw current score
        char score_text[30] = "score: ";
        char score[15];
        sprintf(score, "%5lu", calc_score(board));
        strncat(score_text, score, 15);

        SDL_Rect rect = { 
            SCREEN_PAD * 2 + item_width,
            SCREEN_PAD + panel_height / 2,
            item_width,
            panel_height / 2,
        };  
        SDL_SetRenderDrawColor(renderer, g_score_bg.r, g_score_bg.g, g_score_bg.b, g_score_bg.a);
        SDL_RenderFillRect(renderer, &rect);
        draw_rect_text(renderer, font, score_text, rect);
    }  

}

void clear_screen(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, g_bg.r, g_bg.g, g_bg.b, g_bg.a);
    SDL_RenderClear(renderer);
}

void render_game(SDL_Renderer *renderer, Board board, TTF_Font *font)
{
    clear_screen(renderer);
    draw_panel(renderer, board, font);
    draw_board(renderer, board, font);
    SDL_RenderPresent(renderer);
}

//=== game =====================================================================

void game_loop(Board board, SDL_Renderer *renderer)
{
    TTF_Font *font = NULL;
    font = TTF_OpenFont(FONT_PATH, CELL_FONT_SIZE);
    if (!font) {
        fprintf(stderr, "font not found: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    render_game(renderer, board, font);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYUP) {
                key_handler(e, board, renderer);
                render_game(renderer, board, font);
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                mouse_handler(e, board);
                render_game(renderer, board, font);
            }
        }
    }
    TTF_CloseFont(font);
}

void game_start()
{
    srand(time(NULL));

    unsigned char board[SIZE][SIZE];
    clear_board(board);
    add_random(board);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (!initSDL(&window, &renderer)) {
        fprintf(stderr, "SDL init error\n");
        exit(EXIT_FAILURE);
    }

    g_background_music = Mix_LoadMUS(BG_MUSIC_PATH);
    g_mix_music = Mix_LoadWAV(MIX_MUSIC_PATH);
    if (g_background_music == NULL || g_mix_music == NULL) {
        fprintf(stderr, "music load error\n");
        exit(EXIT_FAILURE);
    }

    Mix_PlayMusic(g_background_music, -1);
    show_text(renderer, "2048", TITLE_FONT_SIZE);
    game_loop(board, renderer);

    closeSDL(&window);
    Mix_FreeMusic(g_background_music);
    Mix_FreeChunk(g_mix_music);
}
