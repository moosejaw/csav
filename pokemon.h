#ifndef PKMN_SUBSTRUCTS_H
#define PKMN_SUBSTRUCTS_H
#include <inttypes.h>



/// POKEMON OUTER STRUCTURE
typedef struct {
    unsigned char isBadEgg;
    unsigned char hasSpecies;
    unsigned char useEggName;
    unsigned char noRubySapphireBox;
    unsigned char _unused[4];
} PokemonMiscFlags;

typedef unsigned char PokemonDataBlock[48];

typedef struct {
    uint32_t         personalityValue;
    uint32_t         otId;
    unsigned char    nickname[10];
    uint8_t          language; // TODO ensure has_species bit is set in here when adding new pokemon.
    unsigned char    miscFlags;
    unsigned char    otName[7];
    uint8_t          markings;
    uint16_t         checksum;
    uint16_t         _unknown;
    PokemonDataBlock data;

    // Part of the full 100-byte structure of a Pokemon, but not used
    // when in a save file:
    //
    // uint32_t statusCondition;
    // uint8_t level;
    // uint8_t mailId;
    // uint16_t currentHp;
    // uint16_t totalHp;
    // uint16_t attackStat;
    // uint16_t defenseStat;
    // uint16_t speedStat;
    // uint16_t specialAttackStat;
    // uint16_t specialDefenseStat;
} Pokemon;
/// /POKEMON OUTER STRUCTURE

unsigned pkmn_get_substructure_order(const Pokemon *p);


/// DATA SUBSTRUCTURE ELEMENT DEFINITIONS
typedef struct {
    uint16_t species;
    uint16_t heldItem;
    uint32_t exp;
    uint8_t ppBonuses;
    uint8_t friendship;
    uint16_t _unused;
} Growth;

typedef struct {
    uint16_t move1;
    uint16_t move2;
    uint16_t move3;
    uint16_t move4;
    uint8_t ppMove1;
    uint8_t ppMove2;
    uint8_t ppMove3;
    uint8_t ppMove4;
} Attacks;

typedef struct {
    uint8_t hpEv;
    uint8_t atkEv;
    uint8_t defEv;
    uint8_t speEv;
    uint8_t spaEv;
    uint8_t spdEv;
    uint8_t coolness;
    uint8_t beauty;
    uint8_t cuteness;
    uint8_t smartness;
    uint8_t toughness;
    uint8_t feel;
} EvsCondition;

typedef struct {
    uint8_t  pokerus;
    uint8_t  metWhere;
    uint16_t originInfo;
    uint32_t ivEggAbility;
    uint32_t ribbonsObedience;
} Misc;


typedef struct {
    Growth       G;
    Attacks      A;
    Misc         M;
    EvsCondition E;
} PokemonDataType;

uint32_t pkmn_get_inner_data_decryption_key(const Pokemon *p);

void pkmn_init_inner_data(PokemonDataType *d, const Pokemon *p);
void pkmn_encrypt_decrypt_inner_data(PokemonDataType *src, PokemonDataType *dst, const uint32_t key);
void pkmn_write_inner_data(const Pokemon *p, const PokemonDataType *pdt);
uint16_t pkmn_calculate_checksum(PokemonDataType *pd);

enum PkmnDataSubstructure {
    GAEM,
    GAME,
    GEAM,
    GEMA,
    GMAE,
    GMEA,
    AGEM,
    AGME,
    AEGM,
    AEMG,
    AMGE,
    AMEG,
    EGAM,
    EGMA,
    EAGM,
    EAMG,
    EMGA,
    EMAG,
    MGAE,
    MGEA,
    MAGE,
    MAEG,
    MEGA,
    MEAG,
};
#endif //PKMN_SUBSTRUCTS_H
