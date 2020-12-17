#pragma once

typedef struct game_graphics {
    float *vertexBuffer;
    int vertexBufferOffset;

    int *indexBuffer;
    int indexBufferOffset;
} game_graphics_t;

void create_sprite(int x, int y, int spriteIndex, game_graphics_t *graphics);