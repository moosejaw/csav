//
// Created by josh on 21/04/2025.
//

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

short text_encode(const char *is, unsigned char *os, const size_t ilen, const size_t olen) {
    // Encode the input string `is` of length `ilen` using the proprietary encoding,
    // storing the result in `os`, where `olen` is the length of the output string.
    // If a string terminator character is encountered before the encoder has encoded
    // `ilen` characters, the encoder stops, and all remaining space in `os`
    //  is filled with `0xFF`.
    //
    // Any unfilled characters in `os` are replaced with `0xFF`.
    // Any characters that are not recognised by the encoder are converted to `0x00`.
    //
    // If `ilen` > `olen`, `0` is returned.
    //
    // TODO find a way of dealing with special characters, i.e,
    // case "♂":
    //     encoded_char = 0xB5;
    //     break;
    // case '♀':
    //     encoded_char = 0xB6;
    //     break;
    // case '‥':
    //     encoded_char = 0xB0;
    //     break;
    // case '“':
    //     encoded_char = 0xB1;
    //     break;
    // case '”':
    //     encoded_char = 0xB2;
    //     break;
    // case '‘':
    //     encoded_char = 0xB3;
    //     break;
    if (ilen > olen) { return 0; }

    memset(os, 0xFF, olen);

    short ecount = 0;
    for (size_t i = 0; i < ilen; i++) {
        if (is[i] == '\0') break;
        unsigned char encoded_char;
        switch (is[i]) {
            case '0':
                encoded_char = 0xA1;
                break;
            case '1':
                encoded_char = 0xA2;
                break;
            case '2':
                encoded_char = 0xA3;
                break;
            case '3':
                encoded_char = 0xA4;
                break;
            case '4':
                encoded_char = 0xA5;
                break;
            case '5':
                encoded_char = 0xA6;
                break;
            case '6':
                encoded_char = 0xA7;
                break;
            case '7':
                encoded_char = 0xA8;
                break;
            case '8':
                encoded_char = 0xA9;
                break;
            case '9':
                encoded_char = 0xAA;
                break;
            case '!':
                encoded_char = 0xAB;
                break;
            case '?':
                encoded_char = 0xAC;
                break;
            case '.':
                encoded_char = 0xAD;
                break;
            case '-':
                encoded_char = 0xAE;
                break;
            case '\'':
                encoded_char = 0xB4;
                break;
            case ',':
                encoded_char = 0xB8;
                break;
            case '/':
                encoded_char = 0xBA;
                break;
            case 'A':
                encoded_char = 0xBB;
                break;
            case 'B':
                encoded_char = 0xBC;
                break;
            case 'C':
                encoded_char = 0xBD;
                break;
            case 'D':
                encoded_char = 0xBE;
                break;
            case 'E':
                encoded_char = 0xBF;
                break;
            case 'F':
                encoded_char = 0xC0;
                break;
            case 'G':
                encoded_char = 0xC1;
                break;
            case 'H':
                encoded_char = 0xC2;
                break;
            case 'I':
                encoded_char = 0xC3;
                break;
            case 'J':
                encoded_char = 0xC4;
                break;
            case 'K':
                encoded_char = 0xC5;
                break;
            case 'L':
                encoded_char = 0xC6;
                break;
            case 'M':
                encoded_char = 0xC7;
                break;
            case 'N':
                encoded_char = 0xC8;
                break;
            case 'O':
                encoded_char = 0xC9;
                break;
            case 'P':
                encoded_char = 0xCA;
                break;
            case 'Q':
                encoded_char = 0xCB;
                break;
            case 'R':
                encoded_char = 0xCC;
                break;
            case 'S':
                encoded_char = 0xCD;
                break;
            case 'T':
                encoded_char = 0xCE;
                break;
            case 'U':
                encoded_char = 0xCF;
                break;
            case 'V':
                encoded_char = 0xD0;
                break;
            case 'W':
                encoded_char = 0xD1;
                break;
            case 'X':
                encoded_char = 0xD2;
                break;
            case 'Y':
                encoded_char = 0xD3;
                break;
            case 'Z':
                encoded_char = 0xD4;
                break;
            case 'a':
                encoded_char = 0xD5;
                break;
            case 'b':
                encoded_char = 0xD6;
                break;
            case 'c':
                encoded_char = 0xD7;
                break;
            case 'd':
                encoded_char = 0xD8;
                break;
            case 'e':
                encoded_char = 0xD9;
                break;
            case 'f':
                encoded_char = 0xDA;
                break;
            case 'g':
                encoded_char = 0xDB;
                break;
            case 'h':
                encoded_char = 0xDC;
                break;
            case 'i':
                encoded_char = 0xDD;
                break;
            case 'j':
                encoded_char = 0xDE;
                break;
            case 'k':
                encoded_char = 0xDF;
                break;
            case 'l':
                encoded_char = 0xE0;
                break;
            case 'm':
                encoded_char = 0xE1;
                break;
            case 'n':
                encoded_char = 0xE2;
                break;
            case 'o':
                encoded_char = 0xE3;
                break;
            case 'p':
                encoded_char = 0xE4;
                break;
            case 'q':
                encoded_char = 0xE5;
                break;
            case 'r':
                encoded_char = 0xE6;
                break;
            case 's':
                encoded_char = 0xE7;
                break;
            case 't':
                encoded_char = 0xE8;
                break;
            case 'u':
                encoded_char = 0xE9;
                break;
            case 'v':
                encoded_char = 0xEA;
                break;
            case 'w':
                encoded_char = 0xEB;
                break;
            case 'x':
                encoded_char = 0xEC;
                break;
            case 'y':
                encoded_char = 0xED;
                break;
            case 'z':
                encoded_char = 0xEE;
                break;
            default:
                printf("warning! unrecognised input character %c. it was replaced with 0x00.\n", is[i]);
                encoded_char = 0x00;
                break;
        }
        if (encoded_char != 0xFF) ecount += 1;

        os[i] = encoded_char;
        printf("(encode) idx: %lu, in: %c out: %hhX\n", i, is[i], os[i]);
    }
    return ecount;
}

void bytes_reverse(unsigned char *src, const unsigned len) {
    // Reverses the byte order of `src` by swapping `len` bytes.
    for (int i = 0; i < len / 2; i++) {
        const unsigned char buf = src[i];
        src[i] = src[len - 1 - i];
        src[len - 1 - i] = buf;
    }
}