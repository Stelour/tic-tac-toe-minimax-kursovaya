#include <stdio.h>
#include <stdlib.h>

int current_N = 3;
int current_K = 3;
int current_level = 1;

//input.c
void show_main_menu(void);
int read_menu_choice(void);
void clear_screen(void);
void pause_for_enter(void);
void exit_menu(void);

//game.c
void start_pvp(void);
void start_pvc(void);
void start_settings(void);

int main(void)
{
    while (1) {
        clear_screen();
        show_main_menu();
        int choice = read_menu_choice();
        switch (choice) {
            case 1:
                start_pvp();
                break;
            case 2:
                start_pvc();
                break;
            case 3:
                start_settings();
                break;
            case 0:
                exit_menu();
                return 0;
            default:
                clear_screen();
                printf("\nНекорректный ввод. Пожалуйста, введите 0, 1, 2 или 3.\n\n");
                pause_for_enter();
                break;
        }
    }
    return 0;
}
