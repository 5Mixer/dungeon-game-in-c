#include "pch.h"

#include <kinc/graphics4/graphics.h>
#include <kinc/graphics4/indexbuffer.h>
#include <kinc/graphics4/pipeline.h>
#include <kinc/graphics4/shader.h>
#include <kinc/graphics4/texture.h>
#include <kinc/graphics4/vertexbuffer.h>
#include <kinc/image.h>
#include <kinc/io/filereader.h>
#include <kinc/system.h>

#include "memory.h"
#include <stdio.h>
#include <math.h>

#include "math.h"
#include "map.h"
#include "graphics.h"

static kinc_g4_shader_t vertex_shader;
static kinc_g4_shader_t fragment_shader;
static kinc_g4_pipeline_t pipeline;
static kinc_g4_vertex_buffer_t vertices;
static kinc_g4_index_buffer_t indices;
static kinc_g4_texture_t texture;
static kinc_g4_texture_unit_t texunit;
static kinc_g4_constant_location_t offset;

static void update() {
	kinc_g4_begin(0);
	kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0, 0.0f, 0);

	kinc_g4_set_pipeline(&pipeline);

	kinc_matrix3x3_t matrix = kinc_matrix3x3_identity();
	const float pixel_scale = 1.f;
	kinc_matrix3x3_set(&matrix, 0, 2, -2.f);
	kinc_matrix3x3_set(&matrix, 1, 2, 2.f);
	kinc_matrix3x3_set(&matrix, 0, 0, pixel_scale/(float)kinc_width());
	kinc_matrix3x3_set(&matrix, 1, 1, -pixel_scale/(float)kinc_height());

	kinc_g4_set_matrix3(offset, &matrix);
	kinc_g4_set_vertex_buffer(&vertices);
	kinc_g4_set_index_buffer(&indices);
	kinc_g4_set_texture(texunit, &texture);
	kinc_g4_set_texture_magnification_filter(texunit, KINC_G4_TEXTURE_FILTER_POINT);
	kinc_g4_draw_indexed_vertices();

	kinc_g4_end(0);
	kinc_g4_swap_buffers();
}

int kickstart(int argc, char **argv) {
	srand(time(0));
	kinc_init("TextureTest", 1024, 768, NULL, NULL);
	kinc_set_update_callback(update);

	init_memory();

	{
		kinc_image_t image;
		void *image_mem = allocate(250 * 250 * 4);
		kinc_image_init_from_file(&image, image_mem, "atlas.png");
		kinc_g4_texture_init_from_image(&texture, &image);
		kinc_image_destroy(&image);
	}

	{
		kinc_file_reader_t reader;
		kinc_file_reader_open(&reader, "texture.vert", KINC_FILE_TYPE_ASSET);
		size_t size = kinc_file_reader_size(&reader);
		uint8_t *data = allocate(size);
		kinc_file_reader_read(&reader, data, size);
		kinc_file_reader_close(&reader);

		kinc_g4_shader_init(&vertex_shader, data, size, KINC_G4_SHADER_TYPE_VERTEX);
	}

	{
		kinc_file_reader_t reader;
		kinc_file_reader_open(&reader, "texture.frag", KINC_FILE_TYPE_ASSET);
		size_t size = kinc_file_reader_size(&reader);
		uint8_t *data = allocate(size);
		kinc_file_reader_read(&reader, data, size);
		kinc_file_reader_close(&reader);

		kinc_g4_shader_init(&fragment_shader, data, size, KINC_G4_SHADER_TYPE_FRAGMENT);
	}

	kinc_g4_vertex_structure_t structure;
	kinc_g4_vertex_structure_init(&structure);
	kinc_g4_vertex_structure_add(&structure, "pos", KINC_G4_VERTEX_DATA_FLOAT2);
	kinc_g4_vertex_structure_add(&structure, "tex", KINC_G4_VERTEX_DATA_FLOAT2);
	kinc_g4_pipeline_init(&pipeline);
	pipeline.input_layout[0] = &structure;
	pipeline.input_layout[1] = NULL;
	pipeline.vertex_shader = &vertex_shader;
	pipeline.fragment_shader = &fragment_shader;
	kinc_g4_pipeline_compile(&pipeline);

	texunit = kinc_g4_pipeline_get_texture_unit(&pipeline, "texsampler");
	offset = kinc_g4_pipeline_get_constant_location(&pipeline, "mvp");

	int sprites = 100 * 100 + 10000;
	game_map_t *map = create_map();

	kinc_g4_vertex_buffer_init(&vertices, 4*sprites, &structure, KINC_G4_USAGE_DYNAMIC, 0);
	float *v = kinc_g4_vertex_buffer_lock_all(&vertices);

	kinc_g4_index_buffer_init(&indices, 6*sprites, KINC_G4_INDEX_BUFFER_FORMAT_32BIT);
	int *i = kinc_g4_index_buffer_lock(&indices);

	game_graphics_t *graphics = allocate(sizeof(game_graphics_t));
	graphics->vertex_buffer_offset = 0;
	graphics->index_buffer_offset = 0;
	graphics->vertex_buffer = v;
	graphics->index_buffer = i;

	game_map_render(map, graphics);
	
	kinc_g4_vertex_buffer_unlock_all(&vertices);
	kinc_g4_index_buffer_unlock(&indices);

	kinc_start();

	free_memory();

	return 0;
}
