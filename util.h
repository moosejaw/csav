//
// Created by josh on 21/04/2025.
//

#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>

short text_encode(const char *is, unsigned char *os, const size_t ilen, const size_t olen);
void bytes_reverse(unsigned char *src, const unsigned len);

#endif //UTIL_H
