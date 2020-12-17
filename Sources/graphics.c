#include "graphics.h"

void create_sprite(int x, int y, int sprite_index, game_graphics_t *graphics) {
	const int sprite_width = 16;
	const int sprite_height = 16;

	int startIndex = (graphics->vertex_buffer_offset)/4;

	// Bottom left
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = x;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = y + sprite_height;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = (float)sprite_index/16.0f;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = 1.f/16.0f;

	// Bottom right
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = x + sprite_width;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = y + sprite_height;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = ((float)sprite_index+1)/16.0f;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = 1.f/16.0f;

	// Top right
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = x + sprite_width;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = y;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = ((float)sprite_index+1)/16.0f;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = 0.f/16.0f;

	// Top left
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = x;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = y;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = (float)sprite_index/16.0f;
	graphics->vertex_buffer[(graphics->vertex_buffer_offset)++] = 0.f/16.0f;

	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+0;
	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+1;
	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+2;
	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+0;
	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+2;
	graphics->index_buffer[(graphics->index_buffer_offset)++] = startIndex+3;
}