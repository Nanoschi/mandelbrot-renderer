#include <iostream>
#include "mandelbrot.h"
#include "raylib.h"

//#define SHOW_PROGRESS

class Renderer {
	// Display
	Image mb_image;
	Texture mb_texture;
	Color set_color;
	Color bg_color;
	Color progress_color;

	double screen_x_offset, screen_y_offset;
	int win_width, win_height;
	int image_width, image_height;

	// Generation
	double zoom;

public:
	Renderer(int _win_width, int _win_height) {
		win_width = _win_width;
		win_height = _win_height;
		image_width = 500;
		image_height = 500;

		screen_x_offset = 0.5 * image_width;
		screen_y_offset = 0.5 * image_height;

		mb_image = GenImageColor(image_width, image_height, PINK);

		set_color = { 230, 34, 171, 255 };
		bg_color = { 11, 9, 33, 255 };
		progress_color = WHITE;

		zoom = 300;

	}

	~Renderer() {
		UnloadImage(mb_image);
		UnloadTexture(mb_texture);
	}

	void run() {
		InitWindow(win_width, win_height, "Mandelbrot Renderer");
		render_texture();

		while (!WindowShouldClose()) {
			BeginDrawing();
			
			get_input();

			DrawTexture(mb_texture, 0, 0, WHITE);
			Vector2 pos = { 0, 0 };
			DrawTextureEx(mb_texture, pos, 0, win_width / image_width, WHITE);

			EndDrawing();
		}

		CloseWindow();
	}

private:
	void get_input() {
		double zooom_amount = 1.5;
		double move_amount = 50;

		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			zooom_amount = 3;
			move_amount = 300;
		}

		if (IsKeyPressed(KEY_KP_ADD)) {
			zoom *= zooom_amount;
			render_texture();
		}
		else if (IsKeyPressed(KEY_KP_SUBTRACT)) {
			zoom /= zooom_amount;
			render_texture();
		}

		if (IsKeyPressed(KEY_LEFT)) {
			screen_x_offset += move_amount;
			render_texture();
		}
		else if (IsKeyPressed(KEY_RIGHT)) {
			screen_x_offset -= move_amount;
			render_texture();
		}
		else if (IsKeyPressed(KEY_UP)) {
			screen_y_offset += move_amount;
			render_texture();
		}
		else if (IsKeyPressed(KEY_DOWN)) {
			screen_y_offset -= move_amount;
			render_texture();
		}

		/*if (IsKeyPressed(KEY_PERIOD)) {
			ClearBackground(bg_color);
			image_width *= 1.5;
			image_height *= 1.5;
			screen_x_offset = 0.5 * image_width;
			screen_y_offset = 0.5 * image_height;
			render_texture();
		}
		if (IsKeyPressed(KEY_COMMA)) {
			ClearBackground(bg_color);
			image_width /= 1.5;
			image_height /= 1.5;
			screen_x_offset = 0.5 * image_width;
			screen_y_offset = 0.5 * image_height;
			render_texture();
		}*/
	}
	void render_texture() {
		Color render_color = {0, 0, 0, 255};
		int total_pixels = image_width * image_height;
	   

		for (int y = 0; y < image_height; y++) {
			for (int x = 0; x < image_width; x++) {
				get_pixel_color(x, y, render_color);
				ImageDrawPixel(&mb_image, x, y, render_color);
			}

#ifdef SHOW_PROGRESS
			if (y % 10 == 0) {
				printf("%.0lf%%\n", (double)(y * image_width) / total_pixels * 100);
			}
#endif
		}

		UnloadTexture(mb_texture);
		mb_texture = LoadTextureFromImage(mb_image);
	}

	void get_pixel_color(int x, int y, Color& target_color) {
		double world_x = (x - screen_x_offset) / zoom;
		double world_y = (y - screen_y_offset) / zoom;
		auto complex_coord = std::complex<double>(world_x, world_y);
		double on_set = is_value_on_set(complex_coord);

		target_color = blend_colors(bg_color, set_color, on_set);
	}

	Color blend_colors(Color color1, Color color2, double weight) {
		Color result;
		result.r = (color1.r * (1 - weight) + color2.r * weight);
		result.g = (color1.g * (1 - weight) + color2.g * weight);
		result.b = (color1.b * (1 - weight) + color2.b * weight);
		result.a = 255;
		return result;
	}

	

};


int main() {
	Renderer renderer(1000, 1000);

	renderer.run();
}