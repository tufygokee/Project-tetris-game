# Project-tetris-game  
Game tetris berbasis terminal ncurses c++  
---
## Daftar Isi  
1.[Latar Belakang & Studi Kasus](#-latar-belakang--studi-kasus)  
2.[Keunggulan Game](#-Keunggulan-Game)    
3.[Prasyarat & Instalasi](#-prasyarat--instalasi)  
4.[Cara Kompilasi](#-cara-kompilasi)  
5.[Kontrol Permainan](#-kontrol-permainan)  
6.[Aset & File Eksternal](#-aset--file-eksternal)  
7.[Credits](#-credits)

---

## 1.Latar Belakang & Studi Kasus  
Latar Belakang :
Pengembangan game Tetris berbasis teks ini menggunakan bahasa C++ dan pustaka ncurses untuk mengimplementasikan logika pemrograman terstruktur. Dengan memanfaatkan matriks ukuran 20 \times 10 (papan) serta array 3 dimensi untuk 7 variasi bentuk balok (bentuk), proyek ini bertujuan untuk melatih pemahaman mengenai manipulasi array multidimensi, algoritma logika spasial, dan pengelolaan antarmuka terminal interaktif secara real-time. 

Studi Kasus :
Proyek ini membahas pembuatan permainan Tetris berbasis terminal. Pemain dapat menggerakkan dan memutar balok menggunakan keyboard agar tersusun dengan baik.
Program menggunakan array dua dimensi untuk papan permainan, fungsi untuk mengatur jalannya game, serta percabangan dan perulangan untuk mengatur pergerakan dan mendeteksi tabrakan. Setiap baris yang berhasil dipenuhi akan dihapus dan menghasilkan tambahan 100 poin. Permainan berakhir ketika balok baru tidak dapat ditempatkan lagi.
Melalui proyek ini, mahasiswa dapat memahami penerapan konsep dasar pemrograman C++ dalam pembuatan sebuah permainan sederhana.

---  
## 2. Keunggulan Game  
* Ringan dan Berjalan Langsung di Terminal (Tanpa GUI): Dibangun menggunakan pustaka ncurses, game dapat dijalankan secara interaktif di terminal Linux maupun WSL tanpa memerlukan lingkungan grafis (GUI) atau mesin game yang berat. 





