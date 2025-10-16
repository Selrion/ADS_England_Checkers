#include "evaluation.h"
// Базовая оценочная функция
// Простой подсчет материала: шашки и дамки
int basicEvaluation(const Board& board, bool isWhite) {
    int score = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = board[i][j];

            if (piece == EMPTY) continue;

            int pieceValue = 0;
            if (piece == WHITE_MAN) pieceValue = WEIGHT_MAN;
            else if (piece == WHITE_KING) pieceValue = WEIGHT_KING;
            else if (piece == BLACK_MAN) pieceValue = -WEIGHT_MAN;
            else if (piece == BLACK_KING) pieceValue = -WEIGHT_KING;

            score += pieceValue;
        }
    }

    return isWhite ? score : -score;
}

// Позиционная оценочная функция
// Учитывает позицию фигур на доске
int positionalEvaluation(const Board& board, bool isWhite) {
    int score = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = board[i][j];

            if (piece == EMPTY || !isPlayableSquare(i, j)) continue;

            int pieceScore = 0;
            bool isWhitePiece = (piece == WHITE_MAN || piece == WHITE_KING);

            // Базовая стоимость фигуры
            if (piece == WHITE_MAN || piece == BLACK_MAN) {
                pieceScore = WEIGHT_MAN;
            }
            else {
                pieceScore = WEIGHT_KING;
            }

            // Бонус за заднюю линию (защита от превращения противника)
            if ((isWhitePiece && i == 0) || (!isWhitePiece && i == BOARD_SIZE - 1)) {
                pieceScore += WEIGHT_BACK_ROW;
            }

            // Бонус за центральные клетки (4x4 в центре)
            if (i >= 2 && i <= 5 && j >= 2 && j <= 5) {
                pieceScore += WEIGHT_MIDDLE_BOX;
            }

            // Бонус за средние ряды
            if (i >= 2 && i <= 5) {
                pieceScore += WEIGHT_MIDDLE_TWO;
            }

            // Бонус за продвижение простых шашек
            if (piece == WHITE_MAN) {
                int advancement = BOARD_SIZE - 1 - i;
                pieceScore += advancement * WEIGHT_ADVANCEMENT;
            }
            else if (piece == BLACK_MAN) {
                pieceScore += i * WEIGHT_ADVANCEMENT;
            }

            // Применяем знак в зависимости от цвета
            score += isWhitePiece ? pieceScore : -pieceScore;
        }
    }

    return isWhite ? score : -score;
}

// Расширенная оценочная функция
// Учитывает защиту и уязвимость фигур
int advancedEvaluation(const Board& board, bool isWhite) {
    int score = positionalEvaluation(board, isWhite);

    // Проверка защищенности и уязвимости
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = board[i][j];

            if (piece == EMPTY || !isPlayableSquare(i, j)) continue;

            bool isWhitePiece = (piece == WHITE_MAN || piece == WHITE_KING);
            int adjustment = 0;

            // Проверка защищенности (есть ли дружественная фигура по диагонали сзади)
            int backRow = isWhitePiece ? i - 1 : i + 1;

            bool isProtected = false;
            if (backRow >= 0 && backRow < BOARD_SIZE) {
                if (j - 1 >= 0) {
                    int backPiece = board[backRow][j - 1];
                    if ((isWhitePiece && ::isWhitePiece(backPiece)) ||
                        (!isWhitePiece && ::isBlackPiece(backPiece))) {
                        isProtected = true;
                    }
                }
                if (j + 1 < BOARD_SIZE) {
                    int backPiece = board[backRow][j + 1];
                    if ((isWhitePiece && ::isWhitePiece(backPiece)) ||
                        (!isWhitePiece && ::isBlackPiece(backPiece))) {
                        isProtected = true;
                    }
                }
            }

            if (isProtected) {
                adjustment += WEIGHT_PROTECTED;
            }

            // Проверка уязвимости (может ли противник срубить)
            int frontRow = isWhitePiece ? i + 1 : i - 1;
            int backRowVuln = isWhitePiece ? i - 1 : i + 1;

            if (frontRow >= 0 && frontRow < BOARD_SIZE &&
                backRowVuln >= 0 && backRowVuln < BOARD_SIZE) {

                // Проверка левой диагонали
                if (j - 1 >= 0 && j + 1 < BOARD_SIZE) {
                    int enemyPiece = board[frontRow][j - 1];
                    bool enemyLeft = isWhitePiece ?
                        ::isBlackPiece(enemyPiece) :
                        ::isWhitePiece(enemyPiece);
                    bool emptyRight = board[backRowVuln][j + 1] == EMPTY;

                    if (enemyLeft && emptyRight) {
                        adjustment += WEIGHT_VULNERABLE;
                    }
                }

                // Проверка правой диагонали
                if (j + 1 < BOARD_SIZE && j - 1 >= 0) {
                    int enemyPiece = board[frontRow][j + 1];
                    bool enemyRight = isWhitePiece ?
                        ::isBlackPiece(enemyPiece) :
                        ::isWhitePiece(enemyPiece);
                    bool emptyLeft = board[backRowVuln][j - 1] == EMPTY;

                    if (enemyRight && emptyLeft) {
                        adjustment += WEIGHT_VULNERABLE;
                    }
                }
            }

            score += isWhitePiece ? adjustment : -adjustment;
        }
    }

    return isWhite ? score : -score;
}

// Комплексная оценочная функция
// Самая продвинутая версия с дополнительными эвристиками
int complexEvaluation(const Board& board, bool isWhite) {
    int score = advancedEvaluation(board, isWhite);

    // Подсчет общего количества фигур каждого типа
    int whiteMen = 0, whiteKings = 0, blackMen = 0, blackKings = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == WHITE_MAN) whiteMen++;
            else if (board[i][j] == WHITE_KING) whiteKings++;
            else if (board[i][j] == BLACK_MAN) blackMen++;
            else if (board[i][j] == BLACK_KING) blackKings++;
        }
    }

    // Бонус за численное превосходство в эндшпиле
    int totalPieces = whiteMen + whiteKings + blackMen + blackKings;
    if (totalPieces <= 6) {
        int whitePieces = whiteMen + whiteKings;
        int blackPieces = blackMen + blackKings;
        int pieceDifference = whitePieces - blackPieces;
        score += pieceDifference * 50;
    }

    // Штраф за отсутствие фигур (проигрышная позиция)
    int whiteTotal = whiteMen + whiteKings;
    int blackTotal = blackMen + blackKings;

    if (isWhite && whiteTotal == 0) {
        score -= 10000;
    }
    else if (!isWhite && blackTotal == 0) {
        score += 10000;
    }

    return isWhite ? score : -score;
}

// Универсальная функция оценки с выбором режима
int evaluate(const Board& board, bool isComputerWhite, EvaluationMode mode) {
    switch (mode) {
    case EVAL_BASIC:
        return basicEvaluation(board, isComputerWhite);
    case EVAL_POSITIONAL:
        return positionalEvaluation(board, isComputerWhite);
    case EVAL_ADVANCED:
        return advancedEvaluation(board, isComputerWhite);
    case EVAL_COMPLEX:
    default:
        return complexEvaluation(board, isComputerWhite);
    }
}