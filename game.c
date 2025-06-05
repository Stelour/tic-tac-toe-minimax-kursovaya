#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//input.c
void clear_screen(void);
void pause_for_enter(void);
int show_end_menu(void);
int read_menu_choice(void);
void show_settings_menu(int current_N, int current_K, int current_level);
int read_new_N(void);
int read_new_K(int current_N);
int read_new_level(void);

//ai.c
void get_random_move(char **field, int N, int *out_row, int *out_col);
void get_minimax_move(char **field, int N, int K, int *out_row, int *out_col);
void get_alphabeta_move(char **field, int N, int K, int *out_row, int *out_col);

extern int current_N;
extern int current_K;
extern int current_level;

#define EMPTY_CELL '_'
#define CROSS 'X'
#define ZERO 'O'

void print_field(int N, char **field) {
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
}

int check_win(int N, int K, char **field, char player) {
    int dir[4][2] = {
        {0, 1}, //вправо
        {1, 0}, //вниз
        {1, 1}, //по диагонали вниз-вправо
        {1, -1} //по диагонали вниз-влево
    };

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] != player)
                continue;
            for (int d = 0; d < 4; d++) {
                int count = 1;
                int dx = dir[d][0];
                int dy = dir[d][1];
                int x = i + dx;
                int y = j + dy;
                while (x >= 0 && x < N && y >= 0 && y < N && field[x][y] == player) {
                    count++;
                    x += dx;
                    y += dy;
                }
                if (count >= K) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void init_field(int N, char **field) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            field[i][j] = EMPTY_CELL;
        }
    }
}

void free_field(int N, char **field) {
    for (int i = 0; i < N; i++) {
        free(field[i]);
    }
    free(field);
}

void start_pvp(void) {
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
            for (int k = 0; k < i; k++) free(field[k]);
            free(field);
            printf("Ошибка выделения памяти!\n");
            pause_for_enter();
            return;
        }
    }

    int input_row, input_col;
    char current_player = CROSS;
    int moves_count = 0;

    init_field(N, field);

    while (1) {
        clear_screen();
        printf("      PvP (N=%d, K=%d)\n\n", N, K);
        print_field(N, field);

        printf("\nХод игрока %c. Введите номер строки и столбца (или 0 0 для выхода): ", current_player);
        if (scanf("%d %d", &input_row, &input_col) != 2) {
            int tmp;
            while ((tmp = getchar()) != '\n' && tmp != EOF) { }
            printf("Некорректный ввод. Попробуйте снова.\n");
            pause_for_enter();
            continue;
        }
        int tmp;
        while ((tmp = getchar()) != '\n' && tmp != EOF) { }

        if (input_row == 0 && input_col == 0) {
            free_field(N, field);
            return;
        }

        int row = input_row - 1;
        int col = input_col - 1;
        if (row < 0 || row >= N || col < 0 || col >= N) {
            printf("Некорректные координаты. Попробуйте снова.\n");
            pause_for_enter();
            continue;
        }
        if (field[row][col] != EMPTY_CELL) {
            printf("Клетка занята! Выберите другую.\n");
            pause_for_enter();
            continue;
        }

        field[row][col] = current_player;
        moves_count++;

        if (check_win(N, K, field, current_player)) {
            clear_screen();
            printf("========================================\n");
            printf("=       PvP завершён: Победа %c        =\n", current_player);
            printf("========================================\n");
            printf("Финальное поле:\n\n");
            print_field(N, field);

            int choice = show_end_menu();
            if (choice == 1) {
                init_field(N, field);
                current_player = CROSS;
                moves_count = 0;
                continue;
            } else if (choice == 2) {
                free_field(N, field);
                return;
            } else {
                free_field(N, field);
                exit(0);
            }
        }

        if (moves_count == N * N) {
            clear_screen();
            printf("========================================\n");
            printf("=             Ничья (Draw)             =\n");
            printf("========================================\n");
            printf("Финальное поле:\n\n");
            print_field(N, field);

            int choice = show_end_menu();
            if (choice == 1) {
                init_field(N, field);
                current_player = CROSS;
                moves_count = 0;
                continue;
            } else if (choice == 2) {
                free_field(N, field);
                return;
            } else {
                free_field(N, field);
                exit(0);
            }
        }

        current_player = (current_player == CROSS ? ZERO : CROSS);
    }
}

void start_pvc(void) {
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
            for (int k = 0; k < i; k++) free(field[k]);
            free(field);
            printf("Ошибка выделения памяти!\n");
            pause_for_enter();
            return;
        }
    }

    int input_row, input_col;
    char current_player = CROSS;
    int moves_count = 0;

    init_field(N, field);
    srand((unsigned) time(NULL));

    while (1) {
        clear_screen();
        printf("      PvC (N=%d, K=%d, уровень ИИ=%d)\n\n", N, K, current_level);
        print_field(N, field);

        if (current_player == CROSS) {
            printf("\nВаш ход (X). Введите номер строки и столбца (или 0 0 для выхода): ");
            if (scanf("%d %d", &input_row, &input_col) != 2) {
                int tmp;
                while ((tmp = getchar()) != '\n' && tmp != EOF) { }
                printf("Некорректный ввод. Попробуйте снова.\n");
                pause_for_enter();
                continue;
            }
            int tmp;
            while ((tmp = getchar()) != '\n' && tmp != EOF) { }

            if (input_row == 0 && input_col == 0) {
                free_field(N, field);
                return;
            }

            int row = input_row - 1;
            int col = input_col - 1;
            if (row < 0 || row >= N || col < 0 || col >= N) {
                printf("Некорректные координаты. Попробуйте снова.\n");
                pause_for_enter();
                continue;
            }
            if (field[row][col] != EMPTY_CELL) {
                printf("Клетка занята! Выберите другую.\n");
                pause_for_enter();
                continue;
            }

            field[row][col] = CROSS;
            moves_count++;

            if (check_win(N, K, field, CROSS)) {
                clear_screen();
                printf("========================================\n");
                printf("=       PvC завершён: Победили X       =\n");
                printf("========================================\n");
                printf("Финальное поле:\n\n");
                print_field(N, field);

                int choice = show_end_menu();
                if (choice == 1) {
                    init_field(N, field);
                    current_player = CROSS;
                    moves_count = 0;
                    continue;
                } else if (choice == 2) {
                    free_field(N, field);
                    return;
                } else {
                    free_field(N, field);
                    exit(0);
                }
            }

            if (moves_count == N * N) {
                clear_screen();
                printf("========================================\n");
                printf("=             Ничья (Draw)             =\n");
                printf("========================================\n");
                printf("Финальное поле:\n\n");
                print_field(N, field);

                int choice = show_end_menu();
                if (choice == 1) {
                    init_field(N, field);
                    current_player = CROSS;
                    moves_count = 0;
                    continue;
                } else if (choice == 2) {
                    free_field(N, field);
                    return;
                } else {
                    free_field(N, field);
                    exit(0);
                }
            }

            current_player = ZERO;
        }
        else {
            printf("\nХод компьютера (O)...\n");
            pause_for_enter();

            int r, c;
            if (current_level == 1) {
                get_random_move(field, N, &r, &c);
                field[r][c] = ZERO;
                moves_count++;
            } else if (current_level == 2) {
                get_minimax_move(field, N, K, &r, &c);
                field[r][c] = ZERO;
                moves_count++;
            } else {
                get_alphabeta_move(field, N, K, &r, &c);
                field[r][c] = ZERO;
                moves_count++;
            }
            if (check_win(N, K, field, ZERO)) {
                clear_screen();
                printf("========================================\n");
                printf("=      PvC завершён: Победили O        =\n");
                printf("========================================\n");
                printf("Финальное поле:\n\n");
                print_field(N, field);

                int choice = show_end_menu();
                if (choice == 1) {
                    init_field(N, field);
                    current_player = CROSS;
                    moves_count = 0;
                    continue;
                } else if (choice == 2) {
                    free_field(N, field);
                    return;
                } else {
                    free_field(N, field);
                    exit(0);
                }
            }

            if (moves_count == N * N) {
                clear_screen();
                printf("========================================\n");
                printf("=             Ничья (Draw)             =\n");
                printf("========================================\n");
                printf("Финальное поле:\n\n");
                print_field(N, field);

                int choice = show_end_menu();
                if (choice == 1) {
                    init_field(N, field);
                    current_player = CROSS;
                    moves_count = 0;
                    continue;
                } else if (choice == 2) {
                    free_field(N, field);
                    return;
                } else {
                    free_field(N, field);
                    exit(0);
                }
            }

            current_player = CROSS;
        }
    }
}

void start_settings(void) {
    while (1) {
        clear_screen();
        show_settings_menu(current_N, current_K, current_level);
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
        } else if (choice == 3) {
            int newL = read_new_level();
            current_level = newL;
        } else if (choice == 0) {
            return;
        } else {
            clear_screen();
            printf("Некорректный ввод. Введите 0, 1, 2 или 3.\n");
            pause_for_enter();
        }
    }
}
