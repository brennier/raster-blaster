#include "rblaster.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

void rb_clear_canvas(struct RB_Canvas *canvas) {
	size_t canvas_size = canvas->height * canvas->width * sizeof(struct RB_Color);
	memset(canvas->pixels, 0, canvas_size);
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

float rb_deg_to_rad(float degrees) {
	return degrees * (M_PI / 180.0);
}

void rb_rotate_point(struct RB_Vec2 *p, struct RB_Vec2 center, float angle) {
	int x = p->x - center.x;
	int y = p->y - center.y;
	float radians = rb_deg_to_rad(angle);
	int new_x = x * cos(radians) - y * sin(radians);
	int new_y = x * sin(radians) + y * cos(radians);
	p->x = new_x + center.x;
	p->y = new_y + center.y;
}

void rb_rotate_triangle(struct RB_Triangle *t, struct RB_Vec2 center, float angle) {
	rb_rotate_point(&t->v0, center, angle);
	rb_rotate_point(&t->v1, center, angle);
	rb_rotate_point(&t->v2, center, angle);
}

void rb_draw_triangle(struct RB_Canvas *canvas, const struct RB_Triangle *t) {
	int min_x = rb_min(t->v0.x, t->v1.x, t->v2.x);
	int min_y = rb_min(t->v0.y, t->v1.y, t->v2.y);
	int max_x = rb_max(t->v0.x, t->v1.x, t->v2.x);
	int max_y = rb_max(t->v0.y, t->v1.y, t->v2.y);
	int double_area = rb_vec2_cross(
		rb_vec2_sub(t->v1, t->v0),
		rb_vec2_sub(t->v2, t->v0)
		);

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
		struct RB_Color color = {
			.r = 255 * w0 / double_area,
			.g = 255 * w1 / double_area,
			.b = 255 * w2 / double_area,
			.a = 255,
		};
		bool inside_triangle = w0 > 0 && w1 > 0 && w2 > 0;
		if (inside_triangle) {
			rb_draw_pixel(canvas, x, y, color);
		}
	}
}
