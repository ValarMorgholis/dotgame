#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NORMAL_MODE 4
#define PRO_MODE 6
#define NPLAYERS 2

int main(int argc, char const *argv[])
{
    /* declaration */
    int player = 0, mode = NORMAL_MODE;
    int matr[PRO_MODE][PRO_MODE];

    /* parse arguments */
    if (argc == 2 && !strncmp("pro", argv[1], 4))
        mode = PRO_MODE;

    /* initialization */
    memset(matr, -1, sizeof(matr));

    /* start game */
    for (;;)
    {
        /* print matrix */
        for (int i = 0; i < mode; i++)
        {
            /* print dots */
            for (int j = 0; j < mode; j++)
                printf("%d%s", matr[i][j],
                        matr[i][j+1] != -1 ? "--" : "  ");

            /* print horizental lines */
            putchar('\n');
            for (int i = 0; i < mode; i++)
            {
                /* "   " or "|  " */
            }
        }

        /* get data */
    retry:
        int direction, row, col, *point;
        char *line = NULL;
        size_t sline = 0;

        printf("Player %c turn.Enter coordinates:\n", 'A' + player);
        getline(&line, &sline, stdin);
        if (feof(stdin))
        {
            fprintf(stderr, "end game: stdin was closed\n");
            _exit(EXIT_FAILURE);
        }
        sscanf(line, "%d%d%d", &direction, &row, &col);

        /* input validation */
        row--, col--;
        point = direction ? &matr[row+1][col] : &matr[row][col+1];
        if (!(direction == 0 || direction == 1)
            || (row < 0 || row > mode)
            || (col < 0 || col > mode)
            || (*point != -1))
        {
            fprintf(stderr, "\033[31mInvalid input. retry\033[0m\n");
            goto retry;
        }

        /* store player lines */
        *point = player;

        /* show score */

        /* check for continue */

        /* next player */
        player = (player + 1) % NPLAYERS;
    }

    /* result */

    return (EXIT_SUCCESS);
}
