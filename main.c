#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>  // malloc etc.
#include <string.h>

#include "csav_dex.h"
#include "csav_items.h"
#include "csav_locations.h"
#include "csav_moves.h"
#include "csav_pokemon.h"
#include "csav_save.h"
#include "csav_text.h"

size_t SIZE_GAME_SAVE = 57344;


int main(void) {
    FILE *savData = fopen("/Users/josh/gba/pkmn_fr.sav", "r");

    if (savData == NULL) {
        printf("Error opening file\n");
    }
    printf("Save data loaded correctly...\n");

    SaveFile saveFile = {};
    const size_t read = fread(&saveFile, sizeof(SaveFile), 1, savData);
    if (read != 1) { printf("Error reading data\n"); fclose(savData); return 1; }

    SaveBlock *latestBlock = save_get_latest_block(&saveFile);
    Section *teamItemsSection = save_get_section_by_id(latestBlock, SAVE_SECTION_TEAM_ITEMS);
    save_print_section_addr(teamItemsSection, &saveFile);
    TeamItems *tiData = (TeamItems*)teamItemsSection->data;
    // // get my otid and name
    // // NOTE
    // //    from squirtle because reading an OTID from the trainer info does not include 2 out of 4 bytes (the secret ID)
    // //    the below code uses my trainer info section
    Section *trainerSection = save_get_section_by_id(latestBlock, SAVE_SECTION_TRAINER_INFO);
    TrainerInfo *tinfo = (TrainerInfo *)trainerSection->data;
    unsigned char *myOtname = tinfo->playerName;
    uint32_t myOtid = tiData->pokemon[0].otId;

    unsigned char pkmn_nick_encoded[10];
    const char pkmn_nick[10] = "BULBASAUR";
    text_encode(pkmn_nick, pkmn_nick_encoded, strlen(pkmn_nick), (size_t)10);

    // CREATE A POKEMON
    Pokemon bulbasaur = {};
    pkmn_init_new(
        &bulbasaur,
        PKMN_SPECIES_BULBASAUR,
        5,
        PKMN_NATURE_MODEST,
        ITEM_EXP_SHARE,
        PKMN_MOVE_SCRATCH,
        PKMN_MOVE_VINE_WHIP,
        PKMN_MOVE_EMPTY_MOVE,
        PKMN_MOVE_EMPTY_MOVE,
        // pp bonuses
        0,
        3,
        0,
        0,
        // EVs
        0, // hp
        10, //atk
        0, // def
        0, // spatk
        0, // spdef
        0, // speed
        // IVs
        31, // hp
        31, //atk
        31, // def
        31, // spatk
        31, // spdef
        31, // speed
        // contest stats
        0,
        0,
        0,
        0,
        0,
        // pokerus
        4,
        // friendship
        255,
        myOtid,
        myOtname,
        pkmn_nick_encoded,
        PKMN_LANG_EN,
        PKMN_LOCATION_PALLET_TOWN
    );

    tiData->pokemon[1] = bulbasaur;
    tiData->teamSize[0] = 2;

    const uint32_t newChecksum = save_checksum_calculate(teamItemsSection);
    teamItemsSection->checksum = newChecksum;

    FILE *dump = fopen("/Users/josh/gba/dump.sav", "w");
    if (dump == NULL) { return 1; }
    const size_t fw = fwrite(&saveFile, sizeof(SaveFile), 1, dump);
    if (fw == 0) { return 1; }
    fclose(dump);

    fclose(savData);

    // Try encoding some characters.
    // printf("Enter a nickname for the new Pokemon (max 10 chars):\n");
    // char name[11];
    // scanf("%10s", &name);
    // printf("You entered the name: %s\n", name);
    //
    // unsigned char newNick[11];
    // const size_t e = text_encode(name, newNick, sizeof(name), sizeof(newNick));
    // if (e == 0) { return 1; }
    //

    //
    // // Find the save block to modify
    // SaveBlock *latestBlock = save_get_latest_block(&saveFile);
    // Section *teamItemsSection = save_get_section_by_id(latestBlock, SAVE_SECTION_TEAM_ITEMS);
    // save_print_section_addr(teamItemsSection, &saveFile);
    // TeamItems *tiData = (TeamItems*)teamItemsSection->data;
    //
    // // get my otid and name
    // // NOTE
    // //    from squirtle because reading an OTID from the trainer info does not include 2 out of 4 bytes (the secret ID)
    // //    the below code uses my trainer info section
    // // Section *trainerSection = save_get_section_by_id(latestBlock, SAVE_SECTION_TRAINER_INFO);
    // // TrainerInfo *tinfo = (TrainerInfo *)trainerSection->data;
    // // uint32_t myOtid = tinfo->trainerId;
    // // unsigned char *myOtname = tinfo->playerName;
    // uint32_t myOtid = tiData->pokemon[0].otId;
    // unsigned char myOtname[7] = {};
    // memcpy(myOtname, tiData->pokemon[0].otName, 7);
    //
    // unsigned char newName[PKMN_NICKNAME_LEN] = { 0xFF };
    // snprintf(newName, PKMN_NICKNAME_LEN, "%s", newNick);
    //
    // /* CREATING A NEW POKEMON */
    // Pokemon poke = {};
    // pkmn_init_new(&poke, myOtid, myOtname, newName, PKMN_LANG_EN);
    // enum PkmnDataSubstructure newPokeOrder = pkmn_get_substructure_order(&poke);
    // printf("\n");
    //
    // FILE *dump = fopen("/Users/josh/gba/dump.sav", "w");
    // if (dump == NULL) { return 1; }
    // const size_t fw = fwrite(&saveFile, sizeof(SaveFile), 1, dump);
    // if (fw == 0) { return 1; }
    // fclose(dump);
    //
    // fclose(savData);
    return 0;
}
