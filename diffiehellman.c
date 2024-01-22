// MIT License
//
// Copyright (c) 2024 gstaaij
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#include "nob.h"

typedef unsigned long long uintll_t;

uintll_t modpow(const uintll_t p, const uintll_t g, const uintll_t a) {
    uintll_t product = 1;
    for (uintll_t _ = 0; _ < a; ++_) {
        product = (product * g) % p;
    }
    return product;
}

char encodechar(const char chin, const int offset) {
    if (chin < 32 || chin > 126)
        return 0;
    
    int64_t newchar = chin - 32;
    
    newchar = (newchar + offset) % 95;

    char chout = (newchar + 32) & 0xff;

    return chout;
}

char decodechar(const char chin, const int offset) {
    if (chin < 32 || chin > 126)
        return 0;
    
    int64_t newchar = chin - 32;
    
    newchar = newchar - (offset % 95);
    while (newchar < 0) newchar += 95;

    char chout = (newchar + 32) & 0xff;

    return chout;
}

Nob_String_Builder encodestring(char* strin, const uintll_t secret) {
    Nob_String_Builder sb = {0};

    srand(secret);

    for (int i = 0; strin[i] != 0; ++i) {
        char encodedchar = encodechar(strin[i], rand());
        if (encodedchar != 0)
            nob_da_append(&sb, encodedchar);
    }

    return sb;
}

Nob_String_Builder decodestring(char* strin, const uintll_t secret) {
    Nob_String_Builder sb = {0};

    srand(secret);

    for (int i = 0; strin[i] != 0; ++i) {
        char decodedchar = decodechar(strin[i], rand());
        if (decodedchar != 0)
            nob_da_append(&sb, decodedchar);
    }

    return sb;
}

int main(void) {
    char pch[128] = {0};
    char gch[128] = {0};
    char ach[128] = {0};
    char Bch[128] = {0};

    printf("Please enter your prime mod number (p): ");
    fgets(pch, 128, stdin);

    printf("Please enter your base (g): ");
    fgets(gch, 128, stdin);

    printf("Please enter your secret exponent (a): ");
    fgets(ach, 128, stdin);

    uintll_t p = atoll(pch);
    uintll_t g = atoll(gch);
    uintll_t a = atoll(ach);

    uintll_t A = modpow(p, g, a);

    printf("\nYour public number A is %llu\n", A);

    printf("\nYou should've gotten a public number B from\nthe person you are trying to send messages to.\nPlease enter it now: ");
    fgets(Bch, 128, stdin);

    uintll_t B = atoll(Bch);

    uintll_t secret = modpow(p, B, a);

    // printf("\nThe secret number is %llu\n", secret);

    char strin[1024];
    char encodeordecode[16];
    for (;;) {
        memset(strin, 0, 1024);
        memset(encodeordecode, 0, 16);

        printf("\nDo you want to encode or decode? [E/d] ");
        fgets(encodeordecode, 16, stdin);

        printf("Please enter the message with at maximum 1023 characters: ");
        fgets(strin, 1024, stdin);

        if (tolower(encodeordecode[0]) == 'd') {
            Nob_String_Builder decodedstring = decodestring(strin, secret);
            nob_sb_append_null(&decodedstring);

            printf("Decoded string: \"%s\"\n", decodedstring.items);
        } else {
            Nob_String_Builder encodedstring = encodestring(strin, secret);
            nob_sb_append_null(&encodedstring);

            printf("Encoded string: \"%s\"\n", encodedstring.items);
        }
    }

    return 0;
}
