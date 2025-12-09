#pragma once
#include "board.h"
#include "minimax.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

// Константы окна
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int BOARD_OFFSET_X = 50;
const int BOARD_OFFSET_Y = 50;
const int CELL_SIZE = 80;

// Цвета
struct Color {
    Uint8 r, g, b, a;
};

const Color COLOR_WHITE_CELL = { 240, 217, 181, 255 };
const Color COLOR_BLACK_CELL = { 181, 136, 99, 255 };
const Color COLOR_HIGHLIGHT = { 255, 255, 0, 200 };
const Color COLOR_VALID_MOVE = { 0, 255, 0, 150 };
const Color COLOR_SELECTED = { 255, 200, 0, 200 };
const Color COLOR_BACKGROUND = { 50, 50, 50, 255 };
const Color COLOR_TEXT = { 255, 255, 255, 255 };

// Состояния игры
enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_PAUSED
};

// Настройки игры
struct GameSettings {
    bool isPlayerWhite;
    int aiDifficulty;  // 1-4 (соответствует глубине поиска)
    EvaluationMode evalMode;
};

// Класс игры
class CheckersGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* largeFont;

    Board board;
    GameState state;
    GameSettings settings;

    bool isWhiteTurn;
    int selectedRow, selectedCol;
    bool pieceSelected;
    std::vector<Move> validMoves;

    std::string statusMessage;
    int moveCount;
    bool gameRunning;

    // История ходов для отображения
    struct MoveHistory {
        Move move;
        std::string description;
    };
    std::vector<MoveHistory> moveHistory;

public:
    CheckersGame();
    ~CheckersGame();

    bool initialize();
    void run();
    void cleanup();

private:
    // Основные методы
    void handleEvents();
    void update();
    void render();

    // Обработка событий
    void handleMouseClick(int x, int y);
    void handleKeyPress(SDL_Keycode key);

    // Игровая логика
    void startNewGame();
    void makePlayerMove(int fromRow, int fromCol, int toRow, int toCol);
    void makeComputerMove();
    void selectPiece(int row, int col);
    void deselectPiece();
    bool isValidSquare(int row, int col);
    void checkGameOver();

    // Преобразование координат
    bool screenToBoard(int x, int y, int& row, int& col);
    void boardToScreen(int row, int col, int& x, int& y);

    // Отрисовка
    void renderMenu();
    void renderGame();
    void renderGameOver();
    void renderBoard();
    void renderPieces();
    void renderHighlights();
    void renderUI();
    void renderText(const std::string& text, int x, int y, TTF_Font* font = nullptr);
    void renderButton(const std::string& text, int x, int y, int w, int h);

    // Вспомогательные методы
    void setDrawColor(const Color& color);
    std::string moveToString(const Move& move);
    void updateValidMoves();
};

// Вспомогательные функции
std::string difficultyToString(int difficulty);
std::string evalModeToString(EvaluationMode mode);