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

	if (shape.v.y + shape.a.y + pos->y + dy > SIMULATE_HEIGHT) {
		float diff = SIMULATE_HEIGHT - dy - pos->y;
		pos->y = SIMULATE_HEIGHT - dy;
		shape.v.y = sqrtf(shape.v.y * shape.v.y + 2 * shape.a.y * abs(diff));
		cout << "Y1\n";
	} else {
		cout << "Y2\n";
		shape.v.y += shape.a.y;
		pos->y += shape.v.y;
	}
	if (pos->y == SIMULATE_HEIGHT - dy) {
		cout << "Y3\n";
		shape.v.y *= -1;
	}



//	if (pos->y < dx) {
//		pos->y += dy - pos->y;
////		shape.v.y
//		cout << "Y0\n";
//	} else if (pos->y + shape.v.y + dy >= SIMULATE_HEIGHT) {
//		cout << "Y1\n";
//		shape.v.y += shape.a.y * sqrtf(abs((SIMULATE_HEIGHT - dy - pos->y)) / abs(shape.v.y + shape.a.y));
//		if (isnan(shape.v.y)) {
//			system("pause");
//		}
//		pos->y = SIMULATE_HEIGHT - dy;
//		shape.v.y *= -1;
//	} else {
//		cout << "Y2\n";
//		pos->y += shape.v.y;
//		shape.v.y += shape.a.y;
//	}
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
	InitWindow(800, 700, "Sisyphus");
	SetTargetFPS(60);

//	Shape shapes[40] {
//		BALL, {0, 1}, {5, 5}, {{250, 250}, {12.5, 0}},
//	};
	Shape ball ={BALL, {0, 1}, {0, 0}, {{250, 250}, {12 , 0}}};

	SCREEN_WIDTH = GetScreenWidth();
	SCREEN_HEIGHT = GetScreenHeight();
	SIMULATE_WIDTH = SCREEN_WIDTH;
	SIMULATE_HEIGHT = SCREEN_HEIGHT;

	Font font = LoadFont("asset/font/PressStart2P-Regular.ttf");

	bool startSimulation = false;
	bool toggleFullScreen = false;
	bool toggleSlowFPS = false;

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
			if (toggleSlowFPS) SetTargetFPS(1);
			else SetTargetFPS(60);
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

		DrawLineV({150, 250}, {350, 250}, GREEN);
		DrawLineV({150, SIMULATE_HEIGHT}, {350, SIMULATE_HEIGHT}, GREEN);
//		for (auto & shape : shapes) {
//			if (shape.type == BALL) {
//				update(shape);
//				DrawCircleV(shape.pos[0], shape.pos[1].x, WHITE);
//			}
//		}
		if (!startSimulation) {
			if (!IsKeyPressed(KEY_N)) {
				DrawCircleV(ball.pos[0], ball.pos[1].x, WHITE);
				EndDrawing();
				continue;
			}
		}
		update(ball);
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
