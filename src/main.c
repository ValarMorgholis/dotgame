#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define NORMAL_MODE     4
#define PRO_MODE        6
#define NPLAYERS        2
#define I2C(I)           (I + 'A' - 1)

int matr[PRO_MODE+1][PRO_MODE][3] = {0};
enum {HOR, VER, WIN};

void print_matrix(int mode) {
    for (int i = 0; i <= mode; i++) {
        for (int j = 0; j <= mode; j++)
            printf("*%s", matr[i][j][HOR] ? "--" : "  ");
        putchar('\n');

        for (int j = 0; j <= mode; j++)
            printf("%c%c ", matr[i][j][VER] ? '|' : ' ',
                            matr[i][j][WIN] ? I2C(matr[i][j][WIN]) : ' ');
        putchar('\n');
    }
}

int is_square(int row, int col)
{
    return matr[row][col][HOR] && matr[row][col][VER]   /* current point */
            && matr[row][col+1][VER]    /* right point */
            && matr[row+1][col][HOR];   /* down point */
}

int is_valid_input(int mode, int direc, int row, int col)
{
    if (direc != HOR && direc != VER)
        return 0;
    /* row in vertical lines is in 1..3 range */
    if (direc && row >= mode)
        return 0;
    /* col in horizontal lines is in 1..3 range */
    if (!direc && col >= mode)
        return 0;
    if (row < 0 || row > mode)
        return 0;
    if (col < 0 || col > mode)
        return 0;
    /* non-zero lines are in use */
    if (matr[row][col][direc])
        return 0;
    return 1;
}

int seeded_random(int max)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec + rand());
    return rand() % max;
}

int main(int argc, char const *argv[])
{
    int direc, row, col, wins, retry = 0, player = 1, faker = 0,
        sumscore = 0, mode = NORMAL_MODE - 1, scores[NPLAYERS] = {0};
    char *line;
    size_t sline;

    if (argc >= 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE - 1;

    if (argc >= 3 && !strncmp("fake", argv[2], 4))
        faker = 1;

    for (;;) {
        if (!retry && player != faker)
            print_matrix(mode);

        if (sumscore == mode * mode)
            break;

        if (player == faker) {
            direc = seeded_random(2);
            row = seeded_random(mode + 2);
            col = seeded_random(mode + 2);
        } else {
            line = NULL;
            printf("Player %c turn.Enter coordinates:\n", I2C(player));
            getline(&line, &sline, stdin);
            sscanf(line, "%d%d%d", &direc, &row, &col);
            if (feof(stdin))
                return (0);
        }

        row--, col--;
        if (!is_valid_input(mode, direc, row, col)) {
            if (player != faker)
                fprintf(stderr, "Invalid input. retry\n");
            retry = 1;
            continue;
        }
        retry = 0;
        matr[row][col][direc] = player;

        wins = 0;
        if (is_square(row, col)) {
            matr[row][col][WIN] = player;
            wins++;
        }
        if (direc) {
            if (col-- >= 0 && !matr[row][col][WIN]
                && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        } else {
            if (row-- >= 0 && !matr[row][col][WIN]
                && is_square(row, col)) {
                matr[row][col][WIN] = player;
                wins++;
            }
        }
        scores[player-1] += wins;
        sumscore += wins;

        if (!wins && ++player > NPLAYERS)
            player = 1;
    }

    for (int i = 0; i < NPLAYERS; i++)
        printf("score(%c): %d; ", 'A' + i, scores[i]);
    putchar('\n');
    return (0);
}
