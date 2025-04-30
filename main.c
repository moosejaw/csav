#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>  // malloc etc.
#include <string.h>

#include "csav_moves.h"
#include "csav_pokemon.h"
#include "csav_text.h"
#include "csav_save.h"

size_t SIZE_GAME_SAVE = 57344;

int main(void) {
    // Try encoding some characters.
    printf("Enter a nickname (max 10 chars):\n");
    char name[11];
    scanf("%10s", &name);
    printf("You entered the name: %s\n", name);

    unsigned char newNick[11];
    const size_t e = text_encode(name, newNick, sizeof(name), sizeof(newNick));
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
    save_print_section_addr(teamItemsSection, &saveFile);

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

    unsigned char revData[sizeof(PokemonDataType)] = {};
    memcpy(&revData, &squirtle->data, sizeof(PokemonDataType));

    // TODO use malloc in init func(?)
    PokemonDataType *pdt = (PokemonDataType*)malloc(sizeof(PokemonDataType));
    pkmn_init_inner_data(pdt, squirtle);

    const uint32_t decrKey = pkmn_get_inner_data_decryption_key(squirtle);
    PokemonDataType *decrypted = (PokemonDataType*)calloc(1, sizeof(PokemonDataType));
    pkmn_encrypt_decrypt_inner_data(pdt, decrypted, decrKey);

    const uint16_t orig_pkmn_checksum = pkmn_calculate_checksum(decrypted);
    printf("pkmn checksum (calc): %#X\n", orig_pkmn_checksum);

    // give squirtle a move
    decrypted->A.move4 = MOVE_SURF;
    decrypted->A.ppMove4 = 15;

    printf("pkmn checksum (orig): %#X\n", squirtle->checksum);
    const uint16_t pkmn_checksum = pkmn_calculate_checksum(decrypted);
    printf("pkmn checksum (new ): %#X\n", pkmn_checksum);
    squirtle->checksum = pkmn_checksum;

    // TODO is language getting mangled? (writing too many bytes from nickname?)
    squirtle->language = 0x2;

    PokemonDataType buf = {};
    pkmn_encrypt_decrypt_inner_data(decrypted, &buf, decrKey);
    pkmn_write_inner_data(squirtle, &buf);
    const uint32_t newChecksum = save_checksum_calculate(teamItemsSection);
    teamItemsSection->checksum = newChecksum;

    printf("\n");

    FILE *dump = fopen("/Users/josh/gba/dump.sav", "w");
    if (dump == NULL) { return 1; }
    const size_t fw = fwrite(&saveFile, sizeof(SaveFile), 1, dump);
    if (fw == 0) { return 1; }
    fclose(dump);

    fclose(savData);
    return 0;
}
