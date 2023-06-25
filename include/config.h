#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH  620
#define SCREEN_HEIGHT 720
#define SCREEN_PAD    12

#define FONT_PATH "ARCADE.TTF"
#define TITLE_FONT_SIZE 200
#define GOVER_FONT_SIZE 100
#define CELL_FONT_SIZE  40

#define MIX_MUSIC_PATH "mixkit-game-action.wav"
#define BG_MUSIC_PATH  "solve-the_puzzle.ogg"

#define RECORD_FILE_PATH "reocord_score.bin"

struct COLOR
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

static const struct COLOR g_bg = {211, 204, 201, 255};
static const struct COLOR g_fg = { 80,  80,  80, 255};
static const struct COLOR g_button_bg = {  0, 162,   0, 255};
static const struct COLOR g_score_bg  = {122, 122, 122, 255};
static const struct COLOR g_COLORS[] = {
    {230, 227, 232, 255},
    {255, 127,  89, 255},
    {224,  74,  69, 255},
    {237, 207, 114, 255},
    { 65, 216, 127, 255},
    { 54,  63, 135, 255},
    { 78,  89, 178, 255},
    {109, 118, 191, 255},
    { 84,  47, 132, 255},
    {125,  77, 188, 255},
    {163,  77, 188, 255},
    {176, 109, 196, 255},
    {  0, 102, 204, 255},
    {  0, 153, 255, 255},
    { 51, 153, 255, 255},
    {153, 204, 255, 255},
    {102, 255, 102, 255}
};

#endif // CONFIG_H
