#include <iostream>
#include "mandelbrot.h"
#include "raylib.h"

struct MbTexture {
	Image image;
	Texture2D texture;
	int width, height;

private:
	Color background_color;
	Color set_color;

	int coordinate_x_offset, coordinate_y_offset;
	double zoom;
	int render_steps;

public:
	MbTexture() = default;

	MbTexture(int _width, int _height, int _steps) {
		width = _width;
		height = _height;
		coordinate_x_offset = width / 2;
		coordinate_y_offset = height / 2;
		zoom = 200;

		background_color = { 0, 0, 0, 255 };
		set_color = { 255, 0, 0, 255 };

		image = GenImageColor(width, height, background_color);
		render_steps = _steps;
		
		if (IsWindowReady()) {
			texture = LoadTextureFromImage(image);
		}
		else {
			std::cout << "Trying to load texture before initialising window" << std::endl;
		}
	}

	MbTexture(int _width, int _height) : MbTexture(_width, _height, 25) {
		// This constructor delegates to the first constructor with _steps = 25
	}

	~MbTexture() {
		UnloadImage(image);
		UnloadTexture(texture);
	}

	void render() {
		render(render_steps);
	}

	void render(int steps) {
		render_steps = steps;

		for (int y = 0; y < width; y++) {
			for (int x = 0; x < height; x++) {
				Color pixel_color = get_pixel_color(x, y, steps);
				ImageDrawPixel(&image, x, y, pixel_color);
			}
		}

		UnloadTexture(texture);
		texture = LoadTextureFromImage(image);
	}

	void draw(int x_pos, int y_pos, double scale) {
		Vector2 pos = { x_pos, y_pos };
		DrawTextureEx(texture, pos, 0, scale, WHITE);
	}

	void zoom_int(double factor) {
		zoom *= factor;
		render(render_steps);
	}

	void zoom_out(double factor) {
		zoom /= factor;
		render(render_steps);
	}

	void move_horizontal(int amount) {
		coordinate_x_offset += amount;
		render(render_steps);
	}

	void move_vertical(int amount) {
		coordinate_y_offset += amount;
		render(render_steps);
	}

private:
	Color get_pixel_color(int x, int y, int steps) {
		// Calculating value at world coordinate
		double world_x = (x - coordinate_x_offset) / zoom;
		double world_y = (y - coordinate_y_offset) / zoom;

		auto complex_coordinate = std::complex<double>(world_x, world_y);
		double result = is_value_on_set(complex_coordinate, steps);

		// Calculate color
		return blend_colors(background_color, set_color, result);

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

class App {
	int win_width, win_height;

public:
	App(int _win_width, int _win_height) {
		win_width = _win_width;
		win_height = _win_height;
	}

	void run() {
		InitWindow(win_width, win_height, "Mandelbrot");
		MbTexture mbt(333, 333, 20);
		mbt.render();

		while (!WindowShouldClose()) {
			BeginDrawing();

			get_input(mbt);
			mbt.draw(0, 0, 3);

			EndDrawing();
		}

		CloseWindow();
	}

private:
	void get_input(MbTexture& mbt) {
		int move_amount = 50;
		double zoom_amount = 1.5;

		if (IsKeyDown(KEY_LEFT_CONTROL)) {
			move_amount = 150;
			zoom_amount = 3;
		}

		if (IsKeyPressed(KEY_LEFT)) {
			mbt.move_horizontal(move_amount);
		}
		else if (IsKeyPressed(KEY_RIGHT)) {
			mbt.move_horizontal(-move_amount);
		}
		else if (IsKeyPressed(KEY_UP)) {
			mbt.move_vertical(move_amount);
		}
		else if (IsKeyPressed(KEY_DOWN)) {
			mbt.move_vertical(-move_amount);
		}

		if (IsKeyPressed(KEY_KP_ADD)) {
			mbt.zoom_int(zoom_amount);
		}
		else if (IsKeyPressed(KEY_KP_SUBTRACT)) {
			mbt.zoom_out(zoom_amount);
		}
	}
};


int main() {
	App app(1000, 1000);
	app.run();
}