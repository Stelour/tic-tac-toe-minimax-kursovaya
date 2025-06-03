#include <stdio.h>
#include <stdlib.h>

void clear_screen(void) {
    printf("\033[2J\033[H");
}

void pause_for_enter(void) {
    printf("Нажмите Enter, чтобы продолжить...");
    while (getchar() != '\n') { }
}

void show_main_menu(void) {
    printf("========================================\n");
    printf("===       Крестики-Нолики (N×N)      ===\n");
    printf("========================================\n");
    printf("1. Игрок vs Игрок\n");
    printf("2. Игрок vs Компьютер\n");
    printf("3. Настройки\n");
    printf("0. Выход\n");
    printf("========================================\n");
    printf("Ваш выбор: ");
}

int read_menu_choice(void) {
    int choice;
    while (1) {
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите корректное число.\n");
            printf("Ваш выбор: ");
            continue;
        }
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        return choice;
    }
}

int read_new_N(void) {
    int N;
    while (1) {
        printf("Введите новое значение N (N ≥ 3): ");
        if (scanf("%d", &N) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите число.\n");
            continue;
        }
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        if (N < 3) {
            printf("Ошибка: N должно быть ≥ 3.\n");
            continue;
        }
        return N;
    }
}

int read_new_K(int current_N) {
    int K;
    while (1) {
        printf("Введите новое значение K (3 ≤ K ≤ %d): ", current_N);
        if (scanf("%d", &K) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите число.\n");
            continue;
        }
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        if (K < 3 || K > current_N) {
            printf("Ошибка: K должно быть в диапазоне [3..%d].\n", current_N);
            continue;
        }
        return K;
    }
}

int read_new_level(void) {
    int lvl;
    while (1) {
        printf("Введите уровень ИИ (1 — Лёгкий, 2 — Средний, 3 — Сложный): ");
        if (scanf("%d", &lvl) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите число.\n");
            continue;
        }
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        if (lvl < 1 || lvl > 3) {
            printf("Ошибка: уровень должен быть 1, 2 или 3.\n");
            continue;
        }
        return lvl;
    }
}

void show_settings_menu(int current_N, int current_K, int current_level) {
    printf("========================================\n");
    printf("===           Настройки игры         ===\n");
    printf("========================================\n");
    printf("Текущий размер поля: N = %d\n", current_N);
    printf("Текущая длина выигрышной линии: K = %d\n", current_K);
    printf("Текущий уровень ИИ: %d (%s)\n",
           current_level,
           current_level == 1 ? "Лёгкий" :
           (current_level == 2 ? "Средний" : "Сложный"));
    printf("----------------------------------------\n");
    printf("1. Изменить N\n");
    printf("2. Изменить K\n");
    printf("3. Изменить уровень ИИ (1–3)\n");
    printf("0. Назад\n");
    printf("========================================\n");
    printf("Ваш выбор: ");
}

int show_end_menu(void) {
    int choice;
    printf("\n========================================\n");
    printf("1. Сыграть ещё раз\n");
    printf("2. В главное меню\n");
    printf("0. Выйти из программы\n");
    printf("========================================\n");
    printf("Ваш выбор: ");
    while (1) {
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите 1, 2 или 0.\n");
            printf("Ваш выбор: ");
            continue;
        }
        int c2;
        while ((c2 = getchar()) != '\n' && c2 != EOF) { }
        if (choice == 0 || choice == 1 || choice == 2) {
            return choice;
        }
        printf("Ошибка: введите 1, 2 или 0.\n");
        printf("Ваш выбор: ");
    }
}

void exit_menu(void) {
    clear_screen();
    printf("========================================\n");
    printf("===           Выход из игры          ===\n");
    printf("========================================\n");
    printf("До новых встреч!\n");
    printf("========================================\n\n");
    pause_for_enter();
}
