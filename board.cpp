#include "board.h"
#include <iostream>

// Проверка, является ли клетка игровой (темной)
bool isPlayableSquare(int row, int col) {
    return (row + col) % 2 == 1;
}

// Проверка принадлежности фигуры игроку
bool isWhitePiece(int piece) {
    return piece == WHITE_MAN || piece == WHITE_KING;
}

bool isBlackPiece(int piece) {
    return piece == BLACK_MAN || piece == BLACK_KING;
}

// Вывод доски в консоль
void printBoard(const Board& board) {
    std::cout << "  ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        std::cout << j << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << i << " ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            char symbol = '.';
            if (board[i][j] == WHITE_MAN) symbol = 'w';
            else if (board[i][j] == WHITE_KING) symbol = 'W';
            else if (board[i][j] == BLACK_MAN) symbol = 'b';
            else if (board[i][j] == BLACK_KING) symbol = 'B';

            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}