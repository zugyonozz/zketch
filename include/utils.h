// utils.h
#pragma once
#include "zenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fungsi untuk mengkonversi karakter hex ke nilai integer
int hexCharToInt(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1; // Karakter tidak valid
}

// Fungsi untuk mengkonversi string hex ke SDL_Color
Color Hex(const char* hex) {
    SDL_Color color = {0, 0, 0, 255}; // Default hitam dengan alpha 255 (opaque)
    if (hex == NULL) return color;
    if (hex[0] == '#') hex++;

    size_t len = strlen(hex);
    // Format yang didukung: RGB, RGBA, RRGGBB, RRGGBBAA
    if (len == 3) {
        // Format RGB
        color.r = hexCharToInt(hex[0]) * 17; // Duplikasi digit untuk mendapatkan nilai 0-255
        color.g = hexCharToInt(hex[1]) * 17;
        color.b = hexCharToInt(hex[2]) * 17;
    } else if (len == 4) {
        // Format RGBA
        color.r = hexCharToInt(hex[0]) * 17;
        color.g = hexCharToInt(hex[1]) * 17;
        color.b = hexCharToInt(hex[2]) * 17;
        color.a = hexCharToInt(hex[3]) * 17;
    } else if (len == 6) {
        // Format RRGGBB
        color.r = (hexCharToInt(hex[0]) << 4) + hexCharToInt(hex[1]);
        color.g = (hexCharToInt(hex[2]) << 4) + hexCharToInt(hex[3]);
        color.b = (hexCharToInt(hex[4]) << 4) + hexCharToInt(hex[5]);
    } else if (len == 8) {
        // Format RRGGBBAA
        color.r = (hexCharToInt(hex[0]) << 4) + hexCharToInt(hex[1]);
        color.g = (hexCharToInt(hex[2]) << 4) + hexCharToInt(hex[3]);
        color.b = (hexCharToInt(hex[4]) << 4) + hexCharToInt(hex[5]);
        color.a = (hexCharToInt(hex[6]) << 4) + hexCharToInt(hex[7]);
    }
    return color;
}