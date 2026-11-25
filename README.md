# Projek-GameTetris
Game tetris berbasis terminal ncurses c++  
---
## Daftar Isi  
1.[Latar Belakang & Studi Kasus](#-latar-belakang--studi-kasus)  
2.[Keunggulan Game](#-keunggulan-game)    
3.[Prasyarat & Instalasi](#-prasyarat--instalasi)  
4.[Cara Kompilasi](#-cara-kompilasi)  
5.[Kontrol Permainan](#-kontrol-permainan)  
6.[Credits](#-credits)  

---

## 🚀 Latar Belakang & Studi Kasus  
Latar Belakang :
Pengembangan game Tetris berbasis teks ini menggunakan bahasa C++ dan pustaka ncurses untuk mengimplementasikan logika pemrograman terstruktur. Dengan memanfaatkan matriks ukuran 20 \times 10 (papan) serta array 3 dimensi untuk 7 variasi bentuk balok (bentuk), proyek ini bertujuan untuk melatih pemahaman mengenai manipulasi array multidimensi, algoritma logika spasial, dan pengelolaan antarmuka terminal interaktif secara real-time. 

Studi Kasus :
Proyek ini membahas pembuatan permainan Tetris berbasis terminal. Pemain dapat menggerakkan dan memutar balok menggunakan keyboard agar tersusun dengan baik.
Program menggunakan array dua dimensi untuk papan permainan, fungsi untuk mengatur jalannya game, serta percabangan dan perulangan untuk mengatur pergerakan dan mendeteksi tabrakan. Setiap baris yang berhasil dipenuhi akan dihapus dan menghasilkan tambahan 100 poin. Permainan berakhir ketika balok baru tidak dapat ditempatkan lagi.
Melalui proyek ini, mahasiswa dapat memahami penerapan konsep dasar pemrograman C++ dalam pembuatan sebuah permainan sederhana.

---  
## 👾 Keunggulan Game  
* Ringan dan Berjalan Langsung di Terminal (Tanpa GUI): Dibangun menggunakan pustaka ncurses, game dapat dijalankan secara interaktif di terminal Linux maupun WSL tanpa memerlukan lingkungan grafis (GUI) atau mesin game yang berat.
* ​Kontrol Ganda & Bebas Lag: Mendukung tombol panah sekaligus W/A/S/D. Fungsi timeout(50) menjaga pergerakan dan putaran balok tetap instan tanpa antrean perintah yang menumpuk setelah tombol dilepas.
* Warna Balok Acak yang Menetap: Setiap balok baru diundi dengan 7 warna dasar ncurses berlatar hitam (COLOR_PAIR). Warna tersebut tersimpan di matriks papan agar tetap menempel setelah balok mendarat.
* Menu Utama Dinamis: Menggunakan kotak berbingkai (newwin & box), efek sorotan (A_REVERSE), serta navigasi melingkar. Opsi "Main Lagi" hanya muncul otomatis jika riwayat pemain sudah tersimpan di file data_pemain.txt.
* Selebrasi Rekor Baru: Saat pemain melampaui rekor tertinggi, layar Game Over menampilkan animasi ketik huruf demi huruf berwarna kuning memakai usleep() dan mengubah teks skor akhir menjadi kuning.
* ​Keluar Bersih Tanpa Error: Menekan tombol q atau Q akan menjalankan endwin() untuk menutup mode ncurses dengan aman, sehingga tampilan terminal kembali normal.
  
---

## 🌟 Prasyarat & Instalasi  
Pastikan lingkungan pengembangan memiliki:  
* *OS:* Linux (Ubuntu/Debian/lainnya) atau *Windows dengan WSL (Windows Subsystem for Linux)*, karena program menggunakan pustaka <ncurses.h> dan <unistd.h> (POSIX).
* *Compiler C++:* g++ (GNU C++ Compiler).
* - *Library Ncurses:* Paket pengembangan libncurses-dev yang terinstal di Linux / WSL.
Instalasi di WSL (Ubuntu/Debian)
Jika g++ atau ncurses belum terpasang di WSL, jalankan perintah berikut di terminal WSL:
```bash```
sudo apt update
sudo apt install g++ libncurses-dev

---

## 💎 Cara Kompilasi  
Buka terminal direktori proyek,lalu jalankan :
```bash
g++ main.cpp -o main -lncurses && ./main
```
---

## 🎮 Kontrol Permainan
| Aksi        | Tombol Keyboard  |  
|-------------|------------------|
| Putar balok | W/Panah atas     |
| Geser Kanan | S/Panah kanan    |
| Geser Kiri  | A/Panah kiri     |
| Turun cepat | S/Panah bawah    |
| Keluar game | Q                |

---

## 📝 Credits
Universitas Lampung
| Nama              | NPM        |
|-------------------|------------|
| Dwantara Effendy  | 2617051047 |
| Sevinna Vierlyani | 2617051034 |
| Suci Widianti     | 2617051013 |

---


