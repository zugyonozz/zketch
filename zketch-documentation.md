# 🧩 Zketch Engine Documentation

Dokumentasi ini mencakup dua bagian utama dari sistem engine UI mini Zketch:

- **`zenv.h`** → Definisi tipe SDL yang disederhanakan
- **`zbj.h`** → Kelas objek grafis fleksibel `zbj` untuk rendering dan manajemen objek

---

## 📘 `zenv.h` - SDL Type Aliases

File `zenv.h` menyediakan alias untuk tipe-tipe SDL3 agar penulisan kode menjadi lebih singkat, konsisten, dan mudah dibaca.

### 📦 Header SDL yang Digunakan
```cpp
#include "SDL3/SDL.h"
#include "SDL3/SDL_image.h"
#include "SDL3/SDL_ttf.h"
```

### 🔧 Type Alias

| Alias      | SDL Asli         | Deskripsi                                                 |
|------------|------------------|------------------------------------------------------------|
| `Font`     | `TTF_Font*`      | Font TTF yang digunakan untuk menggambar teks             |
| `Bound`    | `SDL_Rect`       | Batas persegi (integer) untuk objek                       |
| `FBound`   | `SDL_FRect`      | Batas persegi (float) untuk rendering presisi             |
| `Point`    | `SDL_Point`      | Titik koordinat (x, y) berbasis integer                   |
| `FPoint`   | `SDL_FPoint`     | Titik koordinat (x, y) berbasis float                     |
| `Color`    | `SDL_Color`      | Warna objek (RGBA)                                        |
| `Texture`  | `SDL_Texture*`   | Texture hasil render/caching dari SDL                     |
| `Window`   | `SDL_Window*`    | Window utama aplikasi SDL                                 |
| `Renderer` | `SDL_Renderer*`  | Renderer 2D untuk menggambar ke window                    |
| `Surface`  | `SDL_Surface*`   | Permukaan bitmap mentah sebelum dikonversi jadi texture   |
| `Event`    | `SDL_Event`      | Event dari input atau sistem (keyboard, mouse, dsb)       |

---

## 🎨 `zbj.h` - Objek Grafis `zbj`

Kelas `zbj` adalah bagian dari sistem render Zketch yang menangani objek dengan posisi, warna, texture, dan anchor point. Dapat digunakan untuk menggambar bentuk, teks, dan gambar.

---

### 🧱 Struktur Pendukung

#### AnchorType (enum class)
Enumerasi tipe anchor yang menentukan titik referensi posisi objek:

- `ANCHOR_TOP_LEFT`
- `ANCHOR_TOP_MID`
- `ANCHOR_TOP_RIGHT`
- `ANCHOR_LEFT_MID`
- `ANCHOR_RIGHT_MID`
- `ANCHOR_BOT_LEFT`
- `ANCHOR_BOT_MID`
- `ANCHOR_BOT_RIGHT`
- `ANCHOR_CENTER`

#### Anchor (struct)
Mewakili posisi referensi dan posisi sekarang:

| Field     | Tipe   | Deskripsi                          |
|-----------|--------|------------------------------------|
| `origin`  | Bound  | Batas awal objek                   |
| `current` | Point  | Posisi anchor hasil kalkulasi      |

---

### 🧩 Properti `zbj`

| Properti     | Tipe                        | Deskripsi                                      |
|--------------|-----------------------------|------------------------------------------------|
| `bounds`     | `std::vector<Anchor>`       | Daftar posisi anchor dan bound                |
| `color`      | `Color`                     | Warna objek saat ini                          |
| `renderer`   | `Renderer`                  | Renderer SDL yang digunakan                   |
| `textures`   | `std::vector<Texture>`      | Cache texture untuk masing-masing item        |
| `ID`         | `size_t`                    | ID dari item yang sedang aktif                |
| `anchor`     | `std::vector<AnchorType>`   | Tipe anchor dari masing-masing item           |

---

### ⚙️ Konstruktor & Destruktor

#### `zbj(Renderer renderer)`
Membuat objek `zbj` kosong dengan renderer yang ditentukan.

#### `zbj(Bound bound, Color color, Renderer renderer)`
Membuat objek `zbj` dengan bound awal, warna, dan renderer.

#### `~zbj()`
Menghapus dan membersihkan semua resource yang digunakan.

---

### 🖌️ Fungsi Render

| Fungsi                                                 | Deskripsi                                             |
|--------------------------------------------------------|--------------------------------------------------------|
| `bool drawLine()`                                      | Gambar garis (fitur opsional, tergantung implementasi).|
| `bool draw(float radiusScale = 0.0f)`                  | Gambar bentuk dari item aktif, dengan radius opsional. |
| `bool draw(const Font font, const char* text, Point)`  | Gambar teks di posisi tertentu.                        |
| `bool draw(const char* path)`                          | Gambar gambar dari path file.                          |

---

### 🛠️ Manajemen Item

| Fungsi                                      | Deskripsi                                               |
|---------------------------------------------|----------------------------------------------------------|
| `bool clearItems()`                         | Menghapus semua item, anchor, dan textures.             |
| `bool show(size_t ID)`                      | Menampilkan item dengan ID tertentu.                    |
| `bool show()`                               | Menampilkan item yang sedang aktif.                     |
| `void setBound(const Bound& newBound)`      | Mengubah bound dari item aktif.                         |
| `void setColor(const Color& newColor)`      | Mengatur warna untuk item aktif.                        |
| `void addItem()`                            | Menambahkan item baru ke daftar.                        |
| `bool removeItem(size_t index)`             | Menghapus item berdasarkan indeks.                      |
| `void setActiveID(size_t newID)`            | Mengatur ID item yang sedang aktif.                     |
| `void setAnchorPt(AnchorType anchor)`       | Mengatur anchor point dari item aktif.                  |

---

### 🔍 Getter

| Fungsi                                         | Deskripsi                                              |
|------------------------------------------------|---------------------------------------------------------|
| `const Bound getRealBound(size_t index) const` | Mendapatkan bound dari item ke-`index`.                |
| `const size_t& getID() const`                  | Mengambil ID item aktif saat ini.                      |
| `const std::vector<Anchor>& getBounds() const` | Mengambil seluruh data anchor dan bound.               |
| `const std::vector<Texture>& getTextures() const` | Mengambil seluruh texture yang tersimpan.           |

---

### 📝 Catatan

- `zbj` berfungsi sebagai unit grafis fleksibel berbasis anchor dan bound.
- Menggunakan SDL3 sebagai backend utama.
- Item dikelola dalam bentuk array dengan ID aktif untuk efisiensi kontrol.

---
---

## 🪟 `zmain.h` - Sistem Manajemen Window `zmain`

Kelas `zmain` bertanggung jawab atas manajemen jendela utama dan renderer dalam aplikasi SDL berbasis Zketch.

### 🗂️ Struktur Pendukung

#### `WinData` (struct)
Struktur data untuk menyimpan informasi konfigurasi awal window.

| Field   | Tipe              | Deskripsi                        |
|---------|-------------------|----------------------------------|
| `title` | `const char*`     | Judul window                     |
| `w`     | `int`             | Lebar window                     |
| `h`     | `int`             | Tinggi window                    |
| `flags` | `SDL_WindowFlags` | Opsi/flag SDL untuk window       |

---

### 🧩 Properti `zmain`

| Properti    | Tipe      | Deskripsi                                 |
|-------------|-----------|--------------------------------------------|
| `window`    | `Window`  | Objek window SDL utama                    |
| `renderer`  | `Renderer`| Renderer SDL untuk menggambar objek       |
| `wd`        | `WinData` | Data konfigurasi window                   |

---

### ⚙️ Fungsi `zmain`

| Fungsi                                       | Deskripsi                                                        |
|----------------------------------------------|-------------------------------------------------------------------|
| `zmain()`                                    | Konstruktor default                                               |
| `zmain(const WinData& wd)`                   | Konstruktor dengan data window                                   |
| `bool initZketch(const WinData& wd)`         | Inisialisasi SDL, window, dan renderer dengan data `WinData`     |
| `WinData getWinSize() const`                 | Mengambil ukuran window saat ini                                 |
| `Renderer& getRenderer()`                    | Mendapatkan referensi renderer aktif                             |
| `bool present()`                             | Menampilkan isi renderer ke window                               |
| `bool clearRender(Color color)`              | Membersihkan renderer dengan warna tertentu                      |
| `void delay(Uint32 ms)`                      | Menunda proses selama `ms` milidetik                             |
| `void updateWinSize()`                       | Memperbarui data ukuran window                                   |
| `void setWindowIcon(const char* path)`       | Mengatur ikon window dari file path                              |
| `~zmain()`                                   | Destruktor - membersihkan semua resource                         |

---

### 📝 Catatan

- `zmain` adalah inti utama sistem windowing dari Zketch.
- SDL3 digunakan sebagai backend grafis utama.
- Semua renderer yang digunakan `zbj` maupun objek lain mengacu pada renderer dari `zmain`.


---

## ✒️ `zfont.h` - Manajemen Font

Kelas `zfont` bertanggung jawab untuk memuat, menyimpan, dan menyediakan font dalam sistem Zketch menggunakan SDL_ttf.

### 🗂️ Struktur Pendukung

#### `FontData` (struct)
Struktur penyimpan data font berdasarkan nama dan ukuran.

| Field   | Tipe    | Deskripsi                   |
|---------|---------|-----------------------------|
| `font`  | `Font`  | Pointer ke font (TTF_Font*) |
| `size`  | `size_t`| Ukuran font yang dimuat     |

#### `Fonts` (typedef)
Tipe alias:
```cpp
using Fonts = std::unordered_map<std::string, FontData>;
```
Digunakan untuk menyimpan font dengan kunci string nama font.

---

### 🧩 Properti `zfont`

| Properti | Tipe  | Deskripsi                     |
|----------|-------|-------------------------------|
| `fonts`  | `Fonts` | Koleksi font yang dimuat    |

---

### ⚙️ Fungsi `zfont`

| Fungsi                                                                 | Deskripsi                                                     |
|------------------------------------------------------------------------|----------------------------------------------------------------|
| `zfont()`                                                              | Konstruktor default                                           |
| `bool loadFont(const std::string& name, size_t size, const std::string& path)` | Memuat font dari file dan menyimpannya dengan nama tertentu   |
| `const Font getFont(const std::string& name, int size) const`         | Mengambil pointer font berdasarkan nama dan ukuran            |
| `bool cleanUpFonts()`                                                 | Membersihkan semua font yang dimuat                           |
| `~zfont()`                                                             | Destruktor - menghapus semua resource font                    |

---

### 📝 Catatan

- `zfont` memanfaatkan SDL_ttf untuk menangani font.
- Sangat berguna untuk menggambar teks di kelas seperti `zbj`.
- Font disimpan dalam bentuk map agar dapat dengan cepat diakses berdasarkan nama.

# `utils.h` - Fungsi Utilitas

Berisi fungsi-fungsi utilitas umum untuk konversi data, seperti mengkonversi karakter hex menjadi integer dan mengkonversi nilai hex ke warna SDL.

---

## ⚙️ Fungsi `utils.h`

| Fungsi                                                           | Deskripsi                                                       |
|------------------------------------------------------------------|-----------------------------------------------------------------|
| `int hexCharToInt(char c)`                                        | Mengkonversi karakter hex (0-9, a-f, A-F) menjadi nilai integer |
| `Color Hex(const char* hex)`                                      | Mengkonversi string hex (RGB, RGBA, RRGGBB, RRGGBBAA) menjadi `SDL_Color` |

---

## 📝 Catatan

- Fungsi `Hex` mendukung format warna hex seperti `#RGB`, `#RGBA`, `#RRGGBB`, dan `#RRGGBBAA`.
- Fungsi `hexCharToInt` digunakan oleh `Hex` untuk mengubah karakter hex menjadi nilai integer untuk komponen warna.

---

# `zevent.h` - Pengelolaan Event dan Interaksi Pengguna

Berisi fungsi-fungsi untuk menangani interaksi pengguna dengan elemen UI, seperti hover dan klik, menggunakan posisi mouse dan event dari SDL.

## ⚙️ Struktur dan Tipe Data

### `using zFunctions = std::pair<std::function<bool()>, std::function<bool()>>;`
- `zFunctions` adalah pasangan dari dua fungsi yang mengembalikan nilai boolean. Fungsi pertama digunakan untuk kondisi hover yang tidak aktif, dan fungsi kedua digunakan untuk kondisi hover yang aktif.

### `using zFollowUp = std::pair<Bound, std::function<void()>>;`
- `zFollowUp` adalah pasangan yang berisi:
  - `Bound` adalah batasan area untuk interaksi.
  - Fungsi yang dipanggil ketika area tersebut dipilih atau diklik.

### `using zFollowUps = std::pair<Bound, zFunctions>;`
- `zFollowUps` adalah pasangan yang berisi:
  - `Bound` sebagai area interaksi.
  - `zFunctions` yang mengandung dua fungsi terkait untuk interaksi hover.

### `FPoint getMousePos()`
- Fungsi ini mengembalikan posisi mouse saat ini dalam bentuk `FPoint`, yang menyimpan koordinat `(x, y)` dari posisi mouse.
- Fungsi ini menggunakan `SDL_PumpEvents()` untuk memperbarui event queue dan `SDL_GetMouseState()` untuk mendapatkan posisi mouse.

### `void isHover(FPoint coord, std::vector<zFollowUps>& funcs)`
- Fungsi ini digunakan untuk memeriksa apakah posisi mouse (`coord`) berada dalam area objek yang diberikan dalam vector `funcs`.
- Jika posisi mouse berada dalam area objek (`Bound`), maka fungsi kedua dari `zFunctions` akan dijalankan (fungsi aktif). Jika tidak, fungsi pertama dari `zFunctions` akan dijalankan (fungsi tidak aktif).

### `void isOnClick(const Event& event, std::vector<zFollowUp>& func)`
- Fungsi ini memeriksa apakah event klik mouse (`SDL_EVENT_MOUSE_BUTTON_DOWN`) terjadi dalam area objek tertentu.
- Jika klik terjadi dalam area objek, maka fungsi yang terkait dengan klik tersebut (dalam pasangan `zFollowUp`) akan dijalankan.

---

## 📝 Catatan

- **`getMousePos`**: Mengambil posisi mouse saat ini. Hal ini diperlukan untuk interaksi seperti hover dan klik pada elemen UI.
- **`isHover`**: Memeriksa apakah mouse berada di dalam area objek dan menjalankan aksi yang sesuai berdasarkan kondisi (hover aktif atau tidak).
- **`isOnClick`**: Mengidentifikasi apakah klik terjadi pada area tertentu dan menjalankan aksi yang terkait dengan klik tersebut.

---

# `zketch.h` - Header Utama untuk Framework Zketch

`zketch.h` adalah file header utama yang menggabungkan semua komponen penting dari framework Zketch. File ini menyatukan elemen-elemen seperti objek grafis, pengelolaan font, event handling, window management, dan utilitas.

## 🛠️ Struktur dan Inklusi

File ini mengimpor berbagai header lain yang membentuk dasar dari framework Zketch, yaitu:

### **`#include "zbj.h"`**
- Menyertakan deklarasi untuk kelas `zbj`, yang digunakan untuk menggambar objek-objek grafis, menangani warna, dan manajemen ID objek.

### **`#include "zfont.h"`**
- Menyertakan kelas `zfont` yang digunakan untuk memuat dan mengelola font yang digunakan dalam aplikasi.

### **`#include "zevent.h"`**
- Menyertakan header untuk event handling, termasuk pengelolaan interaksi pengguna seperti hover dan klik pada objek UI.

### **`#include "zmain.h"`**
- Menyertakan kelas `zmain` untuk pengelolaan window dan renderer, serta pengaturan dasar SDL seperti inisialisasi dan rendering.

### **`#include "utils.h"`**
- Menyertakan berbagai fungsi utilitas seperti konversi warna dari string hex ke `SDL_Color`.

## 📜 Deskripsi Umum

`zketch.h` berfungsi sebagai entry point bagi framework Zketch. Dengan menyertakan file ini, pengguna akan mendapatkan akses ke berbagai komponen yang dapat digunakan untuk membuat aplikasi berbasis grafis, mengelola font, menangani event dari pengguna, serta memanipulasi window dan rendering.

---

## 💻 Penggunaan Umum

Berikut adalah gambaran umum tentang bagaimana `zketch.h` dapat digunakan dalam aplikasi berbasis SDL:

1. **Mengelola Window dan Renderer**
   Dengan `zmain`, Anda dapat membuat dan mengelola window, serta renderer untuk menggambar objek grafis ke layar.

2. **Menggambar dan Mengelola Objek Grafis**
   `zbj` menyediakan kelas untuk menggambar objek-objek grafis seperti garis, teks, dan gambar. Anda dapat mengatur warna dan batasan untuk objek ini.

3. **Event Handling**
   `zevent.h` menangani event dari pengguna, seperti klik dan hover, yang memungkinkan interaksi dengan elemen UI.

4. **Pengelolaan Font**
   Dengan `zfont`, Anda dapat memuat dan mengelola font yang digunakan untuk menampilkan teks pada aplikasi.

---