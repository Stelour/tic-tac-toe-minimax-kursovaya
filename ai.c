#include <stdlib.h>

int check_win(int N, int K, char **field, char player);

#define MEDIUM_DEPTH 3

int evaluate(char **field, int N, int K) {
    if (check_win(N, K, field, 'O')) return +10;
    if (check_win(N, K, field, 'X')) return -10;
    return 0;
}

int minimax(char **field, int N, int K, int depth, int is_maximizer) {
    int score = evaluate(field, N, K);
    if (score == +10 || score == -10) {
        return score;
    }

    if (depth == 0) {
        return 0;
    }

    int moves_left = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                moves_left++;
            }
        }
    }
    if (moves_left == 0) {
        return 0;
    }

    if (is_maximizer) {
        int best_val = -1000;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == '_') {
                    field[i][j] = 'O';

                    int value = minimax(field, N, K, depth - 1, 0);

                    field[i][j] = '_';
                    if (value > best_val) {
                        best_val = value;
                    }
                }
            }
        }
        return best_val;
    } else {
        int best_val = +1000;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == '_') {
                    field[i][j] = 'X';

                    int value = minimax(field, N, K, depth - 1, 1);
                    
                    field[i][j] = '_';
                    if (value < best_val) {
                        best_val = value;
                    }
                }
            }
        }
        return best_val;
    }
}

void get_minimax_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_val = -1000;  
    int best_r = 0, best_c = 0;

    int moves_left = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                moves_left++;
            }
        }
    }
    if (moves_left == 0) {
        *out_row = 0;
        *out_col = 0;
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                field[i][j] = 'O';
                int move_val = minimax(field, N, K, MEDIUM_DEPTH - 1, 0);
                field[i][j] = '_';
                if (move_val > best_val) {
                    best_val = move_val;
                    best_r = i;
                    best_c = j;
                }
            }
        }
    }

    *out_row = best_r;
    *out_col = best_c;
}

void get_random_move(char **field, int N, int *out_row, int *out_col) {
    int free_count = 0;
    int (*free_cells)[2] = malloc(N * N * sizeof(int[2]));
    if (free_cells == NULL) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == '_') {
                    *out_row = i;
                    *out_col = j;
                    return;
                }
            }
        }

        *out_row = 0;
        *out_col = 0;
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                free_cells[free_count][0] = i;
                free_cells[free_count][1] = j;
                free_count++;
            }
        }
    }

    if (free_count == 0) {
        *out_row = 0;
        *out_col = 0;
        free(free_cells);
        return;
    }

    int idx = rand() % free_count;

    *out_row = free_cells[idx][0];
    *out_col = free_cells[idx][1];

    free(free_cells);
}
