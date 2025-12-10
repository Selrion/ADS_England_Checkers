#pragma once
#include "board.h"
#include "evaluation.h"
#include <vector>

// Структура для представления хода
struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    bool isCapture;  // Является ли ход взятием
    int capturedPiece;  // Какая фигура была съедена
    int movedPiece;  // Какая фигура двигалась (для отмены превращения)
};

// Структура для результата поиска
struct SearchResult {
    Move bestMove;
    int score;
};

// Функции для работы с ходами
std::vector<Move> generateMoves(const Board& board, bool isWhite);
bool isValidMove(const Board& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite);
void makeMove(Board& board, const Move& move);
void undoMove(Board& board, const Move& move);
int countPieces(const Board& board, bool isWhite);

// Основная функция минимакса с альфа-бета отсечением
int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing,
    bool isComputerWhite, EvaluationMode evalMode);

// Функция поиска лучшего хода
SearchResult findBestMove(Board& board, int depth, bool isComputerWhite,
    EvaluationMode evalMode = EVAL_COMPLEX);

// Утилиты для отладки
void printMove(const Move& move);
bool isGameOver(const Board& board);
int getWinner(const Board& board);  // Возвращает: 1 если белые выиграли, -1 если черные, 0 если игра продолжается