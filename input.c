#include <stdio.h>

void clear_screen(void)
{
    printf("\033[2J\033[H");
}

void pause_for_enter(void)
{
    printf("Нажмите Enter, чтобы продолжить...");
    while (getchar() != '\n') { }
}

void show_main_menu(void)
{
    printf("========================================\n");
    printf("===      Игра «Крестики-Нолики»      ===\n");
    printf("========================================\n");
    printf("1. Игра человек—человек (PvP)\n");
    printf("2. Игра человек—компьютер (PvC)\n");
    printf("3. Настройки (Settings)\n");
    printf("0. Выход\n");
    printf("========================================\n");
    printf("Введите номер пункта: ");
}

int read_menu_choice(void)
{
    int choice;
    if (scanf("%d", &choice) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return -1;
    }
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    return choice;
}

void show_settings_menu(int current_N, int current_K)
{
    printf("========================================\n");
    printf("===           Настройки             ===\n");
    printf("========================================\n");
    printf("Текущие параметры:\n");
    printf("1. Размер поля N = %d (минимум 3)\n", current_N);
    printf("2. Длина выигрышной линии K = %d (3 ≤ K ≤ N)\n", current_K);
    printf("0. Назад в главное меню\n");
    printf("========================================\n");
    printf("Введите номер пункта: ");
}

int read_new_N(void)
{
    int N;
    while (1) {
        printf("Введите новое значение N (целое ≥ 3): ");
        if (scanf("%d", &N) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите целое число.\n");
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        if (N < 3) {
            printf("Ошибка: N должно быть ≥ 3.\n");
            continue;
        }
        return N;
    }
}

int read_new_K(int current_N)
{
    int K;
    while (1) {
        printf("Введите новое значение K (3 ≤ K ≤ %d): ", current_N);
        if (scanf("%d", &K) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите целое число.\n");
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        if (K < 3 || K > current_N) {
            printf("Ошибка: K должно быть в диапазоне 3..%d.\n", current_N);
            continue;
        }
        return K;
    }
}

int show_end_menu(void)
{
    int choice;
    while (1) {
        printf("\n========================================\n");
        printf("1. Новая игра (PvP)\n");
        printf("2. Вернуться в главное меню\n");
        printf("3. Выход из программы\n");
        printf("========================================\n");
        printf("Введите 1, 2 или 3: ");
        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: введите 1, 2 или 3.\n");
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        if (choice >= 1 && choice <= 3) {
            return choice;
        }
        printf("Ошибка: введите 1, 2 или 3.\n");
    }
}

void exit_menu(void)
{
    clear_screen();
    printf("========================================\n");
    printf("===           Выход из игры         ===\n");
    printf("========================================\n");
    printf("До новых встреч!\n");
    printf("========================================\n\n");
    pause_for_enter();
}
