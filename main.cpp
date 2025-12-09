#include "game.h"
#include <iostream>
#include <locale.h>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Rus");

    std::cout << "=== АНГЛИЙСКИЕ ШАШКИ ===" << std::endl;
    std::cout << "Инициализация игры..." << std::endl;

    CheckersGame game;

    if (!game.initialize()) {
        std::cerr << "ОШИБКА: Не удалось инициализировать игру!" << std::endl;
        std::cerr << "Убедитесь что:" << std::endl;
        std::cerr << "1. Установлены библиотеки SDL2 и SDL2_ttf" << std::endl;
        std::cerr << "2. Файл arial.ttf находится в папке программы" << std::endl;
        return -1;
    }

    std::cout << "Игра успешно запущена!" << std::endl;
    std::cout << "Управление:" << std::endl;
    std::cout << "  - Мышь: клик для выбора фигуры и хода" << std::endl;
    std::cout << "  - N: новая игра" << std::endl;
    std::cout << "  - ESC: пауза/выход" << std::endl;
    std::cout << "  - C: сменить цвет (в меню)" << std::endl;
    std::cout << "  - D: изменить сложность (в меню)" << std::endl;

    try {
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "ОШИБКА во время игры: " << e.what() << std::endl;
        game.cleanup();
        return -1;
    }

    game.cleanup();
    std::cout << "Игра завершена. Спасибо за игру!" << std::endl;

    return 0;
}