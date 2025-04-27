#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "moves.h"
#include "pokemon.h"
#include "util.h"

size_t SIZE_GAME_SAVE = 57344;

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

SaveBlock *save_get_latest_block(const SaveFile *save) {
    // Based on input SaveFile `save`, this function returns a pointer to the SaveBlock that
    // was most recently saved by the player.
    //
    // It does this by checking the save index value of the first section of each save block,
    // returning the corresponding save block to the largest save index value.
    const SaveIndex a = save->saveBlocks[0][0].saveIndex, b = save->saveBlocks[1][0].saveIndex;
    return a > b ? (SaveBlock*)save->saveBlocks[0] : (SaveBlock*)save->saveBlocks[1];
}

Section *save_get_section_by_id(SaveBlock *block, const unsigned id) {
    // Returns a pointer to a game save Section, within the given SaveBlock `block`,
    // based on input ID `id`.
    Section *section = block[0];
    for (size_t i = 0; i < 14; i++) {
        if (section->sectionId == id) return section;
        section++;
    }
    return NULL;
}

void print_section_addr(const Section *s, SaveFile *sf) {
    // Prints the starting address of the given Section `*s` in memory, relative to the
    // starting address of the entire save file denoted by `*sf`.
    const unsigned long addr = (unsigned long)s - (unsigned long)sf;
    printf("Address of section : %#lx\n", addr);
}

uint16_t save_checksum_calculate(const Section *s) {
    // Calculate and return a checksum value for the input section pointed to by `s`.

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
        memcpy(&word, s->data + i, 4);
        checksum += word;
    }
    const uint16_t checksum_upper = (checksum & (0xFFFF) << (16)) >> 16;
    const uint16_t checksum_lower = (checksum) & 0xFFFF;
    printf("(checksum) intermed. upper: %#x , intermed. lower: %#x\n", checksum_upper, checksum_lower);

    checksum = checksum_upper + checksum_lower;
    printf("(checksum) original: %#x // calculated: %#x\n", s->checksum, checksum);
    return checksum;
}

int main(void) {
    // Try encoding some characters.
    printf("Enter a nickname (max 10 chars):\n");
    char name[11];
    scanf("%10s", &name);
    printf("You entered the name: %s\n", name);

    unsigned char newNick[11];
    const short e = text_encode(name, newNick, sizeof(name), sizeof(newNick));
    if (e == 0) { return 1; }

    FILE *savData = fopen("/Users/josh/gba/pkmn_fr.sav", "r");

    if (savData == NULL) {
        printf("Error opening file\n");
    }
    printf("Save data loaded correctly...\n");

    SaveFile saveFile = {};
    const size_t read = fread(&saveFile, sizeof(SaveFile), 1, savData);
    if (read != 1) { printf("Error reading data\n"); fclose(savData); return 1; }

    /*
     *  Get latest save block
     *  Get section with id 0
     *  Print section id and save index
     */
    SaveBlock *latestBlock = save_get_latest_block(&saveFile);
    Section *teamItemsSection = save_get_section_by_id(latestBlock, 1);
    print_section_addr(teamItemsSection, &saveFile);

    TeamItems *tiData = (TeamItems*)teamItemsSection->data;
    unsigned char *ptr = tiData->pokemon[0].nickname;
    printf("Original nickname bytes: ");
    for (int i=0; i < 10; i++) {
        printf("%X ", *ptr);
        ptr += 1;
    }
    printf("\n");

    memcpy(tiData->pokemon[0].nickname, &newNick, 11);

    Pokemon *squirtle = &tiData->pokemon[0];
    const enum PkmnDataSubstructure s = pkmn_get_substructure_order(squirtle);
    printf("Pokemon's personality value mod 24: %u\n", s);


    unsigned char revData[sizeof(PokemonDataType)] = {};
    memcpy(&revData, &squirtle->data, sizeof(PokemonDataType));

    // TODO use malloc in init func(?)
    PokemonDataType *pdt = (PokemonDataType*)malloc(sizeof(PokemonDataType));
    pkmn_init_inner_data(pdt, squirtle);

    const uint32_t decrKey = pkmn_get_inner_data_decryption_key(squirtle);
    PokemonDataType *decrypted = (PokemonDataType*)calloc(1, sizeof(PokemonDataType));
    pkmn_encrypt_decrypt_inner_data(pdt, decrypted, decrKey);

    // give squirtle a move
    decrypted->A.move4 = SURF;
    decrypted->A.ppMove4 = 15;

    printf("pkmn checksum (orig): %#X\n", squirtle->checksum);
    const uint16_t pkmn_checksum = pkmn_calculate_checksum(decrypted);
    printf("pkmn checksum (new ): %#X\n", pkmn_checksum);

    pkmn_encrypt_decrypt_inner_data(decrypted, (PokemonDataType*)&squirtle->data, decrKey);
    const uint32_t newChecksum = save_checksum_calculate(teamItemsSection);
    teamItemsSection->checksum = newChecksum;

    printf("\n");

    // unsigned char nicknamebuf[10];
    // memcpy(&nicknamebuf, tiData->pokemon[0].nickname, 10);
    // printf("Nickname bytes: ");
    // for (int i=0; i < 10; i++) {
    //     printf("%#X ", nicknamebuf[i]);
    // }

    // // Section *section = save_get_section_by_id(latestBlock, 0);
    // // print_section_addr(section, &saveFile);
    // // TrainerInfo *tinfo = (TrainerInfo*)section->data;
    // // tinfo->playerGender = 0x1;
    // //
    // // memcpy(tinfo->playerName, encoded, sizeof(encoded));
    // //
    // // const uint32_t newChecksum = checksum_calculate(section, latestBlock);
    // // section->checksum = newChecksum;
    //
    FILE *dump = fopen("/Users/josh/gba/dump.sav", "w");
    if (dump == NULL) { return 1; }
    const size_t fw = fwrite(&saveFile, sizeof(SaveFile), 1, dump);
    if (fw == 0) { return 1; }
    fclose(dump);

    fclose(savData);
    return 0;
}
