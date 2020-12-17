#pragma once

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

void *allocate(size_t size);
void init_memory();
void free_memory();