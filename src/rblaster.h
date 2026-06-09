#ifndef RASTER_BLASTER_H
#define RASTER_BLASTER_H

#include <stdint.h>

struct RB_Color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct RB_Vec2 {
	int x;
	int y;
};

struct RB_Mat2 {
	float x11;
	float x12;
	float x21;
	float x22;
};

struct RB_Triangle {
	struct RB_Vec2 v0;
	struct RB_Vec2 v1;
	struct RB_Vec2 v2;
};

struct RB_Canvas {
	int height;
	int width;
	struct RB_Color *pixels;
};

struct RB_Canvas *rb_canvas_create(int height, int width);
void rb_canvas_free(struct RB_Canvas *canvas);

void rb_clear_canvas(struct RB_Canvas *canvas);
void rb_draw_triangle(struct RB_Canvas *canvas, const struct RB_Triangle *t);
void rb_rotate_triangle(struct RB_Triangle *t, struct RB_Vec2 center, float angle);

#endif // RASTER_BLASTER_H
