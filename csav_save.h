
#ifndef CSAV_SAVE_H
#define CSAV_SAVE_H

#include <inttypes.h>
#include <stdlib.h>    // size_t

#include "csav_pokemon.h"


typedef unsigned char Data[3968];
typedef uint16_t SectionId;
typedef uint16_t Checksum;
typedef uint32_t Signature;
typedef uint32_t SaveIndex;

typedef struct {
    Data          data;
    unsigned char _padding[116];
    SectionId     sectionId;
    Checksum      checksum;
    Signature     signature;
    SaveIndex     saveIndex;
} Section;

typedef Section SaveBlock[14];

typedef struct {
    SaveBlock     saveBlocks[2];
    unsigned char hallOfFame[8192];
    unsigned char mysteryGift[4096];
    unsigned char recordedBattle[4096];
} SaveFile;

typedef struct {
    unsigned char playerName[8];
    unsigned char playerGender;
    unsigned char _unused;
    unsigned char trainerId[4];
    unsigned char options[3];
    unsigned char gameCode[4];
    unsigned char securityKey[4];
} TrainerInfo;

typedef struct {
    unsigned char _unused[52];
    unsigned char teamSize[4];
    Pokemon       pokemon[6];
    unsigned char money[4];
    unsigned char coins[2];
    unsigned char pcItems[120];
    unsigned char itemPocket[168];
    unsigned char keyItemPocket[120];
    unsigned char ballItemPocket[52];
    unsigned char tmCase[232];
    unsigned char berryPocket[172];
} TeamItems;

SaveBlock *save_get_latest_block(const SaveFile *save);
Section *save_get_section_by_id(SaveBlock *block, unsigned id);
void save_print_section_addr(const Section *s, SaveFile *sf);
uint16_t  save_checksum_calculate(const Section *s);



#endif //CSAV_SAVE_H
