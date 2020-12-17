#include "pch.h"
#include "math.h"

kinc_matrix3x3_t kinc_matrix3x3_multiply(kinc_matrix3x3_t *a, kinc_matrix3x3_t *b) {
	kinc_matrix3x3_t result;
	for (unsigned x = 0; x < 3; ++x)
		for (unsigned y = 0; y < 3; ++y) {
			float t = kinc_matrix3x3_get(a, 0, y) * kinc_matrix3x3_get(b, x, 0);
			for (unsigned i = 1; i < 3; ++i) t += kinc_matrix3x3_get(a, i, y) * kinc_matrix3x3_get(b, x, i);
			kinc_matrix3x3_set(&result, x, y, t);
		}
	return result;
}

