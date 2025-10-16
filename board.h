#pragma once

#include <vector>

// Константы для представления фигур
const int EMPTY = 0;
const int WHITE_MAN = 1;      // Белая простая шашка
const int WHITE_KING = 2;     // Белая дамка
const int BLACK_MAN = 3;      // Черная простая шашка
const int BLACK_KING = 4;     // Черная дамка

// Размер доски 8x8
const int BOARD_SIZE = 8;

// Тип для представления доски
using Board = std::vector<std::vector<int>>;

// Вспомогательные функции для работы с доской
bool isPlayableSquare(int row, int col);
bool isWhitePiece(int piece);
bool isBlackPiece(int piece);
void printBoard(const Board& board);
