#pragma once
#define MAP_WIDTH 150
#define MAP_HEIGHT 100
#define MAX_ROOMS 60

#include <stdint.h>
#include "graphics.h"
#include "memory.h"
#include "vector.h"
#include "random.h"

#define TILE_GROUND 5
#define TILE_WALL 6
#define TILE_EMPTY 15

typedef struct game_map {
    uint8_t tiles[MAP_WIDTH * MAP_HEIGHT];
    uint32_t tileColours[MAP_WIDTH * MAP_HEIGHT];
} game_map_t;

typedef struct game_map_room {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
} game_map_room_t;

game_map_t *create_map();
void game_map_set_tile(game_map_t *map, int x, int y, int tile);
void game_map_render(game_map_t *map, game_graphics_t *graphics);