#pragma once
#include "board.h"

// Веса для оценочных функций
const int WEIGHT_MAN = 100;           // Вес простой шашки
const int WEIGHT_KING = 300;          // Вес дамки
const int WEIGHT_BACK_ROW = 10;       // Бонус за защиту задней линии
const int WEIGHT_MIDDLE_BOX = 5;      // Бонус за контроль центра
const int WEIGHT_MIDDLE_TWO = 3;      // Бонус за средние ряды
const int WEIGHT_VULNERABLE = -15;    // Штраф за уязвимую позицию
const int WEIGHT_PROTECTED = 5;       // Бонус за защищенную фигуру
const int WEIGHT_ADVANCEMENT = 2;     // Бонус за продвижение вперед

// Режимы оценки (уровни сложности AI)
enum EvaluationMode {
    EVAL_BASIC,      // Простейшая - для легкого уровня сложности
    EVAL_POSITIONAL, // Средняя - для среднего уровня
    EVAL_ADVANCED,   // Продвинутая - для сложного уровня
    EVAL_COMPLEX     // Максимальная - для экспертного уровня
};

// Оценочные функции
int basicEvaluation(const Board& board, bool isWhite);
int positionalEvaluation(const Board& board, bool isWhite);
int advancedEvaluation(const Board& board, bool isWhite);
int complexEvaluation(const Board& board, bool isWhite);

// Универсальная функция оценки
int evaluate(const Board& board, bool isComputerWhite, EvaluationMode mode = EVAL_COMPLEX);
