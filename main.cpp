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

// Memutar balok
void putarBalok() {
    int hasil[4][4] = {0};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            hasil[j][3-i] = balok[i][j];

    if (!tabrakan(x, y, hasil)) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                balok[i][j] = hasil[i][j];
    }
}

// Menyimpan balok ke papan
void kunciBalok() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (balok[i][j]) {
                if (y + i >= 0 && y + i < TINGGI)
                    papan[y+i][x+j] = 1;
            }
        }
    }
}

// Menghapus baris penuh
void hapusBaris() {
    for (int i = TINGGI - 1; i >= 0; i--) {
        bool penuh = true;

        for (int j = 0; j < LEBAR; j++) {
            if (!papan[i][j]) {
                penuh = false;
                break;
            }
        }

        if (penuh) {
            for (int k = i; k > 0; k--)
                for (int j = 0; j < LEBAR; j++)
                    papan[k][j] = papan[k-1][j];

            for (int j = 0; j < LEBAR; j++)
                papan[0][j] = 0;

            skor += 100;
            i++;
        }
    }
}

// Menggambar papan
void gambar() {
    clear();

    mvprintw(0, 2, "TETRIS GAME");
    mvprintw(0, 20, "SKOR: %d", skor);

    for (int i = 0; i < TINGGI; i++) {
        for (int j = 0; j < LEBAR; j++) {
            char simbol = papan[i][j] ? '#' : '.';

            for (int a = 0; a < 4; a++) {
                for (int b = 0; b < 4; b++) {
                    if (balok[a][b] &&
                        i == y+a && j == x+b)
                        simbol = '#';
                }
            }

            mvaddch(i+2, j*2+5, simbol);
        }
    }

    mvprintw(23, 2, "A/D: Gerak | W: Putar | S: Turun");
    mvprintw(24, 2, "Q: Keluar");
    refresh();
}
