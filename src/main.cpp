#include <raylib.h>
#include <raymath.h>
#include <array>
using namespace std;

Color background = {0, 0, 0, 255};
Color uiColor = {255, 255, 255, 255};
Color genColor0 = {255, 5, 5, 255};
Color genColor1 = {43, 255, 255, 255};
Color genColor2 = {255, 43, 255, 255};
Color genColor3 = {255, 255, 43, 255};
Color genColor4 = {43, 255, 43, 255};
Color genColor5 = {43, 43, 255, 255};
Color genColor6 = {255, 43, 43, 255};
Color genColor7 = {128, 255, 43, 255};
Color genColor8 = {43, 255, 125, 255};
Color genColor9 = {43, 125, 255, 255};
Color genColor10 = {43, 125, 25, 255};
double lastUpdateTime = 0;
const int cellSize = 10;
const int cellCountWidth = 120;
const int cellCountHeight = 90;
const int offset = 75;


bool eventTriggered(double interval) {
    double currentTime = GetTime();

    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

struct Cell {
    int generation;
    bool alive;
};

class Board {
private:
    array<array<Cell, cellCountHeight>, cellCountWidth> grid;
    int generation = 0;
    int liveCells = 0;
    int startCells = 0;

    void GenerateGrid(bool populate) {
        int randomNumber = 0;
        startCells = 0;
        liveCells = 0;

        for(int x=0; x<cellCountWidth; x++) {
            for(int y=0; y<cellCountHeight; y++) {
                grid[x][y].generation = 0;
                if(populate) {
                    randomNumber = rand() % 101;
                    if(randomNumber <= 40) {
                        grid[x][y].alive = true;
                        startCells++;
                        liveCells++;
                    } else {
                         grid[x][y].alive = false;
                    }
                } else {
                    grid[x][y].alive = false;
                }
            }
        }
    }
    
    Color GenerationColor(int generation) {

        switch (generation) {
            case 0:
                return genColor0;
                break;
            case 1 ... 100:
                return genColor1;
                break;
            case 101 ... 200:
                return genColor2;
                break;
            case 201 ... 300:
                return genColor3;
                break;
            case 301 ... 400:
                return genColor4;
                break;
            case 401 ... 500:
                return genColor5;
                break;
            case 501 ... 600:
                return genColor6;
                break;
            case 601 ... 700:
                return genColor7;
                break;
            case 701 ... 800:
                return genColor8;
                break;
            case 801 ... 900:
                return genColor9;
                break;
            default:
                return genColor10;
                break;
        }
    }

public:
    Board(const bool populate) {
        GenerateGrid(populate);
    }

    void Draw() {
        for(int x=0; x<cellCountWidth; x++) {
            for(int y=0; y<cellCountHeight; y++) {
                if(grid[x][y].alive) {
                    DrawRectangle(x*cellSize+offset, y*cellSize+offset, cellSize, cellSize, GenerationColor(grid[x][y].generation));
                }
            }
        }
    }

    void Update() {
        array<array<Cell, cellCountHeight>, cellCountWidth> prevGrid = grid;
        
        generation++;
        
        int cellsAlive = 0;
        
        liveCells = 0;

        for(int x=0; x<cellCountWidth; x++) {
            for(int y=0; y<cellCountHeight; y++) {
                    // Count neighboring cells that are alive
                    cellsAlive = prevGrid[x-1][y-1].alive + prevGrid[x-1][y+1].alive + prevGrid[x-1][y].alive
                               + prevGrid[x+0][y-1].alive + prevGrid[x+0][y+1].alive + prevGrid[x+1][y].alive
                               + prevGrid[x+1][y-1].alive + prevGrid[x+1][y+1].alive;
                if(grid[x][y].alive && (cellsAlive < 2 || cellsAlive > 3)) {
                    // cell died out of isolation/overcrowdness
                    grid[x][y].alive = false;
                } else if(!grid[x][y].alive && cellsAlive == 3) {
                    // cell rebirth
                    grid[x][y].alive = true;
                    grid[x][y].generation = generation;
                }
                if(grid[x][y].alive) {
                    liveCells++;
                }
            }
        }
    }

    int GetLiveCells() {
        return liveCells;
    }

    int GetStartCells() {
        return startCells;
    }
};

class Game {
public:
    bool running = true;
    bool paused = true;
    int score = 0;
    float lapTime = 0.0f;
    Board board;

    Game() : board(true) {
        board = Board(true);
    }

    ~Game() {
    }

    void Draw() {
        board.Draw();
    }

    void Update() {
        if(running && !paused) {
            // each update cicle is a new generation
            score++;
            lapTime += GetFrameTime();
            if(eventTriggered(0.075)) {
                board.Update();
            }
        }
    }
    
    void Restart() {
        board = Board(true);
        score = 0;
        paused = true;
    }
    
    void PauseToggle() {
        paused = !paused;
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

    Game game = Game();

    while(WindowShouldClose() == false) {

        game.Update();
        
        if(IsKeyPressed(KEY_R)) {
            game.Restart();
        }
        if(IsKeyPressed(KEY_SPACE)) {
            game.PauseToggle();
        }

        BeginDrawing();
        
        ClearBackground(background);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCountWidth+10, (float)cellSize*cellCountHeight+10}, 5, uiColor);
        game.Draw();

        DrawText("Conway's Game of Life", offset-5, 20, 40, uiColor);
        // Board Info
        // Current Generation
        DrawText(TextFormat("Generation: %i", game.score), offset-5, offset+cellSize*cellCountHeight+10, 25, uiColor);
        // Elapsed time
        DrawText(TextFormat("Elapsed Time: %i", (int)game.lapTime), offset-5, offset+cellSize*cellCountHeight+35, 25, uiColor);
        // Starting cells
        DrawText(TextFormat("Live Cell Count: %i", game.board.GetLiveCells()), offset+350, offset+cellSize*cellCountHeight+35, 25, uiColor);
        DrawText(TextFormat("Start Cell Count: %i", game.board.GetStartCells()), offset+350, offset+cellSize*cellCountHeight+10, 25, uiColor);
        DrawText(TextFormat("[R]estart - [P]lay / Pause"), cellSize*cellCountWidth-200, offset+cellSize*cellCountHeight+10, 20, uiColor);
        DrawText(TextFormat("[Esc] Exit Game"), cellSize*cellCountWidth-200, offset+cellSize*cellCountHeight+40, 20, uiColor);
            
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
