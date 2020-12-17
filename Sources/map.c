#include "map.h"

game_map_t *create_map() {
    game_map_t *map = allocate(sizeof(game_map_t));
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            map->tiles[y*MAP_WIDTH+x]       = y<1 ? TILE_WALL : TILE_GROUND;
            map->tileColours[y*MAP_WIDTH+x] = 0xffffffff;
        }
    }
    return map;
}

void game_map_render(game_map_t *map, game_graphics_t *graphics) {

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			create_sprite(x*16, y*16, map->tiles[y*MAP_WIDTH+x], graphics);
		}
	}

}