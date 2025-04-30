#include <stdio.h>
#include <string.h>

#include "csav_pokemon.h"
#include "csav_text.h"

/*
 * The substructure order, `o`, is determined by the Pokemon's personality value mod 24.
 *
 * The value of `o` corresponds to a particular order represented in the enum PkmnSubstructure.
 */
static unsigned
pkmn_get_substructure_order(const Pokemon *p) {
    return p->personalityValue % 24;
}

/*
 * The 'decryption key', which also happens to be the same key used to encrypt the data,
 * is the 32-bit 'personality value' mod 'OT ID'. Essentially, the unique value of the
 * mon mod the unique value of its original trainer.
 */
uint32_t
pkmn_get_inner_data_decryption_key(const Pokemon *p) {
    return p->personalityValue ^ p->otId;
};

/*
 * Initialise a `PokemonDataType`, `d`, according to the derived data substructure
 * order of Pokemon `p`.
 */
void
pkmn_init_inner_data(PokemonDataType *d, const Pokemon *p) {
    const enum PkmnDataSubstructure o = pkmn_get_substructure_order(p);
    switch (o) {
        case PKMN_DATA_ORDER_GAEM:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_GAME:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_GEAM:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_GEMA:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_GMAE:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_GMEA:
            memcpy(&d->G, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AGEM:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AGME:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AEGM:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AEMG:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AMGE:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_AMEG:
            memcpy(&d->A, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EGAM:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EGMA:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EAGM:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->M, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EAMG:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->M, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EMGA:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_EMAG:
            memcpy(&d->E, &p->data[0],  12);
            memcpy(&d->M, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MGAE:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MGEA:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->G, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MAGE:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->E, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MAEG:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->A, &p->data[12], 12);
            memcpy(&d->E, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MEGA:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->G, &p->data[24], 12);
            memcpy(&d->A, &p->data[36], 12);
            break;
        case PKMN_DATA_ORDER_MEAG:
            memcpy(&d->M, &p->data[0],  12);
            memcpy(&d->E, &p->data[12], 12);
            memcpy(&d->A, &p->data[24], 12);
            memcpy(&d->G, &p->data[36], 12);
            break;
        default:
            break;
    }
}

/*
 * Encrypt and/or decrypt a block of Pokemon data pointed to by `src`, using `key`.
 * If you pass encrypted data, it will be decrypted and vice versa.
 *
 * En/decryption is performed by taking 4 bytes of data at a time from the block,
 * and XORing those bytes against the key.
 *
 * TODO modify inplace?
 * TODO probably only decrypted data will be passed to this func as a PokemonDataType.
 *   while strictly it doesn't matter what type src and dest are (we can cast as needed),
 *   it makes more sense to use a generic type. PokemonDataBlock? (which is just an unsigned char[48]?)
 */
void
pkmn_encrypt_decrypt_inner_data(PokemonDataType *src, PokemonDataType *dst, const uint32_t key) {
    uint32_t *srcp = (uint32_t *)src;
    uint32_t *dstp = (uint32_t *)dst;
    for (int i=0; i < sizeof(PokemonDataType) / sizeof(uint32_t); i++) {
        uint32_t decr_word = *srcp ^ key;
        memcpy(dstp, &decr_word, sizeof(uint32_t));
        srcp++; dstp++;
    }
}

/*
 * Calculate the checksum for a Pokemon data block.
 * This just the sum of all 16 bit words in the data block.
 *
 * see: https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_(Generation_III)#Checksum
 */
uint16_t
pkmn_calculate_checksum(PokemonDataType *pd) {
    uint16_t calc = 0;
    uint16_t *ptr = (uint16_t *)pd;
    for (int i=0; i < sizeof(PokemonDataType) / sizeof(uint16_t); i++) {
        calc += ptr[i];
        fprintf(stderr, "cur: %#X / word: %#X / pos: %p\n", ptr[i], calc, ptr);
    }
    return calc;
}

/*
 * Write `pdt` to a `Pokemon` `p`.
 *
 * This function writes based on the mon's substructure order.
 */
void
pkmn_write_inner_data(const Pokemon *p, const PokemonDataType *pdt) {
    const enum PkmnDataSubstructure o = pkmn_get_substructure_order(p);
    switch (o) {
        case PKMN_DATA_ORDER_GAEM:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_GAME:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_GEAM:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_GEMA:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_GMAE:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_GMEA:
            memcpy(&p->data[0],  &pdt->G, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_AGEM:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_AGME:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_AEGM:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_AEMG:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_AMGE:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_AMEG:
            memcpy(&p->data[0],  &pdt->A, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_EGAM:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_EGMA:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_EAGM:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_EAMG:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->M, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_EMGA:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_EMAG:
            memcpy(&p->data[0],  &pdt->E, 12);
            memcpy(&p->data[12], &pdt->M, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_MGAE:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_MGEA:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->G, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_MAGE:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_MAEG:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->A, 12);
            memcpy(&p->data[24], &pdt->E, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_MEGA:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->G, 12);
            memcpy(&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_MEAG:
            memcpy(&p->data[0],  &pdt->M, 12);
            memcpy(&p->data[12], &pdt->E, 12);
            memcpy(&p->data[24], &pdt->A, 12);
            memcpy(&p->data[36], &pdt->G, 12);
            break;
        default:
            break;
    }
};
