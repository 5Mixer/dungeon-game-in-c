#include "graphics.h"

void create_sprite(int x, int y, int spriteIndex, game_graphics_t *graphics) {
	const int spriteWidth = 16;
	const int spriteHeight = 16;

	int startIndex = (graphics->vertexBufferOffset)/4;

	// Bottom left
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = x;// - spriteWidth;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = y;// - spriteHeight;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = (float)spriteIndex/16.0f;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = 1.f/16.0f;

	// Bottom right
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = x + spriteWidth;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = y;// - spriteHeight;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = ((float)spriteIndex+1)/16.0f;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = 1.f/16.0f;

	// Top right
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = x + spriteWidth;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = y + spriteHeight;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = ((float)spriteIndex+1)/16.0f;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = 0.f/16.0f;

	// Top left
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = x;// - spriteWidth;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = y + spriteHeight;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = (float)spriteIndex/16.0f;
	graphics->vertexBuffer[(graphics->vertexBufferOffset)++] = 0.f/16.0f;

	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+0;
	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+1;
	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+2;
	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+0;
	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+2;
	graphics->indexBuffer[(graphics->indexBufferOffset)++] = startIndex+3;
}