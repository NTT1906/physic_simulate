#include <iostream>
#include <cmath>
#include "raylib.h"
#include "raymath.h"

#include <ctime>

using namespace std;

enum ShapeType{
	BALL,
	BOX
};

struct Shape{
	ShapeType type;
	Vector2 a; //acceleration
	Vector2 v; //velocity, speed
	Vector2 pos[3];
};

float SCREEN_HEIGHT = 0.0f;
float SCREEN_WIDTH = 0.0f;
float SIMULATE_HEIGHT = 0.0f;
float SIMULATE_WIDTH = 0.0f;

bool isCollided(Shape &shape);

float yMin, yMax;

float sign(float in) {
	return in > 0.0f ? 1.0f : -1.0f;
}

const int dotMax = 100;
Vector2 dots[dotMax];
int dotSize = 0;

void pushDot(Vector2 v) {
	for (int i = dotSize; i >= 1; i--) {
		dots[i] = dots[i - 1];
	}
	dots[0] = v;
	if (dotSize < dotMax - 1) dotSize++;
}

void update(Shape &shape) {
	Vector2 *pos = &shape.pos[0];
//	if (shape.type == BALL) {
//		if (pos->x + shape.pos[1].x + shape.v.x){}
//	}
	float dx{0.0f}, dy{0.0f};
	if (shape.type == BALL) {
		dx = shape.pos[1].x;
		dy = dx;
	}
//	if (pos->y <= dy || pos->y + dy >= SIMULATE_HEIGHT - 1) {
//		shape.v.y *= -1;
//	} else {
//		shape.v.y += shape.a.y;
//	}
	if (shape.v.x + shape.a.x / 2 + pos->x + dx > SIMULATE_WIDTH) {
		float diff = SIMULATE_WIDTH - dx - pos->x;
		pos->x = SIMULATE_WIDTH - dx;
		shape.v.x = sqrtf(shape.v.x * shape.v.x + 2 * shape.a.x * abs(diff));
		cout << "X1A\n";
	} else if (shape.v.x + shape.a.x / 2 + pos->x < dx) {
		float diff = pos->x - dx;
		pos->x = dx;
		shape.v.x = sqrtf(shape.v.x * shape.v.x + 2 * shape.a.x * abs(diff));
		cout << "X1B\n";
	} else {
		cout << "X2\n";
		pos->x += shape.v.x + shape.a.x / 2;
		if (shape.v.x < 0 && shape.v.x + shape.a.x >= 0) {
			yMin = pos->x;
		}
		shape.v.x += shape.a.x;
	}
	if (pos->x == SIMULATE_WIDTH - dx) {
		cout << "X3\n";
		shape.v.x *= -1;
	}

	if (shape.v.y + shape.a.y / 2 + pos->y + dy > SIMULATE_HEIGHT) {
		float diff = SIMULATE_HEIGHT - dy - pos->y;
		pos->y = SIMULATE_HEIGHT - dy;
		shape.v.y = sign(shape.v.y) * sqrtf(shape.v.y * shape.v.y + 2 * shape.a.y * abs(diff));
		cout << "Y1A\n";
	} else if (shape.v.y + shape.a.y / 2 + pos->y < dy) {
		float diff = pos->y - dy;
		pos->y = dy;
		shape.v.y = -sign(shape.v.y + shape.a.y / 2.0f) * sqrtf(shape.v.y * shape.v.y + 2 * shape.a.y * abs(diff));
		cout << "Y1B\n";
	} else {
		cout << "Y2\n";
		pos->y += shape.v.y + shape.a.y / 2;
		if (shape.v.y < 0 && shape.v.y + shape.a.y >= 0) {
			yMin = pos->y;
		}
		shape.v.y += shape.a.y;
	}
	if (pos->y == SIMULATE_HEIGHT - dy) {
		cout << "Y3A\n";
		shape.v.y *= -1;
	}
}

char signFormat(float in) {
	return in < 0 ? '-' : ' ';
}

void formatDebug(char* stringStream, char* preFormat, float in) {
	sprintf(stringStream, "%s%c% 10.3f", preFormat, signFormat(in), abs(in));
}

int main() {
//	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 700, "Classic 1");
	SetTargetFPS(60);

//	Shape shapes[40] {
//		BALL, {0, 1}, {5, 5}, {{250, 250}, {12.5, 0}},
//	};
	Shape ball ={BALL, {0, 1}, {10, 30}, {{250, 250}, {20 , 0}}};

	SCREEN_WIDTH = GetScreenWidth();
	SCREEN_HEIGHT = GetScreenHeight();
	SIMULATE_WIDTH = SCREEN_WIDTH;
	SIMULATE_HEIGHT = SCREEN_HEIGHT;

	Font font = LoadFont("asset/font/PressStart2P-Regular.ttf");

	bool startSimulation = false;
	bool toggleFullScreen = false;
	bool toggleSlowFPS = false;
	yMin = 250 + 1;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(GetColor(0x1e1e1eff));
		if (IsKeyPressed(KEY_F)) {
			toggleFullScreen = !toggleFullScreen;
			if (toggleFullScreen) MaximizeWindow();
			else RestoreWindow();
		}
		if (IsKeyPressed(KEY_SPACE)) {
			startSimulation = !startSimulation;
		}
		if (IsKeyPressed(KEY_S)) {
			toggleSlowFPS = !toggleSlowFPS;
			if (toggleSlowFPS) SetTargetFPS(2);
			else SetTargetFPS(60);
		}
		if (IsKeyDown(KEY_UP)) {
			ball.pos[1].x++;
			ball.v.x *= 1 - 1 / (ball.pos[1].x);
		}
		if (IsKeyDown(KEY_DOWN)) {
			if (ball.pos[1].x > 0) ball.pos[1].x--;
			ball.v.x *= 1 + 1 / (ball.pos[1].x);
		}
		char temp[40];
		int posY = 0;
		int fontSize = 10;
		formatDebug(temp, "Vx: ", ball.v.x);
		DrawTextEx(font, temp, {0, posY}, fontSize, 0, WHITE);
		posY += fontSize;
		formatDebug(temp, "Vy: ", ball.v.y);
		DrawTextEx(font, temp, {0, posY}, fontSize, 0, WHITE);
		posY += fontSize;
		formatDebug(temp, "X:  ", ball.pos[0].x);
		DrawTextEx(font, temp, {0, posY}, fontSize, 0, WHITE);
		posY += fontSize;
		formatDebug(temp, "Y:  ", ball.pos[0].y);
		DrawTextEx(font, temp, {0, posY}, fontSize, 0, WHITE);
		posY += fontSize;
		formatDebug(temp, "Ymi:", yMin);
		DrawTextEx(font, temp, {0, posY}, fontSize, 0, WHITE);
		posY += fontSize;

		DrawLineV({150, 250}, {350, 250}, GREEN);
		DrawLineV({150, SIMULATE_HEIGHT}, {350, SIMULATE_HEIGHT}, GREEN);
//		for (auto & shape : shapes) {
//			if (shape.type == BALL) {
//				update(shape);
//				DrawCircleV(shape.pos[0], shape.pos[1].x, WHITE);
//			}
//		}
		for (int i = 0; i < dotSize; i++) {
//			DrawCircleLinesV(dots[i], 20, GRAY);
			int s = min(dotSize, 256) + i % max(dotMax, 256);
//			DrawCircleLinesV(dots[i], ball.pos[1].x, Color{(unsigned char) s, (unsigned char) s, (unsigned char) 255, 255});
			DrawPixelV(dots[i], Color{(unsigned char) (100 + i % 56), (unsigned char) (100 + i % 56), (unsigned char) 255, 255});
		}
		if (!startSimulation) {
			if (!IsKeyPressed(KEY_N)) {
				DrawCircleV(ball.pos[0], ball.pos[1].x, WHITE);
				EndDrawing();
				continue;
			}
		}
		update(ball);
		pushDot(ball.pos[0]);

		if (ball.pos[0].y + ball.pos[1].x > SIMULATE_HEIGHT) {
			cout << "Invalid!\n";
		}
		DrawCircleV(ball.pos[0], ball.pos[1].x, WHITE);
		EndDrawing();
	}
	UnloadFont(font);
	CloseWindow();
	return 0;
}
