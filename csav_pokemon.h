#ifndef PKMN_SUBSTRUCTS_H
#define PKMN_SUBSTRUCTS_H

#include <inttypes.h>

#define PKMN_NICKNAME_LEN 10
#define PKMN_OT_NAME_LEN 7

enum PkmnInitSettings {
    PKMN_INIT_RANDOM,
};

enum PkmnLanguage {
    PKMN_LANG_JP = 1,
    PKMN_LANG_EN = 2,
    PKMN_LANG_FR = 3,
    PKMN_LANG_IT = 4,
    PKMN_LANG_DE = 5,
    PKMN_LANG_ES = 7,
};

enum PkmnLevellingRate {
    PKMN_LEVELLING_RATE_MEDIUM_FAST,
    PKMN_LEVELLING_RATE_ERRATIC,
    PKMN_LEVELLING_RATE_FLUCTUATING,
    PKMN_LEVELLING_RATE_MEDIUM_SLOW,
    PKMN_LEVELLING_RATE_FAST,
    PKMN_LEVELLING_RATE_SLOW,
};

enum PkmnStat {
    PKMN_STAT_HP  ,
    PKMN_STAT_ATK ,
    PKMN_STAT_DEF ,
    PKMN_STAT_SPA ,
    PKMN_STAT_SPD ,
    PKMN_STAT_SPE ,
};

enum PkmnNature {
    PKMN_NATURE_ADAMANT = 3 ,
    PKMN_NATURE_BASHFUL = 18,
    PKMN_NATURE_BOLD    = 5 ,
    PKMN_NATURE_BRAVE   = 2 ,
    PKMN_NATURE_CALM    = 20,
    PKMN_NATURE_CAREFUL = 23,
    PKMN_NATURE_DOCILE  = 6 ,
    PKMN_NATURE_GENTLE  = 21,
    PKMN_NATURE_HARDY   = 0 ,
    PKMN_NATURE_HASTY   = 11,
    PKMN_NATURE_IMPISH  = 8 ,
    PKMN_NATURE_JOLLY   = 13,
    PKMN_NATURE_LAX     = 9 ,
    PKMN_NATURE_LONELY  = 1 ,
    PKMN_NATURE_MILD    = 16,
    PKMN_NATURE_MODEST  = 15,
    PKMN_NATURE_NAIVE   = 14,
    PKMN_NATURE_NAUGHTY = 4 ,
    PKMN_NATURE_QUIET   = 17,
    PKMN_NATURE_QUIRKY  = 24,
    PKMN_NATURE_RASH    = 19,
    PKMN_NATURE_RELAXED = 7 ,
    PKMN_NATURE_SASSY   = 22,
    PKMN_NATURE_SERIOUS = 12,
    PKMN_NATURE_TIMID   = 10,
};

enum PkmnAbility {
    PKMN_ABILITY_NO_ABILITY       , // 0
    PKMN_ABILITY_STENCH           , // 1
    PKMN_ABILITY_DRIZZLE          , // 2
    PKMN_ABILITY_SPEED_BOOST      , // 3
    PKMN_ABILITY_BATTLE_ARMOR     , // 4
    PKMN_ABILITY_STURDY           , // 5
    PKMN_ABILITY_DAMP             , // 6
    PKMN_ABILITY_LIMBER           , // 7
    PKMN_ABILITY_SAND_VEIL        , // 8
    PKMN_ABILITY_STATIC           , // 9
    PKMN_ABILITY_VOLT_ABSORB      , // 10
    PKMN_ABILITY_WATER_ABSORB     , // 11
    PKMN_ABILITY_OBLIVIOUS        , // 12
    PKMN_ABILITY_CLOUD_NINE       , // 13
    PKMN_ABILITY_COMPOUND_EYES    , // 14
    PKMN_ABILITY_INSOMNIA         , // 15
    PKMN_ABILITY_COLOR_CHANGE     , // 16
    PKMN_ABILITY_IMMUNITY         , // 17
    PKMN_ABILITY_FLASH_FIRE       , // 18
    PKMN_ABILITY_SHIELD_DUST      , // 19
    PKMN_ABILITY_OWN_TEMPO        , // 20
    PKMN_ABILITY_SUCTION_CUPS     , // 21
    PKMN_ABILITY_INTIMIDATE       , // 22
    PKMN_ABILITY_SHADOW_TAG       , // 23
    PKMN_ABILITY_ROUGH_SKIN       , // 24
    PKMN_ABILITY_WONDER_GUARD     , // 25
    PKMN_ABILITY_LEVITATE         , // 26
    PKMN_ABILITY_EFFECT_SPORE     , // 27
    PKMN_ABILITY_SYNCHRONIZE      , // 28
    PKMN_ABILITY_CLEAR_BODY       , // 29
    PKMN_ABILITY_NATURAL_CURE     , // 30
    PKMN_ABILITY_LIGHTNING_ROD    , // 31
    PKMN_ABILITY_SERENE_GRACE     , // 32
    PKMN_ABILITY_SWIFT_SWIM       , // 33
    PKMN_ABILITY_CHLOROPHYLL      , // 34
    PKMN_ABILITY_ILLUMINATE       , // 35
    PKMN_ABILITY_TRACE            , // 36
    PKMN_ABILITY_HUGE_POWER       , // 37
    PKMN_ABILITY_POISON_POINT     , // 38
    PKMN_ABILITY_INNER_FOCUS      , // 39
    PKMN_ABILITY_MAGMA_ARMOR      , // 40
    PKMN_ABILITY_WATER_VEIL       , // 41
    PKMN_ABILITY_MAGNET_PULL      , // 42
    PKMN_ABILITY_SOUNDPROOF       , // 43
    PKMN_ABILITY_RAIN_DISH        , // 44
    PKMN_ABILITY_SAND_STREAM      , // 45
    PKMN_ABILITY_PRESSURE         , // 46
    PKMN_ABILITY_THICK_FAT        , // 47
    PKMN_ABILITY_EARLY_BIRD       , // 48
    PKMN_ABILITY_FLAME_BODY       , // 49
    PKMN_ABILITY_RUN_AWAY         , // 50
    PKMN_ABILITY_KEEN_EYE         , // 51
    PKMN_ABILITY_HYPER_CUTTER     , // 52
    PKMN_ABILITY_PICKUP           , // 53
    PKMN_ABILITY_TRUANT           , // 54
    PKMN_ABILITY_HUSTLE           , // 55
    PKMN_ABILITY_CUTE_CHARM       , // 56
    PKMN_ABILITY_PLUS             , // 57
    PKMN_ABILITY_MINUS            , // 58
    PKMN_ABILITY_FORECAST         , // 59
    PKMN_ABILITY_STICKY_HOLD      , // 60
    PKMN_ABILITY_SHED_SKIN        , // 61
    PKMN_ABILITY_GUTS             , // 62
    PKMN_ABILITY_MARVEL_SCALE     , // 63
    PKMN_ABILITY_LIQUID_OOZE      , // 64
    PKMN_ABILITY_OVERGROW         , // 65
    PKMN_ABILITY_BLAZE            , // 66
    PKMN_ABILITY_TORRENT          , // 67
    PKMN_ABILITY_SWARM            , // 68
    PKMN_ABILITY_ROCK_HEAD        , // 69
    PKMN_ABILITY_DROUGHT          , // 70
    PKMN_ABILITY_ARENA_TRAP       , // 71
    PKMN_ABILITY_VITAL_SPIRIT     , // 72
    PKMN_ABILITY_WHITE_SMOKE      , // 73
    PKMN_ABILITY_PURE_POWER       , // 74
    PKMN_ABILITY_SHELL_ARMOR      , // 75
    PKMN_ABILITY_AIR_LOCK         , // 76
};

/*
 * isBadEgg   : mon is a BAD EGG. should never be set.
 * hasSpecies : should always be set.
 * useEggName : mon's nickname is "EGG". don't set.
 * blockBoxRS : mon can't be deposited into boxes in ruby/sapphire? don't set.
 *
 * see: https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_structure_(Generation_III)#Misc._Flags
 *
 * TODO these are bits, not bytes.
 */
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
    unsigned char    nickname[PKMN_NICKNAME_LEN];
    uint8_t          language;
    unsigned char    miscFlags;
    unsigned char    otName[PKMN_OT_NAME_LEN];
    uint8_t          markings;
    uint16_t         checksum;
    uint16_t         _unknown;
    PokemonDataBlock data;

    uint32_t         statusCondition;
    uint8_t          level;
    uint8_t          mailId;
    uint16_t         currentHp;
    uint16_t         totalHp;
    uint16_t         attackStat;
    uint16_t         defenseStat;
    uint16_t         speedStat;
    uint16_t         specialAttackStat;
    uint16_t         specialDefenseStat;
} Pokemon;


typedef struct {
    uint16_t species;
    uint16_t heldItem;
    uint32_t exp;
    uint8_t  ppBonuses;
    uint8_t  friendship;
    uint16_t _unused;
} Growth;

typedef struct {
    uint16_t move1;
    uint16_t move2;
    uint16_t move3;
    uint16_t move4;
    uint8_t  ppMove1;
    uint8_t  ppMove2;
    uint8_t  ppMove3;
    uint8_t  ppMove4;
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

typedef struct {
    enum PkmnAbility a;
    enum PkmnAbility b;
} AbilityPair;

unsigned short pkmn_init_evs(
    EvsCondition *data,
    unsigned short hpEv,
    unsigned short atkEv,
    unsigned short defEv,
    unsigned short spAtkEv,
    unsigned short spDefEv,
    unsigned short speedEv
);
enum PkmnLevellingRate pkmn_get_levelling_rate(enum Pokedex species);
uint32_t pkmn_calculate_exp(enum Pokedex species, unsigned short level);
uint8_t pkmn_calculate_pp_bonuses(unsigned short move1bonuses, unsigned short move2bonuses, unsigned short move3bonuses, unsigned short move4bonuses);
void pkmn_init_abilities(enum Pokedex species, AbilityPair *abi);
uint8_t pkmn_get_base_stat_value(enum Pokedex species, enum PkmnStat stat);
unsigned pkmn_get_substructure_order(const Pokemon *p);
uint16_t pkmn_calculate_stat(enum PkmnStat stat, enum Pokedex species, uint8_t ev, uint8_t iv, uint8_t level, enum PkmnNature nature);


void
pkmn_init_new(
    Pokemon *p,
    const enum Pokedex species,
    short level,
    const enum PkmnNature nature,
    const enum Items heldItem,
    const enum Moves move1,
    const enum Moves move2,
    const enum Moves move3,
    const enum Moves move4,
    const unsigned short move1bonuses,
    const unsigned short move2bonuses,
    const unsigned short move3bonuses,
    const unsigned short move4bonuses,
    const unsigned short hpEv,
    const unsigned short atkEv,
    const unsigned short defEv,
    const unsigned short spAtkEv,
    const unsigned short spDefEv,
    const unsigned short speedEv,
    unsigned short hpIv,
    unsigned short atkIv,
    unsigned short defIv,
    unsigned short spAtkIv,
    unsigned short spDefIv,
    unsigned short speedIv,
    const unsigned char contestBeauty,
    const unsigned char contestCoolness,
    const unsigned char contestCuteness,
    const unsigned char contestSmartness,
    const unsigned char contestToughness,
    unsigned short pokerusDays,
    const unsigned short friendship,
    const uint32_t otId,
    const unsigned char *otName,
    const unsigned char *nickname,
    const enum PkmnLanguage lang,
    const enum PkmnLocations metLocation
);


uint32_t pkmn_get_inner_data_decryption_key(const Pokemon *p);
void pkmn_init_inner_data(PokemonDataType *d, const Pokemon *p);
double pkmn_calculate_nature_multiplier(enum PkmnNature nature, enum PkmnStat stat);
void pkmn_encrypt_decrypt_inner_data(PokemonDataType *src, PokemonDataType *dst, uint32_t key);
void pkmn_write_inner_data(const Pokemon *p, const PokemonDataType *pdt);
uint16_t pkmn_calculate_checksum(PokemonDataType *pd);

enum PkmnDataSubstructure {
    PKMN_DATA_ORDER_GAEM,  //  0
    PKMN_DATA_ORDER_GAME,  //  1
    PKMN_DATA_ORDER_GEAM,  //  2
    PKMN_DATA_ORDER_GEMA,  //  3
    PKMN_DATA_ORDER_GMAE,  //  4
    PKMN_DATA_ORDER_GMEA,  //  5
    PKMN_DATA_ORDER_AGEM,  //  6
    PKMN_DATA_ORDER_AGME,  //  7
    PKMN_DATA_ORDER_AEGM,  //  8
    PKMN_DATA_ORDER_AEMG,  //  9
    PKMN_DATA_ORDER_AMGE,  // 10
    PKMN_DATA_ORDER_AMEG,  // 11
    PKMN_DATA_ORDER_EGAM,  // 12
    PKMN_DATA_ORDER_EGMA,  // 13
    PKMN_DATA_ORDER_EAGM,  // 14
    PKMN_DATA_ORDER_EAMG,  // 15
    PKMN_DATA_ORDER_EMGA,  // 16
    PKMN_DATA_ORDER_EMAG,  // 17
    PKMN_DATA_ORDER_MGAE,  // 18
    PKMN_DATA_ORDER_MGEA,  // 19
    PKMN_DATA_ORDER_MAGE,  // 20
    PKMN_DATA_ORDER_MAEG,  // 21
    PKMN_DATA_ORDER_MEGA,  // 22
    PKMN_DATA_ORDER_MEAG,  // 23
};

#endif //PKMN_SUBSTRUCTS_H
