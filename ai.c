#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

//game.c
int check_win(int N, int K, char **field, char player);

#define MINIMAX_DEPTH 4

static int count_empty_cells(char **field, int N) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == '_') {
                count++;
            }
        }
    }
    return count;
}

void get_random_move(char **field, int N, int *out_row, int *out_col) {
    int max_cells = N * N;
    int (*free_cells)[2] = malloc(max_cells * sizeof(int[2]));
    if (free_cells == NULL) {
        *out_row = -1; *out_col = -1; return;
    }
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
        *out_row = -1; *out_col = -1;
    } else {
        int idx = rand() % free_count;
        *out_row = free_cells[idx][0];
        *out_col = free_cells[idx][1];
    }
    free(free_cells);
}

static int calculate_potential_lines(char **field, int N, int K, char player_char) {
    int potential_score = 0;
    int opponent_char = (player_char == 'O' ? 'X' : 'O');

    int dr[] = {0, 1, 1, 1}; 
    int dc[] = {1, 0, 1, -1};

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            for (int i = 0; i < 4; ++i) {
                int player_count = 0;
                int empty_count = 0;
                bool possible_line = true;

                for (int k = 0; k < K; ++k) {
                    int cur_r = r + k * dr[i];
                    int cur_c = c + k * dc[i];

                    if (cur_r >= 0 && cur_r < N && cur_c >= 0 && cur_c < N) {
                        if (field[cur_r][cur_c] == player_char) {
                            player_count++;
                        } else if (field[cur_r][cur_c] == '_') {
                            empty_count++;
                        } else if (field[cur_r][cur_c] == opponent_char) {
                            possible_line = false;
                            break;
                        }
                    } else {
                        possible_line = false;
                        break;
                    }
                }

                if (possible_line && player_count > 0 && (player_count + empty_count == K)) {
                    if (player_count == K - 1) potential_score += 50;
                    else if (player_count == K - 2 && K > 2) potential_score += 10;
                    else if (player_count > 0) potential_score += 1;
                }
            }
        }
    }
    return potential_score;
}


// --- Оценочная функция ---
int evaluate(char **field, int N, int K, int depth_remaining) {
    if (check_win(N, K, field, 'O'))  return +1000 + depth_remaining;
    if (check_win(N, K, field, 'X'))  return -1000 - depth_remaining;
    
    if (depth_remaining == 0) {
        int ai_potential = calculate_potential_lines(field, N, K, 'O');
        int human_potential = calculate_potential_lines(field, N, K, 'X');
        return ai_potential - human_potential;
    }
    
    if (count_empty_cells(field, N) == 0) return 0;

    return 0;
}

static int minimax_recursive(char **field, int N, int K, int depth, int is_maximizer) {
    int current_score = evaluate(field, N, K, depth);

    if ((check_win(N, K, field, 'O') || check_win(N, K, field, 'X')) || depth == 0) {
        return current_score; 
    }
    if (count_empty_cells(field, N) == 0) return 0;

    int best_val;
    if (is_maximizer) { 
        best_val = INT_MIN;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == '_') {
                    field[r][c] = 'O';
                    int val = minimax_recursive(field, N, K, depth - 1, 0); 
                    field[r][c] = '_';
                    if (val > best_val) {
                        best_val = val;
                    }
                }
            }
        }
    } else { 
        best_val = INT_MAX;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == '_') {
                    field[r][c] = 'X';
                    int val = minimax_recursive(field, N, K, depth - 1, 1); 
                    field[r][c] = '_';
                    if (val < best_val) {
                        best_val = val;
                    }
                }
            }
        }
    }
    return best_val;
}

void get_minimax_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_score = INT_MIN;
    *out_row = -1;
    *out_col = -1;
    int first_available_r = -1, first_available_c = -1;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                if (first_available_r == -1) { 
                    first_available_r = r;
                    first_available_c = c;
                }
                field[r][c] = 'O';
                if (check_win(N, K, field, 'O')) {
                    *out_row = r; *out_col = c; field[r][c] = '_'; return; 
                }
                field[r][c] = '_';
            }
        }
    }

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                field[r][c] = 'X'; 
                if (check_win(N, K, field, 'X')) {
                    *out_row = r; *out_col = c; field[r][c] = '_'; return; 
                }
                field[r][c] = '_';
            }
        }
    }
    
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                field[r][c] = 'O';
                int move_score = minimax_recursive(field, N, K, MINIMAX_DEPTH - 1, 0);
                field[r][c] = '_';

                if (move_score > best_score) {
                    best_score = move_score;
                    *out_row = r; *out_col = c;
                }
            }
        }
    }

    if (*out_row == -1) {
        if (first_available_r != -1) {
            *out_row = first_available_r; *out_col = first_available_c;
        } else {
            if (count_empty_cells(field, N) > 0) get_random_move(field, N, out_row, out_col);
        }
    }
}

static int alphabeta_recursive(char **field, int N, int K, int depth, int alpha, int beta, int is_maximizer) {
    int current_score = evaluate(field, N, K, depth);

    if ((check_win(N, K, field, 'O') || check_win(N, K, field, 'X')) || depth == 0) {
        return current_score;
    }
    if (count_empty_cells(field, N) == 0) return 0;

    if (is_maximizer) { 
        int best_val = INT_MIN;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == '_') {
                    field[r][c] = 'O';
                    int val = alphabeta_recursive(field, N, K, depth - 1, alpha, beta, 0);
                    field[r][c] = '_';
                    if (val > best_val) best_val = val;
                    if (best_val > alpha) alpha = best_val;
                    if (alpha >= beta) return best_val; 
                }
            }
        }
        return best_val;
    } else { 
        int best_val = INT_MAX;
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (field[r][c] == '_') {
                    field[r][c] = 'X';
                    int val = alphabeta_recursive(field, N, K, depth - 1, alpha, beta, 1);
                    field[r][c] = '_';
                    if (val < best_val) best_val = val;
                    if (best_val < beta) beta = best_val;
                    if (alpha >= beta) return best_val;
                }
            }
        }
        return best_val;
    }
}

void get_alphabeta_move(char **field, int N, int K, int *out_row, int *out_col) {
    int best_score = INT_MIN;
    *out_row = -1; *out_col = -1;
    int first_available_r = -1, first_available_c = -1;

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                 if (first_available_r == -1) { 
                    first_available_r = r; first_available_c = c;
                }
                field[r][c] = 'O';
                if (check_win(N, K, field, 'O')) {
                    *out_row = r; *out_col = c; field[r][c] = '_'; return;
                }
                field[r][c] = '_';
            }
        }
    }

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                field[r][c] = 'X'; 
                if (check_win(N, K, field, 'X')) {
                    *out_row = r; *out_col = c; field[r][c] = '_'; return; 
                }
                field[r][c] = '_';
            }
        }
    }

    int num_empty = count_empty_cells(field, N);
    int search_depth;

    if (N <= 3 && K <=3) search_depth = (num_empty <= 7 ? num_empty : 7); 
    else if (N <= 4 && K <=4) search_depth = (num_empty <= 6 ? num_empty : 6); 
    else if (N <= 5) search_depth = (num_empty <= 5 ? num_empty : 5);
    else if (N <= 7) search_depth = (num_empty <= 4 ? num_empty : 5);
    else search_depth = (num_empty <= 3 ? num_empty : 3); 

    if (search_depth > num_empty) search_depth = num_empty;
    if (search_depth == 0 && num_empty > 0) search_depth = 1;
    
    if (num_empty == 0) {
         if (*out_row == -1 && first_available_r != -1) {
            *out_row = first_available_r; *out_col = first_available_c;
         }
        return;
    }

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            if (field[r][c] == '_') {
                field[r][c] = 'O';
                int move_score = alphabeta_recursive(field, N, K, search_depth - 1, INT_MIN, INT_MAX, 0);
                field[r][c] = '_';

                if (move_score > best_score) {
                    best_score = move_score;
                    *out_row = r; *out_col = c;
                }
            }
        }
    }
    
    if (*out_row == -1) {
         if (first_available_r != -1) {
            *out_row = first_available_r; *out_col = first_available_c;
        } else {
            if(num_empty > 0) get_random_move(field, N, out_row, out_col);
        }
    }
}