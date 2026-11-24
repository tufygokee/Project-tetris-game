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
