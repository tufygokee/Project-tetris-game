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

/*
 =====================================================================
 BAGIAN 2: PENGATURAN WARNA NCURSES (ANIMASI TEKS)
 =====================================================================
 */

void aturWarna() {
    start_color(); // Wajib dipanggil sebelum membuat pasangan warna

    // 7 pasangan warna dasar berlatar hitam untuk balok & tampilan menu
    init_pair(1, COLOR_RED, COLOR_BLACK);     // Merah
    init_pair(2, COLOR_GREEN, COLOR_BLACK);   // Hijau
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // Kuning (Emas/Perunggu/Rekor)
    init_pair(4, COLOR_BLUE, COLOR_BLACK);    // Biru
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // Ungu
    init_pair(6, COLOR_CYAN, COLOR_BLACK);    // Biru muda (Judul/Bingkai)
    init_pair(7, COLOR_WHITE, COLOR_BLACK);   // Putih (Silver/Teks biasa)
}
/*
 =====================================================================
 BAGIAN 3: OPERASI FILE TEXT "data_pemain.txt" (FILE TEXT)
 =====================================================================
 */

/*
 Membaca seluruh data dari "data_pemain.txt" ke dalam array.
 Format isi file per pemain adalah 2 baris:
   Baris 1: Nama Pemain (bisa pakai spasi/simbol)
   Baris 2: Skor (angka bulat)
 */
void muatData() {
    jumlahData = 0;
    pemainTerakhir = "";

    ifstream fileBaca;
    fileBaca.open("data_pemain.txt");

    // Jika file belum ada, langsung keluar (jumlahData tetap 0)
    if (fileBaca.fail()) {
        return;
    }

    string nama;
    int nilaiSkor;

    // Baca terus selama masih ada baris dan belum melebihi 1000 data
    while (jumlahData < MAX_DATA && getline(fileBaca, nama)) {
        if (nama == "") {
            continue; // Lewati jika ada baris kosong
        }

        if (fileBaca >> nilaiSkor) {
            fileBaca.ignore(); // Membuang sisa tombol Enter setelah angka skor

            daftarNama[jumlahData] = nama;
            daftarSkor[jumlahData] = nilaiSkor;
            pemainTerakhir = nama;
            jumlahData++;
        }
    }

    fileBaca.close(); // Tutup file setelah selesai dibaca
}

/*
 Menyimpan 1 data pemain baru ke bagian paling bawah "data_pemain.txt".
 Memakai ios::app agar data pemain yang sudah ada tidak terhapus.
 */
void simpanData(string nama, int skorBaru) {
    muatData(); // Perbarui jumlahData dulu agar akurat

    // Jangan simpan jika kapasitas array 1000 data sudah penuh
    if (jumlahData >= MAX_DATA) {
        return;
    }

    ofstream fileTulis;
    fileTulis.open("data_pemain.txt", ios::app);

    if (!fileTulis.fail()) {
        fileTulis << nama << endl;
        fileTulis << skorBaru << endl;
        fileTulis.close();
    }
}

// Mencari nilai skor paling tinggi dari seluruh data yang tersimpan
int cariSkorTertinggi() {
    int tertinggi = 0;
    for (int i = 0; i < jumlahData; i++) {
        if (daftarSkor[i] > tertinggi) {
            tertinggi = daftarSkor[i];
        }
    }
    return tertinggi;
}

