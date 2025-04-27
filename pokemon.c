#include <stdio.h>
#include <string.h>

#include "pokemon.h"

#include "util.h"

/// The substructure order, o, is determined by the Pokemon's personality
/// value mod 24.
/// The value of o corresponds to a particular order represented in the enum
/// PkmnSubstructure.
unsigned pkmn_get_substructure_order(const Pokemon *p) {
    uint32_t pval = p->personalityValue;
    return pval % 24;
}

uint32_t pkmn_get_inner_data_decryption_key(const Pokemon *p) {
    return p->personalityValue ^ p->otId;
};

void pkmn_init_12byte_section(unsigned char *psrc, unsigned char secDst[12]) {
    // TODO this func isn't necessary as we don't need to reverse bytes anymore(?)
    // Initialises a 12-byte section by reversing each 4-byte word
    // in the starting address `psrc` and writing the output words
    // to `secDst` contiguously.
    uint32_t wordbuf = 0;
    uint32_t *word = (uint32_t *)psrc;
    for (int i = 0; i < 12; i+= 4) {
        memcpy(&wordbuf, word, sizeof(uint32_t));
        memcpy(&secDst[i], &wordbuf, sizeof(uint32_t));
        word++;
    }
}

void pkmn_init_inner_data(PokemonDataType *d, const Pokemon *p) {
    const enum PkmnDataSubstructure o = pkmn_get_substructure_order(p);
    switch (o) {
        // TODO all cases
        case GAME:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case EAMG:
            // TODO cleanup
            pkmn_init_12byte_section((unsigned char *)&p->data[0],  (unsigned char *)&d->E);
            pkmn_init_12byte_section((unsigned char *)&p->data[12], (unsigned char *)&d->A);
            pkmn_init_12byte_section((unsigned char *)&p->data[24], (unsigned char *)&d->M);
            pkmn_init_12byte_section((unsigned char *)&p->data[36], (unsigned char *)&d->G);
            break;
        default:
            memset(d, 0xAE, sizeof(PokemonDataType));
            break;
    }
}

// TODO modify inplace?
void pkmn_encrypt_decrypt_inner_data(PokemonDataType *src, PokemonDataType *dst, const uint32_t key) {
    // XOR 4 bytes at a time.
    uint32_t *srcp = (uint32_t *)src;
    uint32_t *dstp = (uint32_t *)dst;
    for (int i=0; i < sizeof(PokemonDataType) / sizeof(uint32_t); i++) {
        uint32_t decr_word = *srcp ^ key;
        memcpy(dstp, &decr_word, sizeof(uint32_t));
        srcp++; dstp++;
    }
}

uint16_t pkmn_calculate_checksum(PokemonDataType *pd) {
    uint16_t calc = 0;
    uint16_t *ptr = (uint16_t *)pd;
    for (int i=0; i < sizeof(PokemonDataType) / sizeof(uint16_t); i++) {
        calc += ptr[i];
        printf("cur: %#X / word: %#X / pos: %p\n", ptr[i], calc, ptr);
    }
    return calc;
}

/// This function writes the inner data block of a Pokemon
void pkmn_write_inner_data(const Pokemon *p, const PokemonDataType *pdt) {
    const enum PkmnDataSubstructure o = pkmn_get_substructure_order(p);
    // TODO all cases
    switch (o) {
        case EAMG:
            memcpy(&p->data[0] , &pdt->E,  12);
            memcpy(&p->data[12], &pdt->A,  12);
            memcpy(&p->data[24], &pdt->M,  12);
            memcpy(&p->data[36], &pdt->G,  12);
            break;
        default:
            break;
    }
};
