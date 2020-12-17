#pragma once

typedef struct game_graphics {
    float *vertex_buffer;
    int vertex_buffer_offset;

    int *index_buffer;
    int index_buffer_offset;
} game_graphics_t;

void create_sprite(int x, int y, int sprite_index, game_graphics_t *graphics);