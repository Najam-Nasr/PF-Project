#include <stdio.h>
#include <stdlib.h>  
#include <windows.h> 

struct tetris_level {
    int score;
    int nsec;
};

struct tetris {
    char **game;
    int w;
    int h;
    int level;
    int gameover;
    int score;
    struct tetris_block {
        char data[5][5];
        int w;
        int h;
        char block_char; 
    } current;
    int x;
    int y;
} t;

struct tetris_block blocks[] =
{
    {{"##", 
      "##"},
    2, 2, '@'}, 

    {{" ##",
      "###"},
    3, 2, 'T'},  
    
    {{"####"}, 
     5, 1, '-'}, 
    
    {{"##",
      "# ",
      "# "},
    2, 3, 'L'}, 
    
    {{"##",
      " #",
      " #"},
    2, 3, 'J'}, 
    
    {{"## ",
      " ##"},
    3, 2, 'Z'}, 
    
    {{"###",
      " #"},
    3,2, 'C'}, 
    
    {{"###",
      "###",
      "###"},
    3, 3, 'B'}  
};

int TETRIS_PIECES = sizeof(blocks) / sizeof(struct tetris_block);

void tetris_init(int w, int h){
    int x, y;
    t.level = 1;
    t.score = 0;
    t.gameover = 0;
    t.w = w;
    t.h = h;
    t.game = malloc(sizeof(char *) * w);
    for (x = 0; x < w; x++) {
        t.game[x] = malloc(sizeof(char) * h);
        for (y = 0; y < h; y++) {
            t.game[x][y] = ' ';
        }
    }
}

void tetris_clean() {
    int x;
    for (x = 0; x < t.w; x++) {
        free(t.game[x]);
    }
    free(t.game);
}

void tetris_print() {
    int x, y;
    system("cls");  
    printf("[LEVEL: %d | SCORE: %d]\n", t.level, t.score);
    for (x = 0; x < 2 * t.w + 2; x++) {
        printf("~");
    }
    printf("\n");
    for (y = 0; y < t.h; y++) {
        printf("!");
        for (x = 0; x < t.w; x++) {
            if (x >= t.x && y >= t.y && x < (t.x + t.current.w) && y < (t.y + t.current.h) && t.current.data[y - t.y][x - t.x] != ' ') {
                printf("%c ", t.current.block_char); 
            } else {
                printf("%c ", t.game[x][y]);
            }
        }
        printf("!\n");
    }
    for (x = 0; x < 2 * t.w + 2; x++) {
        printf("~");
    }
    printf("\n");
}

int tetris_hittest() {
    int x, y, X, Y;
    struct tetris_block b = t.current;
    for (x = 0; x < b.w; x++) {
        for (y = 0; y < b.h; y++) {
            X = t.x + x;
            Y = t.y + y;
            if (X < 0 || X >= t.w) {
                return 1;
            }
            if (b.data[y][x] != ' ') {
                if ((Y >= t.h) || (X >= 0 && X < t.w && Y >= 0 && t.game[X][Y] != ' ')) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void tetris_new_block() {
    t.current = blocks[rand() % TETRIS_PIECES];
    t.x = (t.w / 2) - (t.current.w / 2);
    t.y = 0;
    if (tetris_hittest()) {
        t.gameover = 1;
    }
}

void tetris_rotate() {
    struct tetris_block b = t.current;
    struct tetris_block s = b;
    int x, y;
    b.w = s.h;
    b.h = s.w;
    for (x = 0; x < s.w; x++) {
        for (y = 0; y < s.h; y++) {
            b.data[x][y] = s.data[s.h - y - 1][x];
        }
    }
    int x_pos = t.x;
    int y_pos = t.y;
    t.x -= (b.w - s.w) / 2;
    t.y -= (b.h - s.h) / 2;
    t.current = b;
    if (tetris_hittest()) {
        t.current = s;
        t.x = x_pos;
        t.y = y_pos;
    }
}

void tetris_gravity() {
    t.y++;
    if (tetris_hittest()) {
        t.y--; 
        for (int x = 0; x < t.current.w; x++) {
            for (int y = 0; y < t.current.h; y++) {
                if (t.current.data[y][x] != ' ') {
                    t.game[t.x + x][t.y + y] = t.current.block_char;
                }
            }
        }
        tetris_new_block();
    }
}

void tetris_run(int w, int h) {
    tetris_init(w, h);
    tetris_new_block();

    while (!t.gameover) {
        tetris_print();

        if (GetAsyncKeyState('A') & 0x8000) {  
            t.x--;
            if (tetris_hittest()) {
                t.x++;
            }
        }
        if (GetAsyncKeyState('D') & 0x8000) { 
            t.x++;
            if (tetris_hittest()) {
                t.x--;
            }
        }
        if (GetAsyncKeyState('W') & 0x8000) {  
            tetris_rotate();
        }

        Sleep(300);  
        tetris_gravity();
    }

    tetris_print();
    printf("*** GAME OVER ***\n");

    tetris_clean();
}

int main(void) {
    tetris_run(12, 15);
    return EXIT_SUCCESS;
}
