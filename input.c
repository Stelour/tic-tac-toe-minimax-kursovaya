#include <stdio.h>

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

void clear_screen(void)
{
    printf("\033[2J\033[H");
}

void pause_for_enter(void)
{
    printf("Нажмите Enter, чтобы продолжить...");
    while (getchar() != '\n') { }
}

void exit_menu(void){
    clear_screen();
    printf("========================================\n");
    printf("===           Выход из игры       ===\n");
    printf("========================================\n");
    printf("До новых встреч!\n");
    printf("========================================\n\n");
    pause_for_enter();
}