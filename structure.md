# zketch mini engine structure

## main lib

### RegWindow					--> data of window utk di daftar ke zkmain
- string name					--> name of window
- int w							--> w size
- int h							--> h size
- int flags						--> flags of window

### zkmain						--> utama
- init()						--> dari lib SDL3 initialisasi SDL, SDL_image, SDL_ttf
- create()						--> daftar data window dan buat window dan renderer
- destroy()						--> destroy window dan renderer

## obj of zketch

### Bound
- int x							--> x position
- int y							--> y position
- int w							--> w size
- int h							--> h size
note: Bound bisa disebut boundary

### Point
- int x							--> x position
- int y							--> y position
note: Point bisa disebut koordinat

### RenderObj
- Bound bound					--> boundary of object
- Style style					--> style of object
- SDL_Texture* texture			--> texture of object
- SDL_renderer renderer			--> renderer pointer to reference renderer
- RenderObj* parent				--> parent of object
- vector<RenderObj*> Childs		--> childs of object
+ draw()						--> draw sesuai shape bentuknya texture biar bisa caching
+ setStyle()					--> editStyle + update texture
+ getStyle()					--> dapatkan info style
+ getRect()						--> dapatkan info rect
// blm ada ide lainya

note: RenderObj ini nanti nya bisa menjadi tipe data siap render, dan dapat di masukkan berbagai style mulai base Shape, color, align, dll

### Style
- Style is unordered_map		--> isi map untuk menggunakan style
note: Style ini digunakan untuk template bentuk obj mulai dari warna, bentuk dasar, round corner atau tidak, dll
note: format border = "none" atau border = "1px #000000 1%" --> borderline 1px, warna hitam, radius 1%
note: align berdasarkan bound dari parent renderObj = jadi parent.child tidak akan bisa diisi kecuali ada object yang mendaftarkan parentnya ke parent tersebut
note: tidak ada posisi absolute atau apa yang jadi konsepnya seperti div

### event
// blm ada gambaran ide