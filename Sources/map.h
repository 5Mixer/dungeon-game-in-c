#pragma once
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

#include <stdint.h>
#include "graphics.h"
#include "memory.h"

#define TILE_GROUND 5
#define TILE_WALL 6

typedef struct game_map {
    uint8_t tiles[MAP_WIDTH * MAP_HEIGHT];
    uint32_t tileColours[MAP_WIDTH * MAP_HEIGHT];
} game_map_t;

game_map_t *create_map();
void game_map_render(game_map_t *map, game_graphics_t *graphics);