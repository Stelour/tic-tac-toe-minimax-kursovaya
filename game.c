#include <stdio.h>
#include <stdlib.h>

// input.c
void clear_screen(void);
void pause_for_enter(void);
int show_end_menu(void);
int read_menu_choice(void);
void show_settings_menu(int current_N, int current_K);
int read_new_N(void);
int read_new_K(int current_N);

extern int current_N;
extern int current_K;

int check_win(int N, int K, char **field, char player)
{
    int dir[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != player) continue;
            for (int d = 0; d < 4; d++) {
                int di = dir[d][0];
                int dj = dir[d][1];
                int count = 1;
                int x = i + di;
                int y = j + dj;
                while (x >= 0 && x < N && y >= 0 && y < N && field[x][y] == player) {
                    count++;
                    if (count == K) return 1;
                    x += di;
                    y += dj;
                }
            }
        }
    }
    return 0;
}

int check_draw(int N, char **field)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                return 0;
            }
        }
    }
    return 1;
}

void free_field(int N, char **field)
{
    if (field == NULL) return;
    for (int i = 0; i < N; i++) {
        free(field[i]);
    }
    free(field);
}

void init_field(int N, char **field)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            field[i][j] = '_';
        }
    }
}

void start_pvp(void)
{
    int N = current_N;
    int K = current_K;
    char **field = malloc(N * sizeof(char *));
    if (field == NULL) {
        printf("Ошибка выделения памяти!\n");
        pause_for_enter();
        return;
    }
    for (int i = 0; i < N; i++) {
        field[i] = malloc(N * sizeof(char));
        if (field[i] == NULL) {
            for (int k = 0; k < i; k++) {
                free(field[k]);
            }
            free(field);
            printf("Ошибка выделения памяти!\n");
            pause_for_enter();
            return;
        }
    }
    int input_row, input_col;
    char current_player = 'X';
    init_field(N, field);
    while (1) {
        clear_screen();
        printf("========================================\n");
        printf("=       Режим PvP (Человек—Человек)   =\n");
        printf("========================================\n");
        printf("Размер поля: %dx%d, K = %d\n", N, N, K);
        printf("Текущий игрок: %c\n", current_player);
        printf("========================================\n");
        printf("   ");
        for (int j = 0; j < N; j++) {
            printf("%2d ", j + 1);
        }
        printf("\n");
        for (int i = 0; i < N; i++) {
            printf("%2d ", i + 1);
            for (int j = 0; j < N; j++) {
                printf(" %c ", field[i][j]);
            }
            printf("\n");
        }
        printf("\nВведите строку и столбец (1..%d), или 0 0 для выхода: ", N);
        if (scanf("%d %d", &input_row, &input_col) != 2) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: нужно ввести два целых числа.\n");
            pause_for_enter();
            continue;
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        if (input_row == 0 && input_col == 0) {
            free_field(N, field);
            return;
        }
        int row = input_row - 1;
        int col = input_col - 1;
        if (row < 0 || row >= N || col < 0 || col >= N) {
            clear_screen();
            printf("Ошибка: номера строки и столбца должны быть от 1 до %d.\n", N);
            pause_for_enter();
            continue;
        }
        if (field[row][col] != '_') {
            clear_screen();
            printf("Ошибка: клетка [%d,%d] уже занята.\n", row + 1, col + 1);
            pause_for_enter();
            continue;
        }
        field[row][col] = current_player;
        if (check_win(N, K, field, current_player)) {
            clear_screen();
            printf("========================================\n");
            printf("=       PvP завершён: Победа %c        =\n", current_player);
            printf("========================================\n");
            printf("Финальное поле:\n\n");
            printf("   ");
            for (int j = 0; j < N; j++) {
                printf("%2d ", j + 1);
            }
            printf("\n");
            for (int i = 0; i < N; i++) {
                printf("%2d ", i + 1);
                for (int j = 0; j < N; j++) {
                    printf(" %c ", field[i][j]);
                }
                printf("\n");
            }
            int choice = show_end_menu();
            if (choice == 1) {
                init_field(N, field);
                current_player = 'X';
                continue;
            } else if (choice == 2) {
                free_field(N, field);
                return;
            } else {
                free_field(N, field);
                exit(0);
            }
        }
        if (check_draw(N, field)) {
            clear_screen();
            printf("========================================\n");
            printf("=            PvP: Ничья!             =\n");
            printf("========================================\n");
            printf("Финальное поле:\n\n");
            printf("   ");
            for (int j = 0; j < N; j++) {
                printf("%2d ", j + 1);
            }
            printf("\n");
            for (int i = 0; i < N; i++) {
                printf("%2d ", i + 1);
                for (int j = 0; j < N; j++) {
                    printf(" %c ", field[i][j]);
                }
                printf("\n");
            }
            int choice = show_end_menu();
            if (choice == 1) {
                init_field(N, field);
                current_player = 'X';
                continue;
            } else if (choice == 2) {
                free_field(N, field);
                return;
            } else {
                free_field(N, field);
                exit(0);
            }
        }
        current_player = (current_player == 'X' ? 'O' : 'X');
    }
}

void start_pvc(void)
{
    clear_screen();
    printf("========================================\n");
    printf("===      Заготовка: игра PvC         ===\n");
    printf("========================================\n");
    printf("Здесь будет реализация игры человек—компьютер (PvC).\n");
    printf("========================================\n\n");
    pause_for_enter();
}

void start_settings(void)
{
    while (1) {
        clear_screen();
        extern int current_N, current_K;
        show_settings_menu(current_N, current_K);
        int choice = read_menu_choice();
        if (choice == 1) {
            int newN = read_new_N();
            current_N = newN;
            if (current_K > current_N) {
                current_K = current_N;
            }
        } else if (choice == 2) {
            int newK = read_new_K(current_N);
            current_K = newK;
        } else if (choice == 0) {
            return;
        } else {
            clear_screen();
            printf("\nНекорректный ввод. Пожалуйста, введите 0, 1 или 2.\n\n");
            pause_for_enter();
        }
    }
}
