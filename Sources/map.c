#include "map.h"

uint8_t can_place_room(game_map_room_t *room, game_map_t *map, game_map_room_t *existingRooms, int existingRoomCount) {
    if (room->x < 1 || room->y < 1 || room->x + room->width > MAP_WIDTH || room->y + room->height > MAP_HEIGHT)
        return 0;

    for (int i = 0; i < existingRoomCount; i++) {
        if (room->x+room->width > existingRooms[i].x && room->x < existingRooms[i].x+existingRooms[i].width &&
            room->y+room->height > existingRooms[i].y && room->y < existingRooms[i].y+existingRooms[i].height) {
            
            return 0;
        }
    }
    return 1;
}
game_map_room_t get_room_extension(game_map_room_t from_room) {
    int extension_side = randi(4);

    game_map_room_t new_room;
    new_room.width = 12+randi(6);
    new_room.height = 12+randi(6);

    // Top
    if (extension_side == 0) {
       new_room.x = from_room.x+(from_room.width/2)-(new_room.width/2); 
       new_room.y = from_room.y-new_room.height;
    }
    // Bottom
    if (extension_side == 1) {
       new_room.x = from_room.x+(from_room.width/2)-(new_room.width/2); 
       new_room.y = from_room.y+from_room.height;
    }
    // Left
    if (extension_side == 2) {
       new_room.x = from_room.x-new_room.width; 
       new_room.y = from_room.y+(from_room.height/2)-(new_room.height/2);
    }
    // Right
    if (extension_side == 3) {
       new_room.x = from_room.x+from_room.width; 
       new_room.y = from_room.y+(from_room.height/2)-(new_room.height/2);
    }

    return new_room;
}

game_map_t *create_map() {

    game_map_t *map = allocate(sizeof(game_map_t));
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            map->tiles[y*MAP_WIDTH+x]       = TILE_EMPTY;
            map->tileColours[y*MAP_WIDTH+x] = 0xffffffff;
        }
    }

    vec2_t startPosition = { MAP_WIDTH/2, MAP_HEIGHT/2 };
    game_map_room_t *rooms = allocate(sizeof(game_map_room_t) * MAX_ROOMS);
    int room_count = 0;
    int failed_iterations = 0;

    game_map_room_t new_room = { startPosition.x, startPosition.y, 12+randi(6), 12+randi(6)};

    while (room_count < MAX_ROOMS && failed_iterations < 400) {
        if (can_place_room(&new_room, map, rooms, room_count)) {
            rooms[room_count++] = new_room;

            // Floor
            for (int x = new_room.x+1; x <= new_room.x + new_room.width-2; x++) {
                for (int y = new_room.y+1; y <= new_room.y + new_room.height-2; y++) {
                    game_map_set_tile(map, x, y, TILE_GROUND);
                }
            }

            // Place horizontal walls
            for (int x = new_room.x; x < new_room.x + new_room.width; x++) {
                game_map_set_tile(map, x, new_room.y, TILE_WALL);
                game_map_set_tile(map, x, new_room.y + new_room.height-1, TILE_WALL);
            }
            // Place vertical walls
            for (int y = new_room.y; y < new_room.y + new_room.height; y++) {
                game_map_set_tile(map, new_room.x, y, TILE_WALL);
                game_map_set_tile(map, new_room.x+new_room.width-1, y, TILE_WALL);
            }
        }else{
            failed_iterations++;
        }

        game_map_room_t from_room = rooms[randi(room_count)];
        new_room = get_room_extension(from_room);
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

void game_map_set_tile(game_map_t *map, int x, int y, int tile) {
    map->tiles[y*MAP_WIDTH+x] = tile;
}