#include "rblaster.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define COLOR_RED (struct RB_Color){ .a = 255, .r = 255 }
#define COLOR_BLUE (struct RB_Color){ .a = 255, .b = 255 }
#define COLOR_GREEN (struct RB_Color){ .a = 255, .g = 255 }

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

static inline int rb_max(int a, int b, int c) {
	int max = a;
	if (b > max) max = b;
	if (c > max) max = c;
	return max;
}

static inline int rb_min(int a, int b, int c) {
	int min = a;
	if (b < min) min = b;
	if (c < min) min = c;
	return min;
}

void rb_draw_pixel(struct RB_Canvas *canvas, int x, int y, struct RB_Color color) {
	canvas->pixels[y * canvas->width + x] = color;
}

static inline int rb_vec2_cross(struct RB_Vec2 u, struct RB_Vec2 v) {
	return u.x * v.y - u.y * v.x;
}

static inline struct RB_Vec2 rb_vec2_sub(struct RB_Vec2 u, struct RB_Vec2 v) {
	return (struct RB_Vec2){
		.x = u.x - v.x,
		.y = u.y - v.y
	};
}

void rb_draw_triangle(struct RB_Canvas *canvas, const struct RB_Triangle *t) {
	int min_x = rb_min(t->v0.x, t->v1.x, t->v2.x);
	int min_y = rb_min(t->v0.y, t->v1.y, t->v2.y);
	int max_x = rb_max(t->v0.x, t->v1.x, t->v2.x);
	int max_y = rb_max(t->v0.y, t->v1.y, t->v2.y);

	for (int y = min_y; y <= max_y; y++)
	for (int x = min_x; x <= max_x; x++)
	{
		struct RB_Vec2 p = { .x = x, .y = y };
		int w0 = rb_vec2_cross(
			rb_vec2_sub(t->v1, t->v0), rb_vec2_sub(p, t->v0)
			);
		int w1 = rb_vec2_cross(
			rb_vec2_sub(t->v2, t->v1), rb_vec2_sub(p, t->v1)
			);
		int w2 = rb_vec2_cross(
			rb_vec2_sub(t->v0, t->v2), rb_vec2_sub(p, t->v2)
			);
		bool inside_triangle = w0 > 0 && w1 > 0 && w2 > 0;
		if (inside_triangle) {
			rb_draw_pixel(canvas, x, y, COLOR_RED);
		}
	}
}
