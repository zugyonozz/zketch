#ifndef ZKETCH_H // zketch.h
#define ZKETCH_H

// Include semua header yang diperlukan
#include "style.h"
#include "element.h"
#include "ui_manager.h"

// Namespace untuk menghindari konflik
namespace zketch {
    // Versi library
    extern const char* VERSION;
    
    // Fungsi inisialisasi global
    bool initialize();
    
    // Fungsi cleanup global
    void cleanup();
}

#endif // ZKETCH_H