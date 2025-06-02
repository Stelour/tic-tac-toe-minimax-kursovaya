#include <stdio.h>

// input.c
void clear_screen(void);
void pause_for_enter(void);

int check_win(int N, int K, char field[N][N], char player)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= N - K; j++) {
            int cnt = 0;
            for (int t = 0; t < K; t++) {
                if (field[i][j + t] == player) {
                    cnt++;
                } else {
                    break;
                }
            }
            if (cnt == K) {
                return 1;
            }
        }
    }

    for (int j = 0; j < N; j++) {
        for (int i = 0; i <= N - K; i++) {
            int cnt = 0;
            for (int t = 0; t < K; t++) {
                if (field[i + t][j] == player) {
                    cnt++;
                } else {
                    break;
                }
            }
            if (cnt == K) {
                return 1;
            }
        }
    }

    for (int i = 0; i <= N - K; i++) {
        for (int j = 0; j <= N - K; j++) {
            int cnt = 0;
            for (int t = 0; t < K; t++) {
                if (field[i + t][j + t] == player) {
                    cnt++;
                } else {
                    break;
                }
            }
            if (cnt == K) {
                return 1;
            }
        }
    }

    for (int i = 0; i <= N - K; i++) {
        for (int j = K - 1; j < N; j++) {
            int cnt = 0;
            for (int t = 0; t < K; t++) {
                if (field[i + t][j - t] == player) {
                    cnt++;
                } else {
                    break;
                }
            }
            if (cnt == K) {
                return 1;
            }
        }
    }

    return 0;
}

void start_pvp(void)
{
    const int N = 3;
    const int K = 3;
    char field[N][N];
    int row, col;
    char current_player = 'X';
    int moves = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            field[i][j] = '_';
        }
    }

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

        if (moves == N * N) {
            printf("\nНичья! Поле заполнено.\n");
            pause_for_enter();
            return;
        }

        printf("\nВведите номер строки и номер столбца (оба от 1 до %d): ", N);
        if (scanf("%d %d", &row, &col) != 2) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
            printf("Ошибка: нужно ввести два целых числа.\n");
            pause_for_enter();
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }

        row--;
        col--;

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
        moves++;

        if (check_win(N, K, field, current_player)) {
            clear_screen();
            printf("========================================\n");
            printf("     Режим PvP завершён: Победа %c     \n", current_player);
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
            pause_for_enter();
            return;
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
    clear_screen();
    printf("========================================\n");
    printf("===      Заготовка: Настройки       ===\n");
    printf("========================================\n");
    printf("Здесь будут настройки (выбор N, K, уровень ИИ).\n");
    printf("========================================\n\n");
    pause_for_enter();
}
