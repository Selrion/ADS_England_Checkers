#include "board.h"
#include "evaluation.h"
#include "minimax.h"
#include <iostream>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Rus");

    bool isComputerWhite = true;

    std::cout << "=== ИГРА АНГЛИЙСКИЕ ШАШКИ ===" << std::endl;
    std::cout << "Компьютер играет за: " << (isComputerWhite ? "БЕЛЫХ" : "ЧЕРНЫХ") << std::endl;
    std::cout << "Игрок играет за: " << (isComputerWhite ? "ЧЕРНЫХ" : "БЕЛЫХ") << std::endl << std::endl;

    // Создание начальной позиции
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

    std::cout << "Начальная позиция:" << std::endl;
    printBoard(board);

    // ДЕМОНСТРАЦИЯ РАБОТЫ МИНИМАКСА
    std::cout << "=== ТЕСТИРОВАНИЕ МИНИМАКСА ===" << std::endl;
    std::cout << "Поиск лучшего хода для "
        << (isComputerWhite ? "БЕЛЫХ" : "ЧЕРНЫХ") << std::endl;
    std::cout << "Глубина поиска: 3 полухода" << std::endl << std::endl;

    SearchResult result = findBestMove(board, 3, isComputerWhite, EVAL_COMPLEX);

    std::cout << "Лучший найденный ход:" << std::endl;
    printMove(result.bestMove);
    std::cout << "Оценка позиции: " << result.score << std::endl << std::endl;

    // Выполнение найденного хода
    if (result.bestMove.fromRow != -1) {
        std::cout << "Применяем ход..." << std::endl;
        makeMove(board, result.bestMove);
        std::cout << "Позиция после хода:" << std::endl;
        printBoard(board);
    }

    // Демонстрация тестовой позиции
    std::cout << "=== ТЕСТОВАЯ ПОЗИЦИЯ ===" << std::endl;
    Board testBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY));
    testBoard[0][1] = BLACK_MAN;
    testBoard[2][2] = WHITE_KING;
    testBoard[3][5] = WHITE_MAN;
    testBoard[5][4] = BLACK_MAN;
    testBoard[6][7] = WHITE_MAN;

    std::cout << "Позиция:" << std::endl;
    printBoard(testBoard);

    std::cout << "Анализ позиции:" << std::endl;
    std::cout << "Возможных ходов для БЕЛЫХ: "
        << generateMoves(testBoard, true).size() << std::endl;
    std::cout << "Возможных ходов для ЧЕРНЫХ: "
        << generateMoves(testBoard, false).size() << std::endl;

    std::cout << "Оценка позиции (COMPLEX): "
        << evaluate(testBoard, true, EVAL_COMPLEX) << std::endl;

    // Поиск лучшего хода для белых в тестовой позиции
    std::cout << "\nПоиск лучшего хода для БЕЛЫХ в тестовой позиции:" << std::endl;
    SearchResult testResult = findBestMove(testBoard, 3, true, EVAL_COMPLEX);

    if (testResult.bestMove.fromRow != -1) {
        std::cout << "Лучший ход:" << std::endl;
        printMove(testResult.bestMove);
        std::cout << "Оценка после хода: " << testResult.score << std::endl;
    }
    else {
        std::cout << "Нет доступных ходов!" << std::endl;
    }

    return 0;
}