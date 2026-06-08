#include "rblaster.h"
#include <stdlib.h>

struct RB_Canvas *rb_canvas_create(int width, int height) {
	struct RB_Canvas *canvas = malloc(sizeof(struct RB_Canvas));
	if (!canvas) {
		return NULL;
	}

	canvas->height = height;
	canvas->width  = width;
	canvas->pixels = calloc(height * width, sizeof(struct RB_Color));
	if (!canvas->pixels) {
		free(canvas);
		return NULL;
	}
	return canvas;
}

void rb_canvas_free(struct RB_Canvas *canvas) {
	free(canvas->pixels);
	free(canvas);
}
