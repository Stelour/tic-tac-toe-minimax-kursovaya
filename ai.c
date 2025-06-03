#include <stdlib.h>

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