#include <iostream>   // Untuk cout, cin, dan getline()
#include <fstream>    // Untuk membaca & menulis data_pemain.txt
#include <string>     // Untuk tipe data string
#include <ncurses.h>  // Library tampilan terminal ncurses
#include <cstdlib>    // Untuk rand() dan srand()
#include <ctime>      // Untuk time(0)
#include <unistd.h>   // Untuk usleep() pada animasi Game Over

using namespace std;

/*
 =====================================================================
 BAGIAN 1: VARIABEL GLOBAL & DATA PERMAINAN
 =====================================================================
 */

const int TINGGI = 20;      // Tinggi papan Tetris (20 baris)
const int LEBAR = 10;       // Lebar papan Tetris (10 kolom)
const int MAX_DATA = 1000;  // Kapasitas maksimal array data_pemain.txt

/*
 Array 2D papan permainan:
 0 = kotak kosong
 1 - 7 = kotak terisi balok (angkanya sekaligus jadi kode warna)
 */
int papan[TINGGI][LEBAR] = {0};
int skor = 0;

// 7 bentuk dasar balok Tetris dalam matriks 4x4
int bentuk[7][4][4] = {
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk I
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk L
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk J
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk O
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk S
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }, // Bentuk T
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }  // Bentuk Z
};

int balok[4][4];       // Balok yang sedang jatuh saat ini
int x, y;              // Posisi kolom (x) dan baris (y) balok aktif
int warnaBalok = 1;    // Kode warna acak (1..7) untuk balok aktif

/*
 Dua array sejajar berkapasitas 1000 data untuk menyimpan
 isi file "data_pemain.txt".
 Indeks yang sama saling berpasangan (daftarNama[0] -> daftarSkor[0]).
 */
string daftarNama[MAX_DATA];
int daftarSkor[MAX_DATA];
int jumlahData = 0;

string namaPemain = "";      // Nama pemain yang sedang bermain
string pemainTerakhir = "";  // Nama pemain terakhir dari file .txt
