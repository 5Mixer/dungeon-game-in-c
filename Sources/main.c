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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "math.h"

static kinc_g4_shader_t vertexShader;
static kinc_g4_shader_t fragmentShader;
static kinc_g4_pipeline_t pipeline;
static kinc_g4_vertex_buffer_t vertices;
static kinc_g4_index_buffer_t indices;
static kinc_g4_texture_t texture;
static kinc_g4_texture_unit_t texunit;
static kinc_g4_constant_location_t offset;

#define HEAP_SIZE 1024 * 1024
static uint8_t *heap = NULL;
static size_t heap_top = 0;

static void *allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	printf("Allocation: heap_top at %d, have used %2.2f%%\n", heap_top, ((double)heap_top/((double)HEAP_SIZE))*100);
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}

void create_sprite(int x, int y, int spriteIndex, float* vertexBuffer, int *vertexBufferOffset, int* indexBuffer, int *indexBufferOffset) {
	const int spriteWidth = 16;
	const int spriteHeight = 16;

	int startIndex = (*vertexBufferOffset)/4;

	// Bottom left
	vertexBuffer[(*vertexBufferOffset)++] = x;// - spriteWidth;
	vertexBuffer[(*vertexBufferOffset)++] = y;// - spriteHeight;
	vertexBuffer[(*vertexBufferOffset)++] = (float)spriteIndex/16.0f;
	vertexBuffer[(*vertexBufferOffset)++] = 1.f/16.0f;

	// Bottom right
	vertexBuffer[(*vertexBufferOffset)++] = x + spriteWidth;
	vertexBuffer[(*vertexBufferOffset)++] = y;// - spriteHeight;
	vertexBuffer[(*vertexBufferOffset)++] = ((float)spriteIndex+1)/16.0f;
	vertexBuffer[(*vertexBufferOffset)++] = 1.f/16.0f;

	// Top right
	vertexBuffer[(*vertexBufferOffset)++] = x + spriteWidth;
	vertexBuffer[(*vertexBufferOffset)++] = y + spriteHeight;
	vertexBuffer[(*vertexBufferOffset)++] = ((float)spriteIndex+1)/16.0f;
	vertexBuffer[(*vertexBufferOffset)++] = 0.f/16.0f;

	// Top left
	vertexBuffer[(*vertexBufferOffset)++] = x;// - spriteWidth;
	vertexBuffer[(*vertexBufferOffset)++] = y + spriteHeight;
	vertexBuffer[(*vertexBufferOffset)++] = (float)spriteIndex/16.0f;
	vertexBuffer[(*vertexBufferOffset)++] = 0.f/16.0f;

	indexBuffer[(*indexBufferOffset)++] = startIndex+0;
	indexBuffer[(*indexBufferOffset)++] = startIndex+1;
	indexBuffer[(*indexBufferOffset)++] = startIndex+2;
	indexBuffer[(*indexBufferOffset)++] = startIndex+0;
	indexBuffer[(*indexBufferOffset)++] = startIndex+2;
	indexBuffer[(*indexBufferOffset)++] = startIndex+3;
}

static void update() {
	kinc_g4_begin(0);
	kinc_g4_clear(KINC_G4_CLEAR_COLOR, 0, 0.0f, 0);

	kinc_g4_set_pipeline(&pipeline);

	// kinc_matrix3x3_t rotation_matrix = kinc_matrix3x_rotation_z((float)kinc_time());

	// float sprite_offset = sin((float)kinc_time()) * 30.f;

	kinc_matrix3x3_t matrix = kinc_matrix3x3_identity();
	const float pixel_scale = 4.f;
	kinc_matrix3x3_set(&matrix, 0, 2, -2.f);
	kinc_matrix3x3_set(&matrix, 1, 2, 2.f);
	kinc_matrix3x3_set(&matrix, 0, 0, pixel_scale/(float)kinc_width());
	kinc_matrix3x3_set(&matrix, 1, 1, -pixel_scale/(float)kinc_height());

	// kinc_matrix3x3_t m = kinc_matrix3x3_multiply(&rotation_matrix, &matrix);
	
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
	kinc_init("TextureTest", 1024, 768, NULL, NULL);
	kinc_set_update_callback(update);

	heap = (uint8_t *)malloc(HEAP_SIZE);
	assert(heap != NULL);

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

		kinc_g4_shader_init(&vertexShader, data, size, KINC_G4_SHADER_TYPE_VERTEX);
	}

	{
		kinc_file_reader_t reader;
		kinc_file_reader_open(&reader, "texture.frag", KINC_FILE_TYPE_ASSET);
		size_t size = kinc_file_reader_size(&reader);
		uint8_t *data = allocate(size);
		kinc_file_reader_read(&reader, data, size);
		kinc_file_reader_close(&reader);

		kinc_g4_shader_init(&fragmentShader, data, size, KINC_G4_SHADER_TYPE_FRAGMENT);
	}

	kinc_g4_vertex_structure_t structure;
	kinc_g4_vertex_structure_init(&structure);
	kinc_g4_vertex_structure_add(&structure, "pos", KINC_G4_VERTEX_DATA_FLOAT2);
	kinc_g4_vertex_structure_add(&structure, "tex", KINC_G4_VERTEX_DATA_FLOAT2);
	kinc_g4_pipeline_init(&pipeline);
	pipeline.input_layout[0] = &structure;
	pipeline.input_layout[1] = NULL;
	pipeline.vertex_shader = &vertexShader;
	pipeline.fragment_shader = &fragmentShader;
	kinc_g4_pipeline_compile(&pipeline);

	texunit = kinc_g4_pipeline_get_texture_unit(&pipeline, "texsampler");
	offset = kinc_g4_pipeline_get_constant_location(&pipeline, "mvp");

	int grid_width = 10;
	int grid_height = 10;
	int sprites = grid_width * grid_height;

	kinc_g4_vertex_buffer_init(&vertices, 4*sprites, &structure, KINC_G4_USAGE_DYNAMIC, 0);
	float *v = kinc_g4_vertex_buffer_lock_all(&vertices);

	kinc_g4_index_buffer_init(&indices, 6*sprites, KINC_G4_INDEX_BUFFER_FORMAT_32BIT);
	int *i = kinc_g4_index_buffer_lock(&indices);

	int *vertexBufferOffset = allocate(sizeof(int));
	int *indexBufferOffset = allocate(sizeof(int));
	*vertexBufferOffset = 0;
	*indexBufferOffset = 0;

	for (int x = 0; x < grid_width; x++) {
		for (int y = 0; y < grid_height; y++) {
			create_sprite(x*16, y*16, y>grid_width/2 ? 5 : 6, v, vertexBufferOffset, i, indexBufferOffset);
		}
	}

	kinc_g4_vertex_buffer_unlock_all(&vertices);
	kinc_g4_index_buffer_unlock(&indices);

	kinc_start();

	free(heap);

	return 0;
}
