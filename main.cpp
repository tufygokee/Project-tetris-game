#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

const int TINGGI = 20;
const int LEBAR = 10;

int papan[TINGGI][LEBAR] = {0};
int skor = 0;

// Bentuk balok Tetris
int bentuk[7][4][4] = {
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

int balok[4][4];
int x, y;

// Membuat balok baru
void balokBaru() {
    int jenis = rand() % 7;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            balok[i][j] = bentuk[jenis][i][j];

    x = LEBAR / 2 - 2;
    y = 0;
}

// Mengecek tabrakan
bool tabrakan(int nx, int ny, int b[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b[i][j]) {
                int px = nx + j;
                int py = ny + i;

                if (px < 0 || px >= LEBAR ||
                    py >= TINGGI)
                    return true;

                if (py >= 0 && papan[py][px])
                    return true;
            }
        }
    }

    return false;
}
