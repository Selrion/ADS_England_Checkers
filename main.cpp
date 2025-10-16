#include "board.h"
#include "evaluation.h"
#include <iostream>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Rus");
    // Пример: компьютер играет за белых, игрок за черных
    bool isComputerWhite = true;

    std::cout << "=== ИГРА АНГЛИЙСКИЕ ШАШКИ ===\n";
    std::cout << "Компьютер играет за: " << (isComputerWhite ? "БЕЛЫХ (w/W)" : "ЧЕРНЫХ (b/B)") << "\n";
    std::cout << "Игрок играет за: " << (isComputerWhite ? "ЧЕРНЫХ (b/B)" : "БЕЛЫХ (w/W)") << "\n\n";

    // Создание тестовой доски с начальной позицией
    Board board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY));

    // Расстановка черных (верх доски)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                board[i][j] = BLACK_MAN;
            }
        }
    }

    // Расстановка белых (низ доски)
    for (int i = 5; i < 8; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 == 1) {
                board[i][j] = WHITE_MAN;
            }
        }
    }

    std::cout << "Начальная позиция:\n";
    printBoard(board);

    std::cout << "=== ОЦЕНКИ ПОЗИЦИИ (с точки зрения компьютера) ===\n";
    std::cout << "Базовая оценка: " << evaluate(board, isComputerWhite, EVAL_BASIC) << "\n";
    std::cout << "Позиционная оценка: " << evaluate(board, isComputerWhite, EVAL_POSITIONAL) << "\n";
    std::cout << "Расширенная оценка: " << evaluate(board, isComputerWhite, EVAL_ADVANCED) << "\n";
    std::cout << "Комплексная оценка: " << evaluate(board, isComputerWhite, EVAL_COMPLEX) << "\n";
    std::cout << "\nПоложительное значение = хорошо для компьютера\n";
    std::cout << "Отрицательное значение = хорошо для игрока\n";

    // Тестовая позиция - компьютер имеет преимущество
    Board winningBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY));
    winningBoard[0][1] = isComputerWhite ? BLACK_MAN : WHITE_MAN;
    winningBoard[3][4] = isComputerWhite ? WHITE_KING : BLACK_KING;
    winningBoard[5][2] = isComputerWhite ? WHITE_KING : BLACK_KING;
    winningBoard[2][5] = isComputerWhite ? BLACK_MAN : WHITE_MAN;

    std::cout << "\n\n=== ТЕСТОВАЯ ПОЗИЦИЯ (компьютер имеет преимущество) ===\n";
    printBoard(winningBoard);

    std::cout << "ОЦЕНКИ (с точки зрения компьютера):\n";
    std::cout << "Базовая: " << evaluate(winningBoard, isComputerWhite, EVAL_BASIC) << "\n";
    std::cout << "Позиционная: " << evaluate(winningBoard, isComputerWhite, EVAL_POSITIONAL) << "\n";
    std::cout << "Расширенная: " << evaluate(winningBoard, isComputerWhite, EVAL_ADVANCED) << "\n";
    std::cout << "Комплексная: " << evaluate(winningBoard, isComputerWhite, EVAL_COMPLEX) << "\n";
    return 0;
}