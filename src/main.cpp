#include <iostream>
#include <raylib.h>
#include <raymath.h>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
double lastUpdateTime = 0;
int cellSize = 30;
int cellCountWidth = 40;
int cellCountHeight = 30;
int offset = 75;



class Game {
public:
    bool running = true;
    int score = 0;

    Game() {
    }

    ~Game() {
    }

    void Draw() {
    }

    void Update() {
        if(running) {
        }
    }

    void GameOver() {
        running = false;
        score = 0;
    }
};

int main () {
    const int SCREEN_WIDTH = 2*offset + cellSize*cellCountWidth;
    const int SCREEN_HEIGHT = 2*offset + cellSize*cellCountHeight;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Retro Snake");
    SetTargetFPS(60);
    InitAudioDevice();

    Game game = Game();

    while(WindowShouldClose() == false) {
        BeginDrawing();
        if(eventTriggered(0.2)) {
            game.Update();
        }
        
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCountWidth+10, (float)cellSize*cellCountHeight+10}, 5, darkGreen);
        game.Draw();

        DrawText("Snake Game", offset-5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %i", game.score), offset-5, offset+cellSize*cellCountHeight+10, 40, darkGreen);
            
        EndDrawing();

    }
    

    CloseWindow();

    return 0;
}
