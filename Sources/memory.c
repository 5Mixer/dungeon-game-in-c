#include "memory.h"

#define HEAP_SIZE 1024 * 1024
static uint8_t *heap = NULL;
static size_t heap_top = 0;

void *allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	printf("Allocation: heap_top at %d, have used %2.2f%%\n", heap_top, ((double)heap_top/((double)HEAP_SIZE))*100);
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}
void init_memory() {
	heap = (uint8_t *)malloc(HEAP_SIZE);
	assert(heap != NULL);
}
void free_memory() {
    free(heap);
}