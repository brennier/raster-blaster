#include <stdlib.h>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "rblaster.h"

#define WINDOW_TITLE  "Raster Blaster"
#define WINDOW_SCALE  5
#define WINDOW_WIDTH  100
#define WINDOW_HEIGHT 100
#define APP_VERSION   "0.1"

struct rb_app {
	struct RB_Canvas *rb_canvas;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *screen_texture;
	SDL_Event event;
	uint64_t old_time;
};

bool rb_app_setup(struct rb_app *app) {
	app->old_time = SDL_GetPerformanceCounter();

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
		return false;
	}

	app->window = SDL_CreateWindow(
		WINDOW_TITLE,
		WINDOW_SCALE * WINDOW_WIDTH,
		WINDOW_SCALE * WINDOW_HEIGHT,
		0);
	if (!app->window) {
		fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
		return false;
	}

	app->renderer = SDL_CreateRenderer(app->window, NULL);
	if (!app->renderer) {
		fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
		return false;
	}

	if (!SDL_SetRenderVSync(app->renderer, 1)) {
		fprintf(stderr, "Error failed to set vsync: %s\n", SDL_GetError());
		return false;
	}

	app->screen_texture = SDL_CreateTexture(app->renderer,
						SDL_PIXELFORMAT_XBGR8888,
						SDL_TEXTUREACCESS_STREAMING,
						WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureScaleMode(app->screen_texture, SDL_SCALEMODE_NEAREST);
	if (!app->screen_texture) {
		fprintf(stderr, "Error creating screen texture: %s\n", SDL_GetError());
		return false;
	}

	app->rb_canvas = rb_canvas_create(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!app->rb_canvas) {
		fprintf(stderr, "Error creating the raster blaster canvas\n");
		return false;
	}

	return true;
}

void rb_app_cleanup(struct rb_app *app) {
	printf("Cleaning up!\n");
	SDL_DestroyTexture(app->screen_texture);
	SDL_DestroyRenderer(app->renderer);
	SDL_DestroyWindow(app->window);
	SDL_Quit();
}

void rb_app_handle_keypress(struct rb_app *app, SDL_Scancode scancode, bool is_pressed) {
	switch (scancode) {

	default: return;
	}
}

void rb_app_handle_events(struct rb_app *app) {
	while (SDL_PollEvent(&app->event)) {
		switch (app->event.type) {

		case SDL_EVENT_QUIT:
			rb_app_cleanup(app);
			exit(0);
			break;
		case SDL_EVENT_KEY_DOWN:
			rb_app_handle_keypress(app, app->event.key.scancode, true);
			break;
		case SDL_EVENT_KEY_UP:
			rb_app_handle_keypress(app, app->event.key.scancode, false);
			break;
		default:
			break;
		}
	}
}

float rotation = 0.0f;
struct RB_Triangle original_triangle = {
	.v0 = { .x = 20, .y = 20 },
	.v1 = { .x = 70, .y = 50 },
	.v2 = { .x = 30, .y = 80 },
};

void main_loop(void* arg) {
	struct rb_app *app = (struct rb_app *)arg;
	rb_app_handle_events(app);

	Uint64 now = SDL_GetPerformanceCounter();
	double delta_time = (double)(now - app->old_time) / SDL_GetPerformanceFrequency();
	app->old_time = now;

	rotation += delta_time * 10.0f;
	struct RB_Triangle triangle = original_triangle;
	struct RB_Vec2 screen_center = {
		.x = WINDOW_WIDTH  / 2,
		.y = WINDOW_HEIGHT / 2
	};
	rb_clear_canvas(app->rb_canvas);
	rb_rotate_triangle(&triangle, screen_center, rotation);
	rb_draw_triangle(app->rb_canvas, &triangle);

	size_t canvas_row_size = WINDOW_WIDTH * sizeof(uint32_t);
	SDL_UpdateTexture(app->screen_texture, NULL, app->rb_canvas->pixels, canvas_row_size);
	SDL_RenderTexture(app->renderer, app->screen_texture, NULL, NULL);
	SDL_RenderPresent(app->renderer);
}

int main(int argc, char *argv[]) {
	struct rb_app app = { 0 };
	rb_app_setup(&app);
	printf("Application started successfully\n");

	while (true) main_loop(&app);

	rb_app_cleanup(&app);
	return 0;
}
