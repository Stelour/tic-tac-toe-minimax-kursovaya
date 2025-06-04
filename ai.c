#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

extern int check_win(int N, int K, char **field, char player);

#define EMPTY_CELL '_'
#define CROSS 'X'
#define ZERO 'O'

#define MINIMAX_DEPTH 4

void get_random_move(char **field, int N, int *out_row, int *out_col) {
    int free_count = 0;
    int (*free_cells)[2] = malloc(N * N * sizeof(int[2]));
    if (free_cells == NULL) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == EMPTY_CELL) {
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
            if (field[i][j] == EMPTY_CELL) {
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

int max_int(int a, int b) { return a > b ? a : b; }
int min_int(int a, int b) { return a < b ? a : b; }

int pow_int(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

int count_empty_cells(char **field, int N) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == EMPTY_CELL) {
                count++;
            }
        }
    }
    return count;
}

bool has_moves_left(char **field, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == EMPTY_CELL) return true;
        }
    }
    return false;
}

int calculate_potential_lines(char **field, int N, int K, char player_char) {
    int potential_score = 0;
    char opponent_char = (player_char == ZERO ? CROSS : ZERO);

    int dr[4] = { 0, 1, 1, 1 };
    int dc[4] = { 1, 0, 1, -1 };

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            for (int d = 0; d < 4; ++d) {
                int player_count = 0;
                int empty_count  = 0;
                bool possible_line = true;

                for (int t = 0; t < K; ++t) {
                    int nr = r + t * dr[d];
                    int nc = c + t * dc[d];
                    if (nr < 0 || nr >= N || nc < 0 || nc >= N) {
                        possible_line = false;
                        break;
                    }
                    char ch = field[nr][nc];
                    if (ch == player_char) {
                        player_count++;
                    } else if (ch == EMPTY_CELL) {
                        empty_count++;
                    } else {
                        possible_line = false;
                        break;
                    }
                }

                if (!possible_line) continue;

                if (player_count == K - 1 && empty_count == 1) {
                    return (player_char == ZERO ? 1000000 : -1000000);
                }

                if (player_count > 0) {
                    potential_score += player_count * player_count;
                }
            }
        }
    }
    return potential_score;
}

int minimax_recursive(char **field, int N, int K, int depth, int is_maximizer) {
    if (check_win(N, K, field, ZERO)) {
        return 1000000 + depth;
    }
    if (check_win(N, K, field, CROSS)) {
        return -1000000 - depth;
    }
    int num_empty = count_empty_cells(field, N);
    if (depth == 0 || num_empty == 0) {
        int score_O = calculate_potential_lines(field, N, K, ZERO);
        int score_X = calculate_potential_lines(field, N, K, CROSS);
        return score_O - score_X;
    }

    if (is_maximizer) {
        int best_val = INT_MIN;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == EMPTY_CELL) {
                    field[r][c] = ZERO;
                    int val = minimax_recursive(field, N, K, depth - 1, 0);
                    field[r][c] = EMPTY_CELL;
                    best_val = max_int(best_val, val);
                }
            }
        }
        return best_val;
    } else {
        int best_val = INT_MAX;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == EMPTY_CELL) {
                    field[r][c] = CROSS;
                    int val = minimax_recursive(field, N, K, depth - 1, 1);
                    field[r][c] = EMPTY_CELL;
                    best_val = min_int(best_val, val);
                }
            }
        }
        return best_val;
    }
}

void get_minimax_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_score = INT_MIN;
    *out_row = -1;
    *out_col = -1;
    int first_r = -1, first_c = -1;

    int empties = count_empty_cells(field, N);
    int depth_limit = empties < MINIMAX_DEPTH ? empties : MINIMAX_DEPTH;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == EMPTY_CELL) {
                if (first_r == -1) {
                    first_r = r;
                    first_c = c;
                }
                field[r][c] = ZERO;
                int score = minimax_recursive(field, N, K, depth_limit - 1, 0);
                field[r][c] = EMPTY_CELL;
                if (score > best_score) {
                    best_score = score;
                    *out_row = r;
                    *out_col = c;
                }
            }
        }
    }

    if (*out_row == -1) {
        if (first_r != -1) {
            *out_row = first_r;
            *out_col = first_c;
        } else if (empties > 0) {
            get_random_move(field, N, out_row, out_col);
        }
    }
}

int alphabeta_recursive(char **field, int N, int K, int depth, int alpha, int beta, int is_maximizer) {
    if (check_win(N, K, field, ZERO)) {
        return 1000000 + depth;
    }
    if (check_win(N, K, field, CROSS)) {
        return -1000000 - depth;
    }
    int num_empty = count_empty_cells(field, N);
    if (depth == 0 || num_empty == 0) {
        int score_O = calculate_potential_lines(field, N, K, ZERO);
        int score_X = calculate_potential_lines(field, N, K, CROSS);
        return score_O - score_X;
    }

    if (is_maximizer) {
        int value = INT_MIN;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == EMPTY_CELL) {
                    field[r][c] = ZERO;
                    int val = alphabeta_recursive(field, N, K, depth - 1, alpha, beta, 0);
                    field[r][c] = EMPTY_CELL;
                    value = max_int(value, val);
                    alpha = max_int(alpha, value);
                    if (alpha >= beta) {
                        return value;
                    }
                }
            }
        }
        return value;
    } else {
        int value = INT_MAX;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == EMPTY_CELL) {
                    field[r][c] = CROSS;
                    int val = alphabeta_recursive(field, N, K, depth - 1, alpha, beta, 1);
                    field[r][c] = EMPTY_CELL;
                    value = min_int(value, val);
                    beta = min_int(beta, value);
                    if (alpha >= beta) {
                        return value;
                    }
                }
            }
        }
        return value;
    }
}

int compute_dynamic_depth(char **field, int N) {
    int empties = count_empty_cells(field, N);
    int total = N * N;
    if (empties > total / 2) {
        return 2;
    } else if (empties > total / 4) {
        return 3;
    } else {
        return 4;
    }
}

void get_alphabeta_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_score = INT_MIN;
    *out_row = -1;
    *out_col = -1;
    int first_r = -1, first_c = -1;

    int empties = count_empty_cells(field, N);
    int depth_limit = empties < compute_dynamic_depth(field, N) ? empties : compute_dynamic_depth(field, N);

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == EMPTY_CELL) {
                if (first_r == -1) {
                    first_r = r;
                    first_c = c;
                }
                field[r][c] = ZERO;
                int score = alphabeta_recursive(field, N, K, depth_limit - 1, INT_MIN, INT_MAX, 0);
                field[r][c] = EMPTY_CELL;
                if (score > best_score) {
                    best_score = score;
                    *out_row = r;
                    *out_col = c;
                }
            }
        }
    }

    if (*out_row == -1) {
        if (first_r != -1) {
            *out_row = first_r;
            *out_col = first_c;
        } else if (empties > 0) {
            get_random_move(field, N, out_row, out_col);
        }
    }
}
