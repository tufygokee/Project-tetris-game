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
/*
 =====================================================================
 BAGIAN 4: BUBBLE SORT DESCENDING (SORTING)
 =====================================================================
 */

/*
 Mengurutkan papan peringkat dari skor TERBESAR ke TERKECIL.
 Saat dua skor ditukar, namanya ikut ditukar agar tidak tertukar orangnya.
 */
void urutkanBubbleSort() {
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            // Tanda '<' dipakai untuk mengurutkan dari besar ke kecil
            if (daftarSkor[j] < daftarSkor[j + 1]) {
                // Tukar skor memakai gelas sementara (tempSkor)
                int tempSkor = daftarSkor[j];
                daftarSkor[j] = daftarSkor[j + 1];
                daftarSkor[j + 1] = tempSkor;

                // Tukar juga namanya memakai gelas sementara (tempNama)
                string tempNama = daftarNama[j];
                daftarNama[j] = daftarNama[j + 1];
                daftarNama[j + 1] = tempNama;
            }
        }
    }
}
/*
 =====================================================================
 BAGIAN 5: LOGIKA UTAMA GAME TETRIS
 =====================================================================
 */

// Mengosongkan papan dan mereset skor sebelum permainan baru dimulai
void resetPapan() {
    for (int i = 0; i < TINGGI; i++) {
        for (int j = 0; j < LEBAR; j++) {
            papan[i][j] = 0;
        }
    }
    skor = 0;
}

// Membuat balok baru secara acak beserta warna acaknya (1..7)
void balokBaru() {
    int jenis = rand() % 7;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            balok[i][j] = bentuk[jenis][i][j];
        }
    }

    warnaBalok = (rand() % 7) + 1; // Pilih warna acak ID 1 sampai 7
    x = LEBAR / 2 - 2;             // Muncul di tengah atas papan
    y = 0;
}

// Mengecek apakah posisi tujuan (nx, ny) menabrak dinding, lantai, atau balok lain
bool tabrakan(int nx, int ny, int b[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b[i][j] != 0) {
                int px = nx + j;
                int py = ny + i;

                // Cek batas dinding kiri, dinding kanan, dan lantai
                if (px < 0 || px >= LEBAR || py >= TINGGI) {
                    return true;
                }

                // Cek tabrakan dengan balok yang sudah menumpuk di papan
                if (py >= 0 && papan[py][px] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Memutar balok 90 derajat searah jarum jam jika tidak menabrak
void putarBalok() {
    int hasil[4][4] = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            hasil[j][3 - i] = balok[i][j];
        }
    }

    if (!tabrakan(x, y, hasil)) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                balok[i][j] = hasil[i][j];
            }
        }
    }
}

// Mengunci balok yang sudah mendarat ke dalam array papan beserta warnanya
void kunciBalok() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (balok[i][j] != 0) {
                if (y + i >= 0 && y + i < TINGGI) {
                    papan[y + i][x + j] = warnaBalok;
                }
            }
        }
    }
}

/*
 Menghapus baris yang penuh (tidak ada angka 0), menurunkan baris di atasnya,
 lalu menambah skor +100 untuk setiap baris yang berhasil dibersihkan.
 */
void hapusBaris() {
    for (int i = TINGGI - 1; i >= 0; i--) {
        bool penuh = true;

        for (int j = 0; j < LEBAR; j++) {
            if (papan[i][j] == 0) {
                penuh = false;
                break;
            }
        }

        if (penuh) {
            // Geser semua baris di atasnya turun 1 tingkat
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < LEBAR; j++) {
                    papan[k][j] = papan[k - 1][j];
                }
            }

            // Kosongkan baris paling atas
            for (int j = 0; j < LEBAR; j++) {
                papan[0][j] = 0;
            }

            skor += 100;
            i++; // Cek ulang baris yang sama karena ada baris baru yang turun
        }
    }
}

// Menggambar arena Tetris di kiri dan papan info/kontrol di kanan
void gambarGame(int rekorLama) {
    erase();

    // Gambar bingkai arena Tetris (ukuran 20 baris x 10 kolom)
    attron(COLOR_PAIR(6));
    mvprintw(1, 2, "+--------------------+");
    for (int i = 0; i < TINGGI; i++) {
        mvprintw(2 + i, 2, "|");
        mvprintw(2 + i, 23, "|");
    }
    mvprintw(22, 2, "+--------------------+");
    attroff(COLOR_PAIR(6));

    // Gambar isi papan dan balok aktif berwarna acak
    for (int i = 0; i < TINGGI; i++) {
        for (int j = 0; j < LEBAR; j++) {
            int warna = papan[i][j]; // 0 jika kosong, 1..7 jika ada balok

            // Cek apakah koordinat (i, j) ditempati balok yang sedang jatuh
            for (int a = 0; a < 4; a++) {
                for (int b = 0; b < 4; b++) {
                    if (balok[a][b] != 0 && i == y + a && j == x + b) {
                        warna = warnaBalok;
                    }
                }
            }

            if (warna != 0) {
                // Cetak balok berwarna menggunakan COLOR_PAIR()
                attron(COLOR_PAIR(warna) | A_BOLD);
                mvprintw(2 + i, 3 + j * 2, "[]");
                attroff(COLOR_PAIR(warna) | A_BOLD);
            } else {
                // Cetak titik sebagai penanda kotak kosong
                mvprintw(2 + i, 3 + j * 2, ". ");
            }
        }
    }

    // Panel informasi & kontrol di sebelah kanan papan
    attron(COLOR_PAIR(6) | A_BOLD);
    mvprintw(2, 28, "=== TETRIS GAME ===");
    attroff(COLOR_PAIR(6) | A_BOLD);

    mvprintw(4, 28, "Pemain : %s", namaPemain.c_str());
    mvprintw(5, 28, "Skor   : %d", skor);
    mvprintw(6, 28, "Rekor  : %d", rekorLama);

    mvprintw(9, 28, "KONTROL:");
    mvprintw(10, 28, "A / Panah Kiri  : Geser Kiri");
    mvprintw(11, 28, "D / Panah Kanan : Geser Kanan");
    mvprintw(12, 28, "W / Panah Atas  : Putar Balok");
    mvprintw(13, 28, "S / Panah Bawah : Turun Cepat");
    mvprintw(14, 28, "Q               : Keluar");

    refresh(); // Tampilkan semua gambar ke layar terminal
}

/*
 =====================================================================
 BAGIAN 6: LAYAR GAME OVER & ANIMASI SKOR TERTINGGI
 =====================================================================
 */

void tampilGameOver(int skorAkhir, bool rekorBaru) {
    clear();
    refresh();

    // Membuat jendela kotak Game Over di tengah layar
    WINDOW *overWin = newwin(12, 46, 5, 15);
    box(overWin, 0, 0);

    wattron(overWin, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(overWin, 2, 18, "GAME OVER!");
    wattroff(overWin, COLOR_PAIR(1) | A_BOLD);

    mvwprintw(overWin, 4, 4, "Pemain     : %s", namaPemain.c_str());
    mvwprintw(overWin, 5, 4, "Skor Akhir : %d", skorAkhir);
    wrefresh(overWin);

    /*
     Jika pemain memecahkan skor tertinggi (skor > 0 dan di atas rekor lama):
     1. Muncul animasi tulisan "SKOR TERTINGGI BARU!" warna kuning
     2. Warna tulisan Skor Akhir ikut berubah menjadi warna kuning
     */
    if (rekorBaru) {
        usleep(300000); // Jeda 0,3 detik sebelum animasi dimulai

        string teksRekor = "*** SKOR TERTINGGI BARU! ***";
        wattron(overWin, COLOR_PAIR(3) | A_BOLD);

        // Animasi muncul huruf demi huruf menggunakan usleep()
        for (int i = 0; i < (int)teksRekor.length(); i++) {
            mvwprintw(overWin, 7, 9 + i, "%c", teksRekor[i]);
            wrefresh(overWin);
            usleep(60000); // Jeda 0,06 detik tiap huruf
        }

        // Ubah warna tulisan Skor Akhir menjadi warna kuning
        mvwprintw(overWin, 5, 4, "Skor Akhir : %d (TERTINGGI!)", skorAkhir);
        wattroff(overWin, COLOR_PAIR(3) | A_BOLD);
        wrefresh(overWin);
    }

    wattron(overWin, COLOR_PAIR(2));
    mvwprintw(overWin, 9, 4, "Data disimpan ke data_pemain.txt");
    wattroff(overWin, COLOR_PAIR(2));

    mvwprintw(overWin, 10, 4, "Tekan tombol apa saja untuk lanjut...");
    wrefresh(overWin);

    wgetch(overWin); // Tunggu pemain menekan tombol
    delwin(overWin); // Hapus jendela dari memori
}

// Menjalankan satu sesi penuh permainan Tetris
void mulaiGameTetris() {
    muatData();
    int rekorLama = cariSkorTertinggi();

    resetPapan();
    balokBaru();

    /*
     timeout(50) membuat getch() menunggu maksimal 50 milidetik.
     Jika pemain menekan panah, langsung diproses detik itu juga
     sehingga perintah tidak menumpuk saat tombol dilepaskan.
     */
    timeout(50);

    int waktu = 0;
    bool selesai = false;

    while (!selesai) {
        gambarGame(rekorLama);

        int tombol = getch();

        if (tombol == 'q' || tombol == 'Q') {
            break;
        }

        if (tombol == KEY_LEFT || tombol == 'a' || tombol == 'A') {
            if (!tabrakan(x - 1, y, balok)) {
                x--;
            }
        }

        if (tombol == KEY_RIGHT || tombol == 'd' || tombol == 'D') {
            if (!tabrakan(x + 1, y, balok)) {
                x++;
            }
        }

        if (tombol == KEY_UP || tombol == 'w' || tombol == 'W') {
            putarBalok();
        }

        if (tombol == KEY_DOWN || tombol == 's' || tombol == 'S') {
            if (!tabrakan(x, y + 1, balok)) {
                y++;
            }
        }

        // Gravitasi: setiap 10 putaran (~0,5 detik), balok turun 1 baris
        waktu++;
        if (waktu >= 10) {
            waktu = 0;

            if (!tabrakan(x, y + 1, balok)) {
                y++;
            } else {
                kunciBalok();
                hapusBaris();
                balokBaru();

                // Jika balok baru langsung tabrakan di atas, Game Over
                if (tabrakan(x, y, balok)) {
                    selesai = true;
                }
            }
        }
    }

    // Matikan timeout agar getch() kembali menunggu tombol secara normal
    timeout(-1);

    // Cek apakah skor pemain kali ini memecahkan rekor tertinggi
    bool rekorBaru = (skor > 0 && skor > rekorLama);

    // Simpan hasil ke data_pemain.txt lalu tampilkan layar Game Over
    simpanData(namaPemain, skor);
    tampilGameOver(skor, rekorBaru);
}


/*
 =====================================================================
 BAGIAN 7: POPUP PENCARIAN (SEQUENTIAL SEARCH) & PAPAN PERINGKAT
 =====================================================================
 */

/*
 Popup pencarian di atas Papan Peringkat:
 - Awalnya kosong bertuliskan "cari nama pemain"
 - Hilang saat pemain mengetik karakter
 - Jika dihapus habis, muncul tulisan "masukan nama pemain"
 - Tekan Enter untuk mencari dengan Sequential Search (Modul 7)
 - Tekan 'c' kedua kalinya pada tampilan hasil untuk menutup popup
 */
void popupCariPemain() {
    WINDOW *popupWin = newwin(10, 48, 6, 14);
    keypad(popupWin, TRUE);

    string kataCari = "";
    bool pernahKetik = false;  // Penanda apakah pemain pernah mengetik
    bool sudahCari = false;    // false = mode mengetik, true = tampil hasil
    bool ketemu = false;
    int indeksKetemu = -1;

    while (true) {
        werase(popupWin);
        box(popupWin, 0, 0);

        wattron(popupWin, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(popupWin, 1, 16, "CARI PEMAIN");
        wattroff(popupWin, COLOR_PAIR(6) | A_BOLD);

        // Kotak masukan nama pemain (lebar 30 karakter)
        mvwprintw(popupWin, 3, 3, "Nama: [");
        mvwprintw(popupWin, 3, 40, "]");

        if (kataCari != "") {
            // Cetak teks yang diketik pengguna
            mvwprintw(popupWin, 3, 10, "%s", kataCari.c_str());
        } else {
            // Tampilkan teks petunjuk redup (placeholder)
            wattron(popupWin, COLOR_PAIR(7) | A_DIM);
            if (!pernahKetik) {
                mvwprintw(popupWin, 3, 10, "cari nama pemain");
            } else {
                mvwprintw(popupWin, 3, 10, "masukan nama pemain");
            }
            wattroff(popupWin, COLOR_PAIR(7) | A_DIM);
        }

        if (!sudahCari) {
            // Mode Mengetik
            mvwprintw(popupWin, 5, 3, "Ketik nama lalu tekan [Enter] untuk cari.");
            mvwprintw(popupWin, 8, 3, "[Enter] Cari   |   [ESC] Batal");
            curs_set(1); // Munculkan kursor ketik
            wmove(popupWin, 3, 10 + (int)kataCari.length());
        } else {
            // Mode Hasil Pencarian
            curs_set(0); // Sembunyikan kursor

            if (ketemu) {
                wattron(popupWin, COLOR_PAIR(2) | A_BOLD);
                mvwprintw(popupWin, 5, 3, "Ditemukan di Peringkat #%d:",
                          indeksKetemu + 1);
                wattroff(popupWin, COLOR_PAIR(2) | A_BOLD);
                mvwprintw(popupWin, 6, 3, "%s  |  Skor: %d",
                          daftarNama[indeksKetemu].c_str(),
                          daftarSkor[indeksKetemu]);
            } else {
                wattron(popupWin, COLOR_PAIR(1) | A_BOLD);
                mvwprintw(popupWin, 5, 3, "Pemain tidak ditemukan!");
                wattroff(popupWin, COLOR_PAIR(1) | A_BOLD);
            }

            mvwprintw(popupWin, 8, 3, "[C] Tutup Popup   |   [Enter] Cari Lagi");
        }

        wrefresh(popupWin);

        int ch = wgetch(popupWin);

        // Tombol ESC (kode 27) dapat menutup popup kapan saja
        if (ch == 27) {
            break;
        }

        if (!sudahCari) {
            if (ch == 10 || ch == KEY_ENTER) {
                /*
                 SEQUENTIAL SEARCH :
                 Memeriksa array daftarNama satu per satu dari indeks 0
                 sampai jumlahData - 1.
                 */
                ketemu = false;
                indeksKetemu = -1;

                if (kataCari != "") {
                    for (int i = 0; i < jumlahData; i++) {
                        // Cek apakah nama sama persis atau mengandung kataCari
                        if (daftarNama[i].find(kataCari) != string::npos) {
                            ketemu = true;
                            indeksKetemu = i;
                            break; // Berhenti begitu ketemu peringkat tertinggi
                        }
                    }
                }
                sudahCari = true;
            } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
                // Hapus 1 karakter terakhir saat Backspace ditekan
                if (kataCari.length() > 0) {
                    kataCari.pop_back();
                }
            } else if (ch >= 32 && ch <= 126) {
                // Tambahkan karakter yang diketik (maksimal 30 karakter)
                if ((int)kataCari.length() < 30) {
                    kataCari += (char)ch;
                    pernahKetik = true;
                }
            }
        } else {
            // Menekan 'c' kedua kalinya menutup popup
            if (ch == 'c' || ch == 'C') {
                break;
            } else if (ch == 10 || ch == KEY_ENTER) {
                // Ulangi pencarian baru
                sudahCari = false;
                kataCari = "";
            }
        }
    }

    curs_set(0);
    delwin(popupWin);
}

// Menampilkan tabel Papan Peringkat dengan Bubble Sort & warna podium
void tampilPapanPeringkat() {
    muatData();
    urutkanBubbleSort(); // Urutkan dari skor tertinggi ke terendah

    WINDOW *rankWin = newwin(18, 56, 2, 10);
    keypad(rankWin, TRUE);

    int mulaiBaris = 0; // Untuk menggulir jika data lebih dari 10

    while (true) {
        refresh();
        werase(rankWin);
        box(rankWin, 0, 0);

        wattron(rankWin, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(rankWin, 1, 17, "PAPAN PERINGKAT TETRIS");
        wattroff(rankWin, COLOR_PAIR(6) | A_BOLD);

        mvwprintw(rankWin, 3, 2, "RANK  NAMA PEMAIN                       SKOR");
        mvwprintw(rankWin, 4, 2, "----------------------------------------------------");

        if (jumlahData == 0) {
            mvwprintw(rankWin, 8, 12, "Belum ada data di data_pemain.txt");
        } else {
            // Tampilkan maksimal 10 data per layar
            for (int baris = 0; baris < 10; baris++) {
                int idx = mulaiBaris + baris;
                if (idx >= jumlahData) {
                    break;
                }

                // Warna podium berlatar hitam untuk posisi 1, 2, dan 3
                if (idx == 0) {
                    // Posisi 1: Emas (Kuning terang tebal)
                    wattron(rankWin, COLOR_PAIR(3) | A_BOLD);
                } else if (idx == 1) {
                    // Posisi 2: Silver (Putih terang tebal)
                    wattron(rankWin, COLOR_PAIR(7) | A_BOLD);
                } else if (idx == 2) {
                    // Posisi 3: Perunggu / Coklat (Kuning redup)
                    wattron(rankWin, COLOR_PAIR(3) | A_DIM);
                } else {
                    // Posisi 4 ke bawah: Putih biasa
                    wattron(rankWin, COLOR_PAIR(7));
                }

                mvwprintw(rankWin, 5 + baris, 2, "#%-4d %-30s %8d",
                          idx + 1,
                          daftarNama[idx].c_str(),
                          daftarSkor[idx]);

                // Matikan semua atribut agar tidak bocor ke baris selanjutnya
                wattroff(rankWin, COLOR_PAIR(3) | COLOR_PAIR(7) | A_BOLD | A_DIM);
            }
        }

        mvwprintw(rankWin, 16, 2, "[C] Cari   |   [Panah] Gulir   |   [Q] Kembali");
        wrefresh(rankWin);

        int tombol = wgetch(rankWin);

        if (tombol == 'q' || tombol == 'Q') {
            break; // Keluar dari Papan Peringkat ke Menu Utama
        } else if (tombol == 'c' || tombol == 'C') {
            popupCariPemain();
        } else if (tombol == KEY_UP) {
            if (mulaiBaris > 0) {
                mulaiBaris--;
            }
        } else if (tombol == KEY_DOWN) {
            if (mulaiBaris + 10 < jumlahData) {
                mulaiBaris++;
            }
        }
    }

    delwin(rankWin);
}


/*
 =====================================================================
 BAGIAN 8: MENU MULAI BARU (INPUT getline() + TOMBOL MULAI)
 =====================================================================
 */

bool inputNamaBaru() {
    char bufferNama[31]; // Menampung maks 30 karakter + penutup teks '\0'
    string namaInput = "";
    bool kosong = false;

    clear();
    refresh();

    WINDOW *inputWin = newwin(11, 46, 5, 15);
    keypad(inputWin, TRUE);

    // 1. Input nama langsung di dalam kotak ncurses (tidak keluar ke terminal)
    do {
        werase(inputWin);
        box(inputWin, 0, 0);

        wattron(inputWin, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(inputWin, 1, 13, "MULAI BARU - TETRIS");
        wattroff(inputWin, COLOR_PAIR(6) | A_BOLD);

        mvwprintw(inputWin, 3, 4, "Masukkan nama (maks 30 karakter):");
        mvwprintw(inputWin, 5, 4, "> ");

        if (kosong) {
            wattron(inputWin, COLOR_PAIR(1) | A_BOLD);
            mvwprintw(inputWin, 7, 4, "Nama tidak boleh kosong!");
            wattroff(inputWin, COLOR_PAIR(1) | A_BOLD);
        }

        mvwprintw(inputWin, 9, 4, "Ketik nama lalu tekan [Enter]");
        wrefresh(inputWin);

        // Nyalakan echo dan kursor agar ketikan tampil (Modul Pertemuan 14)
        echo();
        curs_set(1);

        // Baca teks maksimal 30 karakter (huruf, spasi, simbol diterima)
        mvwgetnstr(inputWin, 5, 6, bufferNama, 30);

        // Matikan kembali echo dan kursor setelah selesai mengetik
        noecho();
        curs_set(0);

        namaInput = bufferNama;
        kosong = (namaInput == "");
    } while (kosong);

    // 2. Layar konfirmasi tombol MULAI dan KEMBALI di kotak yang sama
    const char *tombolOpsi[2] = {" MULAI ", " KEMBALI "};
    int pilih = 0; // Langsung menyorot tombol MULAI

    while (true) {
         werase(inputWin);
        box(inputWin, 0, 0);

        wattron(inputWin, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(inputWin, 1, 16, "SIAP BERMAIN?");
        wattroff(inputWin, COLOR_PAIR(6) | A_BOLD);

        mvwprintw(inputWin, 3, 4, "Nama: %s", namaInput.c_str());

        // Cetak tombol MULAI dan KEMBALI dengan sorotan A_REVERSE
        for (int i = 0; i < 2; i++) {
            if (i == pilih) {
                wattron(inputWin, A_REVERSE | A_BOLD);
            }
            mvwprintw(inputWin, 6, 10 + i * 16, "[%s]", tombolOpsi[i]);
            wattroff(inputWin, A_REVERSE | A_BOLD);
        }

        mvwprintw(inputWin, 9, 4, "[Panah Kiri/Kanan] Pilih  [Enter] OK");
        wrefresh(inputWin);

        int ch = wgetch(inputWin);
        if (ch == KEY_LEFT || ch == KEY_RIGHT) {
            pilih = (pilih == 0) ? 1 : 0; // Tukar sorotan tombol
        } else if (ch == 10 || ch == KEY_ENTER) {
            delwin(inputWin);
            if (pilih == 0) {
                namaPemain = namaInput;
                return true;  // Tekan MULAI -> masuk ke game
            } else {
                return false; // Tekan KEMBALI -> balik ke Menu Utama
            }
        }
    }
}


/*
 =====================================================================
 BAGIAN 9: MENU UTAMA DINAMIS (ANIMASI TEKS + TOMBOL PANAH)
 =====================================================================
 */

int tampilMenuUtama() {
    muatData(); // Cek apakah data_pemain.txt sudah berisi data

    const char *opsi[4];
    int kodeMenu[4];
    int jumlahOpsi = 0;

    // Menu 1: Mulai Baru (selalu muncul)
    opsi[jumlahOpsi] = "Mulai Baru";
    kodeMenu[jumlahOpsi] = 1;
    jumlahOpsi++;

    // Menu 2: Main Lagi (HANYA muncul jika data_pemain.txt sudah ada isinya)
    if (jumlahData > 0) {
        opsi[jumlahOpsi] = "Main Lagi";
        kodeMenu[jumlahOpsi] = 2;
        jumlahOpsi++;
    }

    // Menu 3: Papan Peringkat (selalu muncul)
    opsi[jumlahOpsi] = "Papan Peringkat";
    kodeMenu[jumlahOpsi] = 3;
    jumlahOpsi++;

    // Menu 4: Keluar (selalu muncul)
    opsi[jumlahOpsi] = "Keluar";
    kodeMenu[jumlahOpsi] = 4;
    jumlahOpsi++;

    clear();
    refresh();

    WINDOW *menuWin = newwin(12, 36, 4, 20);
    keypad(menuWin, TRUE);

    int pilihanTerpilih = 0;

    while (true) {
        werase(menuWin);
        box(menuWin, 0, 0);

        wattron(menuWin, COLOR_PAIR(6) | A_BOLD);
        mvwprintw(menuWin, 1, 9, "=== GAME TETRIS ===");
        wattroff(menuWin, COLOR_PAIR(6) | A_BOLD);

        // Cetak daftar menu dengan efek sorotan A_REVERSE (MODUUL 14)
        for (int i = 0; i < jumlahOpsi; i++) {
            if (i == pilihanTerpilih) {
                wattron(menuWin, A_REVERSE);
            }
            mvwprintw(menuWin, 3 + i, 8, " %-18s ", opsi[i]);
            wattroff(menuWin, A_REVERSE);
        }

        mvwprintw(menuWin, 9, 3, "[Panah] Pilih   [Enter] OK");
        mvwprintw(menuWin, 10, 3, "[Q] Keluar ke Terminal");
        wrefresh(menuWin);

        int tombol = wgetch(menuWin);
        switch (tombol) {
            case KEY_UP:
                if (pilihanTerpilih == 0) {
                    pilihanTerpilih = jumlahOpsi - 1;
                } else {
                    pilihanTerpilih--;
                }
                break;

            case KEY_DOWN:
                if (pilihanTerpilih == jumlahOpsi - 1) {
                    pilihanTerpilih = 0;
                } else {
                    pilihanTerpilih++;
                }
                break;

            case 10:        // Tombol Enter
            case KEY_ENTER:
                delwin(menuWin);
                return kodeMenu[pilihanTerpilih];

            case 'q':
            case 'Q':
                delwin(menuWin);
                return 4; // Keluar
        }
    }
