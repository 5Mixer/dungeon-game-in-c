#include "random.h"

int randi(int max) {
    return (int)floor(max*(float)rand()/(float)RAND_MAX);
}