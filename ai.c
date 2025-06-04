#include <stdlib.h>
#include <limits.h>
#include <time.h>

// game.c
int check_win(int N, int K, char **field, char player);

void get_random_move(char **field, int N, int *out_row, int *out_col) {
    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    int free_count = 0;

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
    } else {
        int idx = rand() % free_count;
        *out_row = free_cells[idx][0];
        *out_col = free_cells[idx][1];
    }

    free(free_cells);
}

int evaluate(char **field, int N, int K) {
    if (check_win(N, K, field, 'O'))  return +1000;
    if (check_win(N, K, field, 'X'))  return -1000;
    return 0;
}

#define MEDIUM_DEPTH 3

int minimax(char **field, int N, int K, int depth, int is_maximizer) {
    int score = evaluate(field, N, K);

    if (score == +1000 || score == -1000 || depth == 0) {
        return score;
    }

    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    int free_count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                free_cells[free_count][0] = i;
                free_cells[free_count][1] = j;
                free_count++;
            }
        }
    }

    int best;
    if (is_maximizer) {
        best = INT_MIN;
        for (int idx = 0; idx < free_count; idx++) {
            int r = free_cells[idx][0];
            int c = free_cells[idx][1];
            field[r][c] = 'O';
            int val = minimax(field, N, K, depth - 1, 0);
            field[r][c] = '_';
            if (val > best) {
                best = val;
            }
        }
    } else {
        best = INT_MAX;
        for (int idx = 0; idx < free_count; idx++) {
            int r = free_cells[idx][0];
            int c = free_cells[idx][1];
            field[r][c] = 'X';
            int val = minimax(field, N, K, depth - 1, 1);
            field[r][c] = '_';
            if (val < best) {
                best = val;
            }
        }
    }

    free(free_cells);
    return best;
}

void get_minimax_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_val = INT_MIN;
    int best_r = 0, best_c = 0;

    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    int free_count = 0;

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

    for (int idx = 0; idx < free_count; idx++) {
        int r = free_cells[idx][0];
        int c = free_cells[idx][1];

        field[r][c] = 'O';
        int move_val = minimax(field, N, K, MEDIUM_DEPTH - 1, 0);
        field[r][c] = '_';

        if (move_val > best_val) {
            best_val = move_val;
            best_r = r;
            best_c = c;
        }
    }

    *out_row = best_r;
    *out_col = best_c;
    free(free_cells);
}

int alphabeta(char **field, int N, int K, int depth, int alpha, int beta, int is_maximizer) {
    int score = evaluate(field, N, K);

    if (score == +1000 || score == -1000 || depth == 0) {
        return score;
    }

    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    int free_count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                free_cells[free_count][0] = i;
                free_cells[free_count][1] = j;
                free_count++;
            }
        }
    }

    int best;
    if (is_maximizer) {
        best = INT_MIN;
        for (int idx = 0; idx < free_count; idx++) {
            int r = free_cells[idx][0];
            int c = free_cells[idx][1];

            field[r][c] = 'O';
            int val = alphabeta(field, N, K, depth - 1, alpha, beta, 0);
            field[r][c] = '_';

            if (val > best) {
                best = val;
            }
            if (best > alpha) {
                alpha = best;
            }
            if (alpha >= beta) {
                break;
            }
        }
    } else {
        best = INT_MAX;
        for (int idx = 0; idx < free_count; idx++) {
            int r = free_cells[idx][0];
            int c = free_cells[idx][1];

            field[r][c] = 'X';
            int val = alphabeta(field, N, K, depth - 1, alpha, beta, 1);
            field[r][c] = '_';

            if (val < best) {
                best = val;
            }
            if (best < beta) {
                beta = best;
            }
            if (alpha >= beta) {
                break;
            }
        }
    }

    free(free_cells);
    return best;
}

void get_alphabeta_move(char **field, int N, int K, int *out_row, int *out_col) {
    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    int free_count = 0;

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

    for (int k = 0; k < free_count; k++) {
        int r = free_cells[k][0];
        int c = free_cells[k][1];
        field[r][c] = 'O';
        if (check_win(N, K, field, 'O')) {
            *out_row = r;
            *out_col = c;
            field[r][c] = '_';
            free(free_cells);
            return;
        }
        field[r][c] = '_';
    }

    int search_depth = (free_count <= 10 ? free_count : 5);
    int best_val = INT_MIN;
    int best_r = free_cells[0][0];
    int best_c = free_cells[0][1];

    for (int k = 0; k < free_count; k++) {
        int r = free_cells[k][0];
        int c = free_cells[k][1];
        field[r][c] = 'O';

        int move_val = alphabeta(field, N, K, search_depth - 1,
                                 INT_MIN, INT_MAX, 0);

        field[r][c] = '_';

        if (move_val > best_val) {
            best_val = move_val;
            best_r = r;
            best_c = c;
        }
    }

    *out_row = best_r;
    *out_col = best_c;
    free(free_cells);
}

void get_ai_move(int level, char **field, int N, int K, int *out_row, int *out_col) {
    if (level == 1) {
        get_random_move(field, N, out_row, out_col);
    } else if (level == 2) {
        get_minimax_move(field, N, K, out_row, out_col);
    } else {
        get_alphabeta_move(field, N, K, out_row, out_col);
    }
}
