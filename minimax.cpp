#include "minimax.h"
#include <iostream>
#include <algorithm>

// Проверка, заканчивается ли игра
bool isGameOver(const Board& board) {
    // Игра заканчивается если у одной из сторон нет фигур
    int whitePieces = countPieces(board, true);
    int blackPieces = countPieces(board, false);

    // Также нужно проверить наличие возможных ходов
    std::vector<Move> whiteMoves = generateMoves(board, true);
    std::vector<Move> blackMoves = generateMoves(board, false);

    return (whitePieces == 0 || blackPieces == 0 ||
        (whiteMoves.empty() && blackMoves.empty()));
}

// Определение победителя
int getWinner(const Board& board) {
    int whitePieces = countPieces(board, true);
    int blackPieces = countPieces(board, false);

    if (whitePieces == 0) return -1;  // Белые проиграли, черные выиграли
    if (blackPieces == 0) return 1;   // Черные проиграли, белые выиграли
    return 0;  // Игра еще продолжается
}

// Подсчет фигур одной стороны
int countPieces(const Board& board, bool isWhite) {
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (isWhite && isWhitePiece(board[i][j])) count++;
            else if (!isWhite && isBlackPiece(board[i][j])) count++;
        }
    }
    return count;
}

// Генерация всех возможных ходов для стороны
std::vector<Move> generateMoves(const Board& board, bool isWhite) {
    std::vector<Move> moves;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = board[i][j];

            // Проверяем, принадлежит ли фигура стороне
            if ((isWhite && !isWhitePiece(piece)) ||
                (!isWhite && !isBlackPiece(piece))) {
                continue;
            }

            // Генерируем ходы для этой фигуры
            bool isKing = (piece == WHITE_KING || piece == BLACK_KING);

            // Направления движения
            int directions[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

            for (int d = 0; d < 4; d++) {
                int nextRow = i + directions[d][0];
                int nextCol = j + directions[d][1];

                // Простая шашка не может двигаться назад
                if (!isKing) {
                    if ((isWhite && directions[d][0] > 0) ||
                        (!isWhite && directions[d][0] < 0)) {
                        continue;
                    }
                }

                // Проверка граней доски
                if (nextRow < 0 || nextRow >= BOARD_SIZE ||
                    nextCol < 0 || nextCol >= BOARD_SIZE) {
                    continue;
                }

                // Обычный ход на пустую клетку
                if (board[nextRow][nextCol] == EMPTY &&
                    isPlayableSquare(nextRow, nextCol)) {
                    Move move;
                    move.fromRow = i;
                    move.fromCol = j;
                    move.toRow = nextRow;
                    move.toCol = nextCol;
                    move.isCapture = false;
                    moves.push_back(move);
                }

                // Ход со взятием
                int captureRow = i + 2 * directions[d][0];
                int captureCol = j + 2 * directions[d][1];

                if (captureRow >= 0 && captureRow < BOARD_SIZE &&
                    captureCol >= 0 && captureCol < BOARD_SIZE) {

                    int enemy = board[nextRow][nextCol];
                    bool isEnemy = (isWhite && isBlackPiece(enemy)) ||
                        (!isWhite && isWhitePiece(enemy));

                    if (isEnemy && board[captureRow][captureCol] == EMPTY &&
                        isPlayableSquare(captureRow, captureCol)) {
                        Move move;
                        move.fromRow = i;
                        move.fromCol = j;
                        move.toRow = captureRow;
                        move.toCol = captureCol;
                        move.isCapture = true;
                        moves.push_back(move);
                    }
                }
            }
        }
    }

    return moves;
}

// Проверка валидности хода
bool isValidMove(const Board& board, int fromRow, int fromCol, int toRow, int toCol, bool isWhite) {
    std::vector<Move> moves = generateMoves(board, isWhite);
    for (const auto& move : moves) {
        if (move.fromRow == fromRow && move.fromCol == fromCol &&
            move.toRow == toRow && move.toCol == toCol) {
            return true;
        }
    }
    return false;
}

// Выполнение хода на доске
void makeMove(Board& board, const Move& move) {
    int piece = board[move.fromRow][move.fromCol];
    board[move.fromRow][move.fromCol] = EMPTY;
    board[move.toRow][move.toCol] = piece;

    // Проверка превращения в дамку
    if (piece == WHITE_MAN && move.toRow == 0) {
        board[move.toRow][move.toCol] = WHITE_KING;
    }
    else if (piece == BLACK_MAN && move.toRow == BOARD_SIZE - 1) {
        board[move.toRow][move.toCol] = BLACK_KING;
    }

    // Удаление съеденной фигуры
    if (move.isCapture) {
        int midRow = (move.fromRow + move.toRow) / 2;
        int midCol = (move.fromCol + move.toCol) / 2;
        board[midRow][midCol] = EMPTY;
    }
}

// Отмена последнего хода
void undoMove(Board& board, const Move& move) {
    int piece = board[move.toRow][move.toCol];

    // Возвращаем фигуру на исходную позицию
    board[move.fromRow][move.fromCol] = piece;
    board[move.toRow][move.toCol] = EMPTY;

    // Восстанавливаем оригинальный тип фигуры если было превращение
    if ((piece == WHITE_KING && move.toRow == 0) ||
        (piece == BLACK_KING && move.toRow == BOARD_SIZE - 1)) {
        // Определяем был ли это WHITE_MAN или BLACK_MAN до превращения
        if (piece == WHITE_KING) {
            board[move.fromRow][move.fromCol] = WHITE_MAN;
        }
        else if (piece == BLACK_KING) {
            board[move.fromRow][move.fromCol] = BLACK_MAN;
        }
    }

    // Восстанавливаем съеденную фигуру
    if (move.isCapture) {
        int midRow = (move.fromRow + move.toRow) / 2;
        int midCol = (move.fromCol + move.toCol) / 2;

        // Нужно определить какая фигура была съедена
        // Это требует дополнительной информации в структуре Move
    }
}

// ОСНОВНАЯ ФУНКЦИЯ МИНИМАКСА С АЛЬФА-БЕТА ОТСЕЧЕНИЕМ
int minimax(Board& board, int depth, int alpha, int beta, bool isMaximizing,
    bool isComputerWhite, EvaluationMode evalMode) {

    // Условие выхода: достигнута максимальная глубина или игра закончилась
    if (depth == 0 || isGameOver(board)) {
        return evaluate(board, isComputerWhite, evalMode);
    }

    std::vector<Move> moves = generateMoves(board, isMaximizing ? isComputerWhite : !isComputerWhite);

    if (moves.empty()) {
        return evaluate(board, isComputerWhite, evalMode);
    }

    if (isMaximizing) {
        // Максимизирующий игрок (компьютер)
        int maxEval = INT_MIN;
        for (const auto& move : moves) {
            makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, false, isComputerWhite, evalMode);
            undoMove(board, move);

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            // Альфа-бета отсечение
            if (beta <= alpha) break;
        }
        return maxEval;
    }
    else {
        // Минимизирующий игрок (противник)
        int minEval = INT_MAX;
        for (const auto& move : moves) {
            makeMove(board, move);
            int eval = minimax(board, depth - 1, alpha, beta, true, isComputerWhite, evalMode);
            undoMove(board, move);

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            // Альфа-бета отсечение
            if (beta <= alpha) break;
        }
        return minEval;
    }
}

// Поиск лучшего хода
SearchResult findBestMove(Board& board, int depth, bool isComputerWhite,
    EvaluationMode evalMode) {
    std::vector<Move> moves = generateMoves(board, isComputerWhite);

    SearchResult result;
    result.score = INT_MIN;

    if (moves.empty()) {
        result.bestMove = { -1, -1, -1, -1, false };
        return result;
    }

    for (const auto& move : moves) {
        makeMove(board, move);
        int eval = minimax(board, depth - 1, INT_MIN, INT_MAX, false, isComputerWhite, evalMode);
        undoMove(board, move);

        if (eval > result.score) {
            result.score = eval;
            result.bestMove = move;
        }
    }

    return result;
}

// Вывод хода в консоль
void printMove(const Move& move) {
    std::cout << "Ход: [" << move.fromRow << "," << move.fromCol << "] -> "
        << "[" << move.toRow << "," << move.toCol << "]";
    if (move.isCapture) std::cout << " (взятие)";
    std::cout << "\n";
}