#include <stdio.h>
#include <string.h>

#include "csav_save.h"
#include "csav_pokemon.h"

/*
 * Based on input SaveFile `save`, return a pointer to the SaveBlock that
 * was most recently saved by the player.
 *
 * We do this by checking the "save index" value of the first section of each save block,
 * returning the corresponding save block to the largest save index value.
 */
SaveBlock
*save_get_latest_block(const SaveFile *save) {
    const SaveIndex a = save->saveBlocks[0][0].saveIndex, b = save->saveBlocks[1][0].saveIndex;
    return a > b ? (SaveBlock*)save->saveBlocks[0] : (SaveBlock*)save->saveBlocks[1];
}

/*
 * Return a pointer to a Section in `block` with the specified `id`.
 */
Section
*save_get_section_by_id(SaveBlock *block, const enum SaveSectionId id) {
    Section *section = block[0];
    for (size_t i = 0; i < 14; i++) {
        if (section->sectionId == id) return section;
        section++;
    }
    return NULL;
}

/*
 * Prints the starting address of the Section pointed to by `s` in memory,
 * relative to the starting address of the entire save file denoted by `sf`.
 *
 * The purpose of this is to determine an offset within the context of the
 * save file itself.
 */
void
save_print_section_addr(const Section *s, SaveFile *sf) {
    const unsigned long addr = (unsigned long)s - (unsigned long)sf;
    printf("Address of section : %#lx\n", addr);
}

/*
 * Calculate and return a checksum value for the Section pointed to by `s`.
 *
 * The number of bytes we use to calculate the checksum is fixed based on
 * the Section's ID.
 *
 * see: https://bulbapedia.bulbagarden.net/wiki/Save_data_structure_(Generation_III)#Checksum
 */
uint16_t
save_checksum_calculate(const Section *s) {
    // Determine how many bytes to process
    unsigned numValidateBytes;
    switch (s->sectionId) {
        case 0:
            numValidateBytes = 3884; break;
        case 4:
            numValidateBytes = 3848; break;
        case 13:
            numValidateBytes = 2000; break;
        default:
            numValidateBytes = 3968; break;
    }
    printf("(checksum) section id: %u no. validate bytes: %u\n", s->sectionId, numValidateBytes);

    // Calculate the checksum value
    uint32_t checksum = 0;
    for (int i = 0; i <= numValidateBytes; i+=4) {
        const uint32_t word;
        memcpy((uint32_t *)&word, s->data + i, 4);
        checksum += word;
    }
    const uint16_t checksum_upper = (checksum & (0xFFFF) << (16)) >> 16;
    const uint16_t checksum_lower = (checksum) & 0xFFFF;
    printf("(checksum) intermed. upper: %#x , intermed. lower: %#x\n", checksum_upper, checksum_lower);

    checksum = checksum_upper + checksum_lower;
    printf("(checksum) original: %#x // calculated: %#x\n", s->checksum, checksum);
    return checksum;
}
