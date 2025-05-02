#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csav_dex.h"
#include "csav_items.h"
#include "csav_locations.h"
#include "csav_moves.h"
#include "csav_pokemon.h"

static unsigned char RANDOM_INITIALISED = 0;

/*
 * Initialise the EV data of a Pokemon, based on the input values for the EVs.
 * You cannot assign a single EV value greater than 255, and your total EV values
 * cannot sum to more than 510.
 *
 * If the sum of your EVs > 510, then "255" is returned.
 * 
 * For each requested EV value greater than 255, the underlying EV value is set to 0,
 * and a value > 0 will be returned, where the number represents each invalid EV
 * value provided.
 *
 * If all EVs are assigned without issue, 0 is returned.
 */
unsigned short
pkmn_init_evs(
    EvsCondition *data,
    const unsigned short hpEv,
    const unsigned short atkEv,
    const unsigned short defEv,
    const unsigned short spAtkEv,
    const unsigned short spDefEv,
    const unsigned short speedEv
) {
    unsigned short returnValue = 0;

    if ((hpEv + atkEv + defEv + spAtkEv + spDefEv + speedEv) > 510) {
        return 255;     
    }
    
    if (hpEv > 255) {
        data->hpEv = 0;
        returnValue++;
    } else {
        data->hpEv = hpEv;
    }
    if (atkEv > 255) {
        data->atkEv = 0;
        returnValue++;
    } else {
        data->atkEv = atkEv;
    }
    if (defEv > 255) {
        data->defEv = 0;
        returnValue++;
    } else {
        data->defEv = defEv;
    }
    if (spAtkEv > 255) {
        data->spaEv = 0;
        returnValue++;
    } else {
        data->spaEv = spAtkEv;
    }
    if (spDefEv > 255) {
        data->spdEv = 0;
        returnValue++;
    } else {
        data->spdEv = spDefEv;
    }
    if (speedEv > 255) {
        data->speEv = 0;
        returnValue++;
    } else {
        data->speEv = speedEv;
    }

    return returnValue;
}

enum PkmnLevellingRate pkmn_get_levelling_rate(enum Pokedex species) {
    enum PkmnLevellingRate rate = 0;
    switch (species) {

        // LEVELLING_RATE_ERRATIC
        case PKMN_SPECIES_ALTARIA         :
        case PKMN_SPECIES_ANORITH         :
        case PKMN_SPECIES_ARMALDO         :
        case PKMN_SPECIES_CLAMPERL        :
        case PKMN_SPECIES_CRADILY         :
        case PKMN_SPECIES_FEEBAS          :
        case PKMN_SPECIES_GOREBYSS        :
        case PKMN_SPECIES_HUNTAIL         :
        case PKMN_SPECIES_LILEEP          :
        case PKMN_SPECIES_MILOTIC         :
        case PKMN_SPECIES_NINCADA         :
        case PKMN_SPECIES_NINJASK         :
        case PKMN_SPECIES_SHEDINJA        :
        case PKMN_SPECIES_SWABLU          :
        case PKMN_SPECIES_VOLBEAT         :
        case PKMN_SPECIES_ZANGOOSE        :
            rate = PKMN_LEVELLING_RATE_ERRATIC; break;


        // LEVELLING_RATE_FAST
        case PKMN_SPECIES_AIPOM           :
        case PKMN_SPECIES_ARIADOS         :
        case PKMN_SPECIES_AZUMARILL       :
        case PKMN_SPECIES_AZURILL         :
        case PKMN_SPECIES_BANETTE         :
        case PKMN_SPECIES_BLISSEY         :
        case PKMN_SPECIES_CHANSEY         :
        case PKMN_SPECIES_CHIMECHO        :
        case PKMN_SPECIES_CLEFABLE        :
        case PKMN_SPECIES_CLEFAIRY        :
        case PKMN_SPECIES_CLEFFA          :
        case PKMN_SPECIES_CORSOLA         :
        case PKMN_SPECIES_DELCATTY        :
        case PKMN_SPECIES_DELIBIRD        :
        case PKMN_SPECIES_DUSCLOPS        :
        case PKMN_SPECIES_DUSKULL         :
        case PKMN_SPECIES_GRANBULL        :
        case PKMN_SPECIES_GRUMPIG         :
        case PKMN_SPECIES_IGGLYBUFF       :
        case PKMN_SPECIES_JIGGLYPUFF      :
        case PKMN_SPECIES_LEDIAN          :
        case PKMN_SPECIES_LEDYBA          :
        case PKMN_SPECIES_LUNATONE        :
        case PKMN_SPECIES_LUVDISC         :
        case PKMN_SPECIES_MARILL          :
        case PKMN_SPECIES_MAWILE          :
        case PKMN_SPECIES_MISDREAVUS      :
        case PKMN_SPECIES_SHUPPET         :
        case PKMN_SPECIES_SKITTY          :
        case PKMN_SPECIES_SMEARGLE        :
        case PKMN_SPECIES_SNUBBULL        :
        case PKMN_SPECIES_SOLROCK         :
        case PKMN_SPECIES_SPINARAK        :
        case PKMN_SPECIES_SPINDA          :
        case PKMN_SPECIES_SPOINK          :
        case PKMN_SPECIES_TOGEPI          :
        case PKMN_SPECIES_TOGETIC         :
        case PKMN_SPECIES_WIGGLYTUFF      :
            rate = PKMN_LEVELLING_RATE_FAST; break;


        // LEVELLING_RATE_FLUCTUATING
        case PKMN_SPECIES_BRELOOM         :
        case PKMN_SPECIES_CORPHISH        :
        case PKMN_SPECIES_CRAWDAUNT       :
        case PKMN_SPECIES_GULPIN          :
        case PKMN_SPECIES_HARIYAMA        :
        case PKMN_SPECIES_ILLUMISE        :
        case PKMN_SPECIES_MAKUHITA        :
        case PKMN_SPECIES_SEVIPER         :
        case PKMN_SPECIES_SHROOMISH       :
        case PKMN_SPECIES_SWALOT          :
        case PKMN_SPECIES_WAILMER         :
        case PKMN_SPECIES_WAILORD         :
            rate = PKMN_LEVELLING_RATE_FLUCTUATING; break;


        // LEVELLING_RATE_MEDIUM_FAST
        case PKMN_SPECIES_ARBOK           :
        case PKMN_SPECIES_BALTOY          :
        case PKMN_SPECIES_BARBOACH        :
        case PKMN_SPECIES_BEAUTIFLY       :
        case PKMN_SPECIES_BEEDRILL        :
        case PKMN_SPECIES_BUTTERFREE      :
        case PKMN_SPECIES_CAMERUPT        :
        case PKMN_SPECIES_CASCOON         :
        case PKMN_SPECIES_CASTFORM        :
        case PKMN_SPECIES_CATERPIE        :
        case PKMN_SPECIES_CLAYDOL         :
        case PKMN_SPECIES_CROBAT          :
        case PKMN_SPECIES_CUBONE          :
        case PKMN_SPECIES_DEWGONG         :
        case PKMN_SPECIES_DIGLETT         :
        case PKMN_SPECIES_DITTO           :
        case PKMN_SPECIES_DODRIO          :
        case PKMN_SPECIES_DODUO           :
        case PKMN_SPECIES_DONPHAN         :
        case PKMN_SPECIES_DROWZEE         :
        case PKMN_SPECIES_DUGTRIO         :
        case PKMN_SPECIES_DUNSPARCE       :
        case PKMN_SPECIES_DUSTOX          :
        case PKMN_SPECIES_EEVEE           :
        case PKMN_SPECIES_EKANS           :
        case PKMN_SPECIES_ELECTABUZZ      :
        case PKMN_SPECIES_ELECTRODE       :
        case PKMN_SPECIES_ELEKID          :
        case PKMN_SPECIES_ESPEON          :
        case PKMN_SPECIES_FARFETCHD       :
        case PKMN_SPECIES_FEAROW          :
        case PKMN_SPECIES_FLAREON         :
        case PKMN_SPECIES_FORRETRESS      :
        case PKMN_SPECIES_FURRET          :
        case PKMN_SPECIES_GIRAFARIG       :
        case PKMN_SPECIES_GLALIE          :
        case PKMN_SPECIES_GOLBAT          :
        case PKMN_SPECIES_GOLDEEN         :
        case PKMN_SPECIES_GOLDUCK         :
        case PKMN_SPECIES_GRIMER          :
        case PKMN_SPECIES_HITMONCHAN      :
        case PKMN_SPECIES_HITMONLEE       :
        case PKMN_SPECIES_HITMONTOP       :
        case PKMN_SPECIES_HOOTHOOT        :
        case PKMN_SPECIES_HORSEA          :
        case PKMN_SPECIES_HYPNO           :
        case PKMN_SPECIES_JOLTEON         :
        case PKMN_SPECIES_JYNX            :
        case PKMN_SPECIES_KABUTO          :
        case PKMN_SPECIES_KABUTOPS        :
        case PKMN_SPECIES_KAKUNA          :
        case PKMN_SPECIES_KANGASKHAN      :
        case PKMN_SPECIES_KINGDRA         :
        case PKMN_SPECIES_KINGLER         :
        case PKMN_SPECIES_KOFFING         :
        case PKMN_SPECIES_KRABBY          :
        case PKMN_SPECIES_LICKITUNG       :
        case PKMN_SPECIES_LINOONE         :
        case PKMN_SPECIES_MAGBY           :
        case PKMN_SPECIES_MAGCARGO        :
        case PKMN_SPECIES_MAGMAR          :
        case PKMN_SPECIES_MAGNEMITE       :
        case PKMN_SPECIES_MAGNETON        :
        case PKMN_SPECIES_MANKEY          :
        case PKMN_SPECIES_MAROWAK         :
        case PKMN_SPECIES_MASQUERAIN      :
        case PKMN_SPECIES_MEDICHAM        :
        case PKMN_SPECIES_MEDITITE        :
        case PKMN_SPECIES_MEOWTH          :
        case PKMN_SPECIES_METAPOD         :
        case PKMN_SPECIES_MIGHTYENA       :
        case PKMN_SPECIES_MINUN           :
        case PKMN_SPECIES_MR_MIME         :
        case PKMN_SPECIES_MUK             :
        case PKMN_SPECIES_NATU            :
        case PKMN_SPECIES_NINETALES       :
        case PKMN_SPECIES_NOCTOWL         :
        case PKMN_SPECIES_NOSEPASS        :
        case PKMN_SPECIES_NUMEL           :
        case PKMN_SPECIES_OCTILLERY       :
        case PKMN_SPECIES_OMANYTE         :
        case PKMN_SPECIES_OMASTAR         :
        case PKMN_SPECIES_ONIX            :
        case PKMN_SPECIES_PARAS           :
        case PKMN_SPECIES_PARASECT        :
        case PKMN_SPECIES_PELIPPER        :
        case PKMN_SPECIES_PERSIAN         :
        case PKMN_SPECIES_PHANPY          :
        case PKMN_SPECIES_PICHU           :
        case PKMN_SPECIES_PIKACHU         :
        case PKMN_SPECIES_PINECO          :
        case PKMN_SPECIES_PLUSLE          :
        case PKMN_SPECIES_PONYTA          :
        case PKMN_SPECIES_POOCHYENA       :
        case PKMN_SPECIES_PORYGON         :
        case PKMN_SPECIES_PORYGON2        :
        case PKMN_SPECIES_PRIMEAPE        :
        case PKMN_SPECIES_PSYDUCK         :
        case PKMN_SPECIES_QUAGSIRE        :
        case PKMN_SPECIES_QWILFISH        :
        case PKMN_SPECIES_RAICHU          :
        case PKMN_SPECIES_RAPIDASH        :
        case PKMN_SPECIES_RATICATE        :
        case PKMN_SPECIES_RATTATA         :
        case PKMN_SPECIES_REMORAID        :
        case PKMN_SPECIES_SANDSHREW       :
        case PKMN_SPECIES_SANDSLASH       :
        case PKMN_SPECIES_SCIZOR          :
        case PKMN_SPECIES_SCYTHER         :
        case PKMN_SPECIES_SEADRA          :
        case PKMN_SPECIES_SEAKING         :
        case PKMN_SPECIES_SEEL            :
        case PKMN_SPECIES_SENTRET         :
        case PKMN_SPECIES_SILCOON         :
        case PKMN_SPECIES_SLOWBRO         :
        case PKMN_SPECIES_SLOWKING        :
        case PKMN_SPECIES_SLOWPOKE        :
        case PKMN_SPECIES_SLUGMA          :
        case PKMN_SPECIES_SMOOCHUM        :
        case PKMN_SPECIES_SNORUNT         :
        case PKMN_SPECIES_SPEAROW         :
        case PKMN_SPECIES_STEELIX         :
        case PKMN_SPECIES_SUDOWOODO       :
        case PKMN_SPECIES_SURSKIT         :
        case PKMN_SPECIES_TANGELA         :
        case PKMN_SPECIES_TEDDIURSA       :
        case PKMN_SPECIES_TORKOAL         :
        case PKMN_SPECIES_TYROGUE         :
        case PKMN_SPECIES_UMBREON         :
        case PKMN_SPECIES_UNOWN           :
        case PKMN_SPECIES_URSARING        :
        case PKMN_SPECIES_VAPOREON        :
        case PKMN_SPECIES_VENOMOTH        :
        case PKMN_SPECIES_VENONAT         :
        case PKMN_SPECIES_VOLTORB         :
        case PKMN_SPECIES_VULPIX          :
        case PKMN_SPECIES_WEEDLE          :
        case PKMN_SPECIES_WEEZING         :
        case PKMN_SPECIES_WHISCASH        :
        case PKMN_SPECIES_WINGULL         :
        case PKMN_SPECIES_WOBBUFFET       :
        case PKMN_SPECIES_WOOPER          :
        case PKMN_SPECIES_WURMPLE         :
        case PKMN_SPECIES_WYNAUT          :
        case PKMN_SPECIES_XATU            :
        case PKMN_SPECIES_YANMA           :
        case PKMN_SPECIES_ZIGZAGOON       :
        case PKMN_SPECIES_ZUBAT           :
            rate = PKMN_LEVELLING_RATE_MEDIUM_FAST; break;


        // LEVELLING_RATE_MEDIUM_SLOW
        case PKMN_SPECIES_ABRA            :
        case PKMN_SPECIES_ABSOL           :
        case PKMN_SPECIES_ALAKAZAM        :
        case PKMN_SPECIES_AMPHAROS        :
        case PKMN_SPECIES_BAYLEEF         :
        case PKMN_SPECIES_BELLOSSOM       :
        case PKMN_SPECIES_BELLSPROUT      :
        case PKMN_SPECIES_BLASTOISE       :
        case PKMN_SPECIES_BLAZIKEN        :
        case PKMN_SPECIES_BULBASAUR       :
        case PKMN_SPECIES_CACNEA          :
        case PKMN_SPECIES_CACTURNE        :
        case PKMN_SPECIES_CELEBI          :
        case PKMN_SPECIES_CHARIZARD       :
        case PKMN_SPECIES_CHARMANDER      :
        case PKMN_SPECIES_CHARMELEON      :
        case PKMN_SPECIES_CHIKORITA       :
        case PKMN_SPECIES_COMBUSKEN       :
        case PKMN_SPECIES_CROCONAW        :
        case PKMN_SPECIES_CYNDAQUIL       :
        case PKMN_SPECIES_EXPLOUD         :
        case PKMN_SPECIES_FERALIGATR      :
        case PKMN_SPECIES_FLAAFFY         :
        case PKMN_SPECIES_FLYGON          :
        case PKMN_SPECIES_GASTLY          :
        case PKMN_SPECIES_GENGAR          :
        case PKMN_SPECIES_GEODUDE         :
        case PKMN_SPECIES_GLIGAR          :
        case PKMN_SPECIES_GLOOM           :
        case PKMN_SPECIES_GOLEM           :
        case PKMN_SPECIES_GRAVELER        :
        case PKMN_SPECIES_GROVYLE         :
        case PKMN_SPECIES_HAUNTER         :
        case PKMN_SPECIES_HOPPIP          :
        case PKMN_SPECIES_IVYSAUR         :
        case PKMN_SPECIES_JUMPLUFF        :
        case PKMN_SPECIES_KADABRA         :
        case PKMN_SPECIES_KECLEON         :
        case PKMN_SPECIES_LOMBRE          :
        case PKMN_SPECIES_LOTAD           :
        case PKMN_SPECIES_LOUDRED         :
        case PKMN_SPECIES_LUDICOLO        :
        case PKMN_SPECIES_MACHAMP         :
        case PKMN_SPECIES_MACHOKE         :
        case PKMN_SPECIES_MACHOP          :
        case PKMN_SPECIES_MAREEP          :
        case PKMN_SPECIES_MARSHTOMP       :
        case PKMN_SPECIES_MEGANIUM        :
        case PKMN_SPECIES_MEW             :
        case PKMN_SPECIES_MUDKIP          :
        case PKMN_SPECIES_MURKROW         :
        case PKMN_SPECIES_NIDOKING        :
        case PKMN_SPECIES_NIDOQUEEN       :
        case PKMN_SPECIES_NIDORAN_FEMALE  :
        case PKMN_SPECIES_NIDORAN_MALE    :
        case PKMN_SPECIES_NIDORINA        :
        case PKMN_SPECIES_NIDORINO        :
        case PKMN_SPECIES_NUZLEAF         :
        case PKMN_SPECIES_ODDISH          :
        case PKMN_SPECIES_PIDGEOT         :
        case PKMN_SPECIES_PIDGEOTTO       :
        case PKMN_SPECIES_PIDGEY          :
        case PKMN_SPECIES_POLITOED        :
        case PKMN_SPECIES_POLIWAG         :
        case PKMN_SPECIES_POLIWHIRL       :
        case PKMN_SPECIES_POLIWRATH       :
        case PKMN_SPECIES_QUILAVA         :
        case PKMN_SPECIES_ROSELIA         :
        case PKMN_SPECIES_SABLEYE         :
        case PKMN_SPECIES_SCEPTILE        :
        case PKMN_SPECIES_SEALEO          :
        case PKMN_SPECIES_SEEDOT          :
        case PKMN_SPECIES_SHIFTRY         :
        case PKMN_SPECIES_SHUCKLE         :
        case PKMN_SPECIES_SKIPLOOM        :
        case PKMN_SPECIES_SNEASEL         :
        case PKMN_SPECIES_SPHEAL          :
        case PKMN_SPECIES_SQUIRTLE        :
        case PKMN_SPECIES_SUNFLORA        :
        case PKMN_SPECIES_SUNKERN         :
        case PKMN_SPECIES_SWAMPERT        :
        case PKMN_SPECIES_SWELLOW         :
        case PKMN_SPECIES_TAILLOW         :
        case PKMN_SPECIES_TORCHIC         :
        case PKMN_SPECIES_TOTODILE        :
        case PKMN_SPECIES_TRAPINCH        :
        case PKMN_SPECIES_TREECKO         :
        case PKMN_SPECIES_TYPHLOSION      :
        case PKMN_SPECIES_VENUSAUR        :
        case PKMN_SPECIES_VIBRAVA         :
        case PKMN_SPECIES_VICTREEBEL      :
        case PKMN_SPECIES_VILEPLUME       :
        case PKMN_SPECIES_WALREIN         :
        case PKMN_SPECIES_WARTORTLE       :
        case PKMN_SPECIES_WEEPINBELL      :
        case PKMN_SPECIES_WHISMUR         :
            rate = PKMN_LEVELLING_RATE_MEDIUM_SLOW; break;


        // LEVELLING_RATE_SLOW
        case PKMN_SPECIES_AERODACTYL      :
        case PKMN_SPECIES_AGGRON          :
        case PKMN_SPECIES_ARCANINE        :
        case PKMN_SPECIES_ARON            :
        case PKMN_SPECIES_ARTICUNO        :
        case PKMN_SPECIES_BAGON           :
        case PKMN_SPECIES_BELDUM          :
        case PKMN_SPECIES_CARVANHA        :
        case PKMN_SPECIES_CHINCHOU        :
        case PKMN_SPECIES_CLOYSTER        :
        case PKMN_SPECIES_DEOXYS          :
        case PKMN_SPECIES_DRAGONAIR       :
        case PKMN_SPECIES_DRAGONITE       :
        case PKMN_SPECIES_DRATINI         :
        case PKMN_SPECIES_ELECTRIKE       :
        case PKMN_SPECIES_ENTEI           :
        case PKMN_SPECIES_EXEGGCUTE       :
        case PKMN_SPECIES_EXEGGUTOR       :
        case PKMN_SPECIES_GARDEVOIR       :
        case PKMN_SPECIES_GROUDON         :
        case PKMN_SPECIES_GROWLITHE       :
        case PKMN_SPECIES_GYARADOS        :
        case PKMN_SPECIES_HERACROSS       :
        case PKMN_SPECIES_HO_OH           :
        case PKMN_SPECIES_HOUNDOOM        :
        case PKMN_SPECIES_HOUNDOUR        :
        case PKMN_SPECIES_JIRACHI         :
        case PKMN_SPECIES_KIRLIA          :
        case PKMN_SPECIES_KYOGRE          :
        case PKMN_SPECIES_LAIRON          :
        case PKMN_SPECIES_LANTURN         :
        case PKMN_SPECIES_LAPRAS          :
        case PKMN_SPECIES_LARVITAR        :
        case PKMN_SPECIES_LATIAS          :
        case PKMN_SPECIES_LATIOS          :
        case PKMN_SPECIES_LUGIA           :
        case PKMN_SPECIES_MAGIKARP        :
        case PKMN_SPECIES_MANECTRIC       :
        case PKMN_SPECIES_MANTINE         :
        case PKMN_SPECIES_METAGROSS       :
        case PKMN_SPECIES_METANG          :
        case PKMN_SPECIES_MEWTWO          :
        case PKMN_SPECIES_MILTANK         :
        case PKMN_SPECIES_MOLTRES         :
        case PKMN_SPECIES_PILOSWINE       :
        case PKMN_SPECIES_PINSIR          :
        case PKMN_SPECIES_PUPITAR         :
        case PKMN_SPECIES_RAIKOU          :
        case PKMN_SPECIES_RALTS           :
        case PKMN_SPECIES_RAYQUAZA        :
        case PKMN_SPECIES_REGICE          :
        case PKMN_SPECIES_REGIROCK        :
        case PKMN_SPECIES_REGISTEEL       :
        case PKMN_SPECIES_RELICANTH       :
        case PKMN_SPECIES_RHYDON          :
        case PKMN_SPECIES_RHYHORN         :
        case PKMN_SPECIES_SALAMENCE       :
        case PKMN_SPECIES_SHARPEDO        :
        case PKMN_SPECIES_SHELGON         :
        case PKMN_SPECIES_SHELLDER        :
        case PKMN_SPECIES_SKARMORY        :
        case PKMN_SPECIES_SLAKING         :
        case PKMN_SPECIES_SLAKOTH         :
        case PKMN_SPECIES_SNORLAX         :
        case PKMN_SPECIES_STANTLER        :
        case PKMN_SPECIES_STARMIE         :
        case PKMN_SPECIES_STARYU          :
        case PKMN_SPECIES_SUICUNE         :
        case PKMN_SPECIES_SWINUB          :
        case PKMN_SPECIES_TAUROS          :
        case PKMN_SPECIES_TENTACOOL       :
        case PKMN_SPECIES_TENTACRUEL      :
        case PKMN_SPECIES_TROPIUS         :
        case PKMN_SPECIES_TYRANITAR       :
        case PKMN_SPECIES_VIGOROTH        :
        case PKMN_SPECIES_ZAPDOS          :
            rate = PKMN_LEVELLING_RATE_SLOW; break;

        default:
            rate = PKMN_LEVELLING_RATE_FAST; break; // TODO handle properly
    };
    return rate;
}

void
pkmn_init_abilities(const enum Pokedex species, AbilityPair *abi) {
    switch (species) {
        case PKMN_SPECIES__UNUSED:
            abi->a = PKMN_ABILITY_NO_ABILITY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ABRA:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_INNER_FOCUS;
        case PKMN_SPECIES_ABSOL:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_AERODACTYL:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_PRESSURE;
        case PKMN_SPECIES_AGGRON:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_AIPOM:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_PICKUP;
        case PKMN_SPECIES_ALAKAZAM:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_INNER_FOCUS;
        case PKMN_SPECIES_ALTARIA:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_AMPHAROS:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ANORITH:
            abi->a = PKMN_ABILITY_BATTLE_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ARBOK:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_SHED_SKIN;
        case PKMN_SPECIES_ARCANINE:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_ARIADOS:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_INSOMNIA;
        case PKMN_SPECIES_ARMALDO:
            abi->a = PKMN_ABILITY_BATTLE_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ARON:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_ARTICUNO:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_AZUMARILL:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_HUGE_POWER;
        case PKMN_SPECIES_AZURILL:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_HUGE_POWER;
        case PKMN_SPECIES_BAGON:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BALTOY:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BANETTE:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BARBOACH:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BAYLEEF:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BEAUTIFLY:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BEEDRILL:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BELDUM:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BELLOSSOM:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BELLSPROUT:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BLASTOISE:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BLAZIKEN:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BLISSEY:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_SERENE_GRACE;
        case PKMN_SPECIES_BRELOOM:
            abi->a = PKMN_ABILITY_EFFECT_SPORE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BULBASAUR:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_BUTTERFREE:
            abi->a = PKMN_ABILITY_COMPOUND_EYES;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CACNEA:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CACTURNE:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CAMERUPT:
            abi->a = PKMN_ABILITY_MAGMA_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CARVANHA:
            abi->a = PKMN_ABILITY_ROUGH_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CASCOON:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CASTFORM:
            abi->a = PKMN_ABILITY_FORECAST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CATERPIE:
            abi->a = PKMN_ABILITY_SHIELD_DUST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CELEBI:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHANSEY:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_SERENE_GRACE;
        case PKMN_SPECIES_CHARIZARD:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHARMANDER:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHARMELEON:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHIKORITA:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHIMECHO:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CHINCHOU:
            abi->a = PKMN_ABILITY_VOLT_ABSORB;
            abi->b = PKMN_ABILITY_ILLUMINATE;
        case PKMN_SPECIES_CLAMPERL:
            abi->a = PKMN_ABILITY_SHELL_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CLAYDOL:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CLEFABLE:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CLEFAIRY:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CLEFFA:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CLOYSTER:
            abi->a = PKMN_ABILITY_SHELL_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_COMBUSKEN:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CORPHISH:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_CORSOLA:
            abi->a = PKMN_ABILITY_HUSTLE;
            abi->b = PKMN_ABILITY_NATURAL_CURE;
        case PKMN_SPECIES_CRADILY:
            abi->a = PKMN_ABILITY_SUCTION_CUPS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CRAWDAUNT:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_CROBAT:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CROCONAW:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_CUBONE:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_LIGHTNING_ROD;
        case PKMN_SPECIES_CYNDAQUIL:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DELCATTY:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DELIBIRD:
            abi->a = PKMN_ABILITY_VITAL_SPIRIT;
            abi->b = PKMN_ABILITY_HUSTLE;
        case PKMN_SPECIES_DEOXYS:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DEWGONG:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DIGLETT:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_ARENA_TRAP;
        case PKMN_SPECIES_DITTO:
            abi->a = PKMN_ABILITY_LIMBER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DODRIO:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_DODUO:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_DONPHAN:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DRAGONAIR:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DRAGONITE:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DRATINI:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DROWZEE:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DUGTRIO:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_ARENA_TRAP;
        case PKMN_SPECIES_DUNSPARCE:
            abi->a = PKMN_ABILITY_SERENE_GRACE;
            abi->b = PKMN_ABILITY_RUN_AWAY;
        case PKMN_SPECIES_DUSCLOPS:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DUSKULL:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_DUSTOX:
            abi->a = PKMN_ABILITY_SHIELD_DUST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_EEVEE:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_EKANS:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_SHED_SKIN;
        case PKMN_SPECIES_ELECTABUZZ:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ELECTRIKE:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_LIGHTNING_ROD;
        case PKMN_SPECIES_ELECTRODE:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_STATIC;
        case PKMN_SPECIES_ELEKID:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ENTEI:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ESPEON:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_EXEGGCUTE:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_EXEGGUTOR:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_EXPLOUD:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FARFETCHD:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_INNER_FOCUS;
        case PKMN_SPECIES_FEAROW:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FEEBAS:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_OBLIVIOUS;
        case PKMN_SPECIES_FERALIGATR:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FLAAFFY:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FLAREON:
            abi->a = PKMN_ABILITY_FLASH_FIRE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FLYGON:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FORRETRESS:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_FURRET:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_KEEN_EYE;
        case PKMN_SPECIES_GARDEVOIR:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_TRACE;
        case PKMN_SPECIES_GASTLY:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GENGAR:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GEODUDE:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_GIRAFARIG:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_GLALIE:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GLIGAR:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_SAND_VEIL;
        case PKMN_SPECIES_GLOOM:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GOLBAT:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GOLDEEN:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_WATER_VEIL;
        case PKMN_SPECIES_GOLDUCK:
            abi->a = PKMN_ABILITY_DAMP;
            abi->b = PKMN_ABILITY_CLOUD_NINE;
        case PKMN_SPECIES_GOLEM:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_GOREBYSS:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GRANBULL:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GRAVELER:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_GRIMER:
            abi->a = PKMN_ABILITY_STENCH;
            abi->b = PKMN_ABILITY_STICKY_HOLD;
        case PKMN_SPECIES_GROUDON:
            abi->a = PKMN_ABILITY_DROUGHT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GROVYLE:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_GROWLITHE:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_GRUMPIG:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_OWN_TEMPO;
        case PKMN_SPECIES_GULPIN:
            abi->a = PKMN_ABILITY_LIQUID_OOZE;
            abi->b = PKMN_ABILITY_STICKY_HOLD;
        case PKMN_SPECIES_GYARADOS:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HARIYAMA:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_GUTS;
        case PKMN_SPECIES_HAUNTER:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HERACROSS:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_GUTS;
        case PKMN_SPECIES_HITMONCHAN:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HITMONLEE:
            abi->a = PKMN_ABILITY_LIMBER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HITMONTOP:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HO_OH:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HOOTHOOT:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_KEEN_EYE;
        case PKMN_SPECIES_HOPPIP:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HORSEA:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HOUNDOOM:
            abi->a = PKMN_ABILITY_EARLY_BIRD;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_HOUNDOUR:
            abi->a = PKMN_ABILITY_EARLY_BIRD;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_HUNTAIL:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_HYPNO:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_IGGLYBUFF:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ILLUMISE:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_IVYSAUR:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_JIGGLYPUFF:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_JIRACHI:
            abi->a = PKMN_ABILITY_SERENE_GRACE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_JOLTEON:
            abi->a = PKMN_ABILITY_VOLT_ABSORB;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_JUMPLUFF:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_JYNX:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KABUTO:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_BATTLE_ARMOR;
        case PKMN_SPECIES_KABUTOPS:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_BATTLE_ARMOR;
        case PKMN_SPECIES_KADABRA:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_INNER_FOCUS;
        case PKMN_SPECIES_KAKUNA:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KANGASKHAN:
            abi->a = PKMN_ABILITY_EARLY_BIRD;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KECLEON:
            abi->a = PKMN_ABILITY_COLOR_CHANGE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KINGDRA:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KINGLER:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_KIRLIA:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_TRACE;
        case PKMN_SPECIES_KOFFING:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_KRABBY:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_KYOGRE:
            abi->a = PKMN_ABILITY_DRIZZLE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LAIRON:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_LANTURN:
            abi->a = PKMN_ABILITY_VOLT_ABSORB;
            abi->b = PKMN_ABILITY_ILLUMINATE;
        case PKMN_SPECIES_LAPRAS:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_LARVITAR:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LATIAS:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LATIOS:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LEDIAN:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_LEDYBA:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_LICKITUNG:
            abi->a = PKMN_ABILITY_OWN_TEMPO;
            abi->b = PKMN_ABILITY_OBLIVIOUS;
        case PKMN_SPECIES_LILEEP:
            abi->a = PKMN_ABILITY_SUCTION_CUPS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LINOONE:
            abi->a = PKMN_ABILITY_PICKUP;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LOMBRE:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_RAIN_DISH;
        case PKMN_SPECIES_LOTAD:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_RAIN_DISH;
        case PKMN_SPECIES_LOUDRED:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LUDICOLO:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_RAIN_DISH;
        case PKMN_SPECIES_LUGIA:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LUNATONE:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_LUVDISC:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MACHAMP:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MACHOKE:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MACHOP:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MAGBY:
            abi->a = PKMN_ABILITY_FLAME_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MAGCARGO:
            abi->a = PKMN_ABILITY_MAGMA_ARMOR;
            abi->b = PKMN_ABILITY_FLAME_BODY;
        case PKMN_SPECIES_MAGIKARP:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MAGMAR:
            abi->a = PKMN_ABILITY_FLAME_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MAGNEMITE:
            abi->a = PKMN_ABILITY_MAGNET_PULL;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_MAGNETON:
            abi->a = PKMN_ABILITY_MAGNET_PULL;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_MAKUHITA:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_GUTS;
        case PKMN_SPECIES_MANECTRIC:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_LIGHTNING_ROD;
        case PKMN_SPECIES_MANKEY:
            abi->a = PKMN_ABILITY_VITAL_SPIRIT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MANTINE:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_WATER_ABSORB;
        case PKMN_SPECIES_MAREEP:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MARILL:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_HUGE_POWER;
        case PKMN_SPECIES_MAROWAK:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_LIGHTNING_ROD;
        case PKMN_SPECIES_MARSHTOMP:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MASQUERAIN:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MAWILE:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_INTIMIDATE;
        case PKMN_SPECIES_MEDICHAM:
            abi->a = PKMN_ABILITY_PURE_POWER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MEDITITE:
            abi->a = PKMN_ABILITY_PURE_POWER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MEGANIUM:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MEOWTH:
            abi->a = PKMN_ABILITY_PICKUP;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_METAGROSS:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_METANG:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_METAPOD:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MEW:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MEWTWO:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MIGHTYENA:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MILOTIC:
            abi->a = PKMN_ABILITY_MARVEL_SCALE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MILTANK:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MINUN:
            abi->a = PKMN_ABILITY_MINUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MISDREAVUS:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MOLTRES:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MR_MIME:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MUDKIP:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_MUK:
            abi->a = PKMN_ABILITY_STENCH;
            abi->b = PKMN_ABILITY_STICKY_HOLD;
        case PKMN_SPECIES_MURKROW:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NATU:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_NIDOKING:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NIDOQUEEN:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NIDORAN_FEMALE:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NIDORAN_MALE:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NIDORINA:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NIDORINO:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NINCADA:
            abi->a = PKMN_ABILITY_COMPOUND_EYES;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NINETALES:
            abi->a = PKMN_ABILITY_FLASH_FIRE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NINJASK:
            abi->a = PKMN_ABILITY_SPEED_BOOST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NOCTOWL:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_KEEN_EYE;
        case PKMN_SPECIES_NOSEPASS:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_MAGNET_PULL;
        case PKMN_SPECIES_NUMEL:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_NUZLEAF:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_OCTILLERY:
            abi->a = PKMN_ABILITY_SUCTION_CUPS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ODDISH:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_OMANYTE:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_OMASTAR:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_SHELL_ARMOR;
        case PKMN_SPECIES_ONIX:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_PARAS:
            abi->a = PKMN_ABILITY_EFFECT_SPORE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PARASECT:
            abi->a = PKMN_ABILITY_EFFECT_SPORE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PELIPPER:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_DRIZZLE;
        case PKMN_SPECIES_PERSIAN:
            abi->a = PKMN_ABILITY_LIMBER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PHANPY:
            abi->a = PKMN_ABILITY_PICKUP;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PICHU:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PIDGEOT:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PIDGEOTTO:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PIDGEY:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PIKACHU:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PILOSWINE:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PINECO:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PINSIR:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PLUSLE:
            abi->a = PKMN_ABILITY_PLUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_POLITOED:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_DAMP;
        case PKMN_SPECIES_POLIWAG:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_DAMP;
        case PKMN_SPECIES_POLIWHIRL:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_DAMP;
        case PKMN_SPECIES_POLIWRATH:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_DAMP;
        case PKMN_SPECIES_PONYTA:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_POOCHYENA:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PORYGON:
            abi->a = PKMN_ABILITY_TRACE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PORYGON2:
            abi->a = PKMN_ABILITY_TRACE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PRIMEAPE:
            abi->a = PKMN_ABILITY_VITAL_SPIRIT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_PSYDUCK:
            abi->a = PKMN_ABILITY_DAMP;
            abi->b = PKMN_ABILITY_CLOUD_NINE;
        case PKMN_SPECIES_PUPITAR:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_QUAGSIRE:
            abi->a = PKMN_ABILITY_DAMP;
            abi->b = PKMN_ABILITY_WATER_ABSORB;
        case PKMN_SPECIES_QUILAVA:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_QWILFISH:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_SWIFT_SWIM;
        case PKMN_SPECIES_RAICHU:
            abi->a = PKMN_ABILITY_STATIC;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_RAIKOU:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_RALTS:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_TRACE;
        case PKMN_SPECIES_RAPIDASH:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_FLASH_FIRE;
        case PKMN_SPECIES_RATICATE:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_GUTS;
        case PKMN_SPECIES_RATTATA:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_GUTS;
        case PKMN_SPECIES_RAYQUAZA:
            abi->a = PKMN_ABILITY_AIR_LOCK;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_REGICE:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_REGIROCK:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_REGISTEEL:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_RELICANTH:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_REMORAID:
            abi->a = PKMN_ABILITY_HUSTLE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_RHYDON:
            abi->a = PKMN_ABILITY_LIGHTNING_ROD;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_RHYHORN:
            abi->a = PKMN_ABILITY_LIGHTNING_ROD;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_ROSELIA:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_POISON_POINT;
        case PKMN_SPECIES_SABLEYE:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SALAMENCE:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SANDSHREW:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SANDSLASH:
            abi->a = PKMN_ABILITY_SAND_VEIL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SCEPTILE:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SCIZOR:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SCYTHER:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SEADRA:
            abi->a = PKMN_ABILITY_POISON_POINT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SEAKING:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_WATER_VEIL;
        case PKMN_SPECIES_SEALEO:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SEEDOT:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_SEEL:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SENTRET:
            abi->a = PKMN_ABILITY_RUN_AWAY;
            abi->b = PKMN_ABILITY_KEEN_EYE;
        case PKMN_SPECIES_SEVIPER:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHARPEDO:
            abi->a = PKMN_ABILITY_ROUGH_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHEDINJA:
            abi->a = PKMN_ABILITY_WONDER_GUARD;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHELGON:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHELLDER:
            abi->a = PKMN_ABILITY_SHELL_ARMOR;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHIFTRY:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHROOMISH:
            abi->a = PKMN_ABILITY_EFFECT_SPORE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHUCKLE:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SHUPPET:
            abi->a = PKMN_ABILITY_INSOMNIA;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SILCOON:
            abi->a = PKMN_ABILITY_SHED_SKIN;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SKARMORY:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_SKIPLOOM:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SKITTY:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SLAKING:
            abi->a = PKMN_ABILITY_TRUANT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SLAKOTH:
            abi->a = PKMN_ABILITY_TRUANT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SLOWBRO:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_OWN_TEMPO;
        case PKMN_SPECIES_SLOWKING:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_OWN_TEMPO;
        case PKMN_SPECIES_SLOWPOKE:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_OWN_TEMPO;
        case PKMN_SPECIES_SLUGMA:
            abi->a = PKMN_ABILITY_MAGMA_ARMOR;
            abi->b = PKMN_ABILITY_FLAME_BODY;
        case PKMN_SPECIES_SMEARGLE:
            abi->a = PKMN_ABILITY_OWN_TEMPO;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SMOOCHUM:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SNEASEL:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_KEEN_EYE;
        case PKMN_SPECIES_SNORLAX:
            abi->a = PKMN_ABILITY_IMMUNITY;
            abi->b = PKMN_ABILITY_THICK_FAT;
        case PKMN_SPECIES_SNORUNT:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SNUBBULL:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_RUN_AWAY;
        case PKMN_SPECIES_SOLROCK:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SPEAROW:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SPHEAL:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SPINARAK:
            abi->a = PKMN_ABILITY_SWARM;
            abi->b = PKMN_ABILITY_INSOMNIA;
        case PKMN_SPECIES_SPINDA:
            abi->a = PKMN_ABILITY_OWN_TEMPO;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SPOINK:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_OWN_TEMPO;
        case PKMN_SPECIES_SQUIRTLE:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_STANTLER:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_STARMIE:
            abi->a = PKMN_ABILITY_ILLUMINATE;
            abi->b = PKMN_ABILITY_NATURAL_CURE;
        case PKMN_SPECIES_STARYU:
            abi->a = PKMN_ABILITY_ILLUMINATE;
            abi->b = PKMN_ABILITY_NATURAL_CURE;
        case PKMN_SPECIES_STEELIX:
            abi->a = PKMN_ABILITY_ROCK_HEAD;
            abi->b = PKMN_ABILITY_STURDY;
        case PKMN_SPECIES_SUDOWOODO:
            abi->a = PKMN_ABILITY_STURDY;
            abi->b = PKMN_ABILITY_ROCK_HEAD;
        case PKMN_SPECIES_SUICUNE:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SUNFLORA:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SUNKERN:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SURSKIT:
            abi->a = PKMN_ABILITY_SWIFT_SWIM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SWABLU:
            abi->a = PKMN_ABILITY_NATURAL_CURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SWALOT:
            abi->a = PKMN_ABILITY_LIQUID_OOZE;
            abi->b = PKMN_ABILITY_STICKY_HOLD;
        case PKMN_SPECIES_SWAMPERT:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SWELLOW:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_SWINUB:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TAILLOW:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TANGELA:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TAUROS:
            abi->a = PKMN_ABILITY_INTIMIDATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TEDDIURSA:
            abi->a = PKMN_ABILITY_PICKUP;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TENTACOOL:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_LIQUID_OOZE;
        case PKMN_SPECIES_TENTACRUEL:
            abi->a = PKMN_ABILITY_CLEAR_BODY;
            abi->b = PKMN_ABILITY_LIQUID_OOZE;
        case PKMN_SPECIES_TOGEPI:
            abi->a = PKMN_ABILITY_HUSTLE;
            abi->b = PKMN_ABILITY_SERENE_GRACE;
        case PKMN_SPECIES_TOGETIC:
            abi->a = PKMN_ABILITY_HUSTLE;
            abi->b = PKMN_ABILITY_SERENE_GRACE;
        case PKMN_SPECIES_TORCHIC:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TORKOAL:
            abi->a = PKMN_ABILITY_WHITE_SMOKE;
            abi->b = PKMN_ABILITY_DROUGHT;
        case PKMN_SPECIES_TOTODILE:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TRAPINCH:
            abi->a = PKMN_ABILITY_HYPER_CUTTER;
            abi->b = PKMN_ABILITY_ARENA_TRAP;
        case PKMN_SPECIES_TREECKO:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TROPIUS:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TYPHLOSION:
            abi->a = PKMN_ABILITY_BLAZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TYRANITAR:
            abi->a = PKMN_ABILITY_SAND_STREAM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_TYROGUE:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_UMBREON:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_UNOWN:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_URSARING:
            abi->a = PKMN_ABILITY_GUTS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VAPOREON:
            abi->a = PKMN_ABILITY_WATER_ABSORB;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VENOMOTH:
            abi->a = PKMN_ABILITY_SHIELD_DUST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VENONAT:
            abi->a = PKMN_ABILITY_COMPOUND_EYES;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VENUSAUR:
            abi->a = PKMN_ABILITY_OVERGROW;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VIBRAVA:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VICTREEBEL:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VIGOROTH:
            abi->a = PKMN_ABILITY_VITAL_SPIRIT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VILEPLUME:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_VOLBEAT:
            abi->a = PKMN_ABILITY_ILLUMINATE;
            abi->b = PKMN_ABILITY_SWARM;
        case PKMN_SPECIES_VOLTORB:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_STATIC;
        case PKMN_SPECIES_VULPIX:
            abi->a = PKMN_ABILITY_FLASH_FIRE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WAILMER:
            abi->a = PKMN_ABILITY_WATER_VEIL;
            abi->b = PKMN_ABILITY_OBLIVIOUS;
        case PKMN_SPECIES_WAILORD:
            abi->a = PKMN_ABILITY_WATER_VEIL;
            abi->b = PKMN_ABILITY_OBLIVIOUS;
        case PKMN_SPECIES_WALREIN:
            abi->a = PKMN_ABILITY_THICK_FAT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WARTORTLE:
            abi->a = PKMN_ABILITY_TORRENT;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WEEDLE:
            abi->a = PKMN_ABILITY_SHIELD_DUST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WEEPINBELL:
            abi->a = PKMN_ABILITY_CHLOROPHYLL;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WEEZING:
            abi->a = PKMN_ABILITY_LEVITATE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WHISCASH:
            abi->a = PKMN_ABILITY_OBLIVIOUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WHISMUR:
            abi->a = PKMN_ABILITY_SOUNDPROOF;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WIGGLYTUFF:
            abi->a = PKMN_ABILITY_CUTE_CHARM;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WINGULL:
            abi->a = PKMN_ABILITY_KEEN_EYE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WOBBUFFET:
            abi->a = PKMN_ABILITY_SHADOW_TAG;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WOOPER:
            abi->a = PKMN_ABILITY_DAMP;
            abi->b = PKMN_ABILITY_WATER_ABSORB;
        case PKMN_SPECIES_WURMPLE:
            abi->a = PKMN_ABILITY_SHIELD_DUST;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_WYNAUT:
            abi->a = PKMN_ABILITY_SHADOW_TAG;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_XATU:
            abi->a = PKMN_ABILITY_SYNCHRONIZE;
            abi->b = PKMN_ABILITY_EARLY_BIRD;
        case PKMN_SPECIES_YANMA:
            abi->a = PKMN_ABILITY_SPEED_BOOST;
            abi->b = PKMN_ABILITY_COMPOUND_EYES;
        case PKMN_SPECIES_ZANGOOSE:
            abi->a = PKMN_ABILITY_IMMUNITY;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ZAPDOS:
            abi->a = PKMN_ABILITY_PRESSURE;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ZIGZAGOON:
            abi->a = PKMN_ABILITY_PICKUP;
            abi->b = PKMN_ABILITY_NO_ABILITY;
        case PKMN_SPECIES_ZUBAT:
            abi->a = PKMN_ABILITY_INNER_FOCUS;
            abi->b = PKMN_ABILITY_NO_ABILITY;
    };
}

uint8_t
pkmn_get_base_stat_value(const enum Pokedex species, enum PkmnStat stat) {
    uint8_t val;
    switch (species) {
        case PKMN_SPECIES__UNUSED:
            val = 0; break;
        case PKMN_SPECIES_ABRA:
            switch (stat) {
                case PKMN_STAT_HP : val = 25  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 15  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_ABSOL:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_AERODACTYL:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 130 ; break;
            };
        case PKMN_SPECIES_AGGRON:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 180 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_AIPOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_ALAKAZAM:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 135 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 120 ; break;
            };
        case PKMN_SPECIES_ALTARIA:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 105 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_AMPHAROS:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_ANORITH:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_ARBOK:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 69  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 79  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_ARCANINE:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_ARIADOS:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_ARMALDO:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 125 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_ARON:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_ARTICUNO:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 125 ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_AZUMARILL:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_AZURILL:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_BAGON:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_BALTOY:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_BANETTE:
            switch (stat) {
                case PKMN_STAT_HP : val = 64  ; break;
                case PKMN_STAT_ATK: val = 115 ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 83  ; break;
                case PKMN_STAT_SPD: val = 63  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_BARBOACH:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 48  ; break;
                case PKMN_STAT_DEF: val = 43  ; break;
                case PKMN_STAT_SPA: val = 46  ; break;
                case PKMN_STAT_SPD: val = 41  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_BAYLEEF:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 62  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 63  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_BEAUTIFLY:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_BEEDRILL:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_BELDUM:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_BELLOSSOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_BELLSPROUT:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_BLASTOISE:
            switch (stat) {
                case PKMN_STAT_HP : val = 79  ; break;
                case PKMN_STAT_ATK: val = 83  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 105 ; break;
                case PKMN_STAT_SPE: val = 78  ; break;
            };
        case PKMN_SPECIES_BLAZIKEN:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_BLISSEY:
            switch (stat) {
                case PKMN_STAT_HP : val = 255 ; break;
                case PKMN_STAT_ATK: val = 10  ; break;
                case PKMN_STAT_DEF: val = 10  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 135 ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_BRELOOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_BULBASAUR:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 49  ; break;
                case PKMN_STAT_DEF: val = 49  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_BUTTERFREE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_CACNEA:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_CACTURNE:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 115 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_CAMERUPT:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_CARVANHA:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 20  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 20  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_CASCOON:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_CASTFORM:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_CATERPIE:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 20  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_CELEBI:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_CHANSEY:
            switch (stat) {
                case PKMN_STAT_HP : val = 250 ; break;
                case PKMN_STAT_ATK: val = 5   ; break;
                case PKMN_STAT_DEF: val = 5   ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 105 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_CHARIZARD:
            switch (stat) {
                case PKMN_STAT_HP : val = 78  ; break;
                case PKMN_STAT_ATK: val = 84  ; break;
                case PKMN_STAT_DEF: val = 78  ; break;
                case PKMN_STAT_SPA: val = 109 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_CHARMANDER:
            switch (stat) {
                case PKMN_STAT_HP : val = 39  ; break;
                case PKMN_STAT_ATK: val = 52  ; break;
                case PKMN_STAT_DEF: val = 43  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_CHARMELEON:
            switch (stat) {
                case PKMN_STAT_HP : val = 58  ; break;
                case PKMN_STAT_ATK: val = 64  ; break;
                case PKMN_STAT_DEF: val = 58  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_CHIKORITA:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 49  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 49  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_CHIMECHO:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_CHINCHOU:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 38  ; break;
                case PKMN_STAT_DEF: val = 38  ; break;
                case PKMN_STAT_SPA: val = 56  ; break;
                case PKMN_STAT_SPD: val = 56  ; break;
                case PKMN_STAT_SPE: val = 67  ; break;
            };
        case PKMN_SPECIES_CLAMPERL:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 64  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 74  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 32  ; break;
            };
        case PKMN_SPECIES_CLAYDOL:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 120 ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_CLEFABLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 73  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_CLEFAIRY:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 48  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_CLEFFA:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 25  ; break;
                case PKMN_STAT_DEF: val = 28  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_CLOYSTER:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 180 ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_COMBUSKEN:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_CORPHISH:
            switch (stat) {
                case PKMN_STAT_HP : val = 43  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_CORSOLA:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_CRADILY:
            switch (stat) {
                case PKMN_STAT_HP : val = 86  ; break;
                case PKMN_STAT_ATK: val = 81  ; break;
                case PKMN_STAT_DEF: val = 97  ; break;
                case PKMN_STAT_SPA: val = 81  ; break;
                case PKMN_STAT_SPD: val = 107 ; break;
                case PKMN_STAT_SPE: val = 43  ; break;
            };
        case PKMN_SPECIES_CRAWDAUNT:
            switch (stat) {
                case PKMN_STAT_HP : val = 63  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_CROBAT:
            switch (stat) {
                case PKMN_STAT_HP : val = 85  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 130 ; break;
            };
        case PKMN_SPECIES_CROCONAW:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 59  ; break;
                case PKMN_STAT_SPD: val = 63  ; break;
                case PKMN_STAT_SPE: val = 58  ; break;
            };
        case PKMN_SPECIES_CUBONE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_CYNDAQUIL:
            switch (stat) {
                case PKMN_STAT_HP : val = 39  ; break;
                case PKMN_STAT_ATK: val = 52  ; break;
                case PKMN_STAT_DEF: val = 43  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_DELCATTY:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_DELIBIRD:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_DEOXYS:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 150 ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 150 ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 150 ; break;
            };
        case PKMN_SPECIES_DEWGONG:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_DIGLETT:
            switch (stat) {
                case PKMN_STAT_HP : val = 10  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 25  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_DITTO:
            switch (stat) {
                case PKMN_STAT_HP : val = 48  ; break;
                case PKMN_STAT_ATK: val = 48  ; break;
                case PKMN_STAT_DEF: val = 48  ; break;
                case PKMN_STAT_SPA: val = 48  ; break;
                case PKMN_STAT_SPD: val = 48  ; break;
                case PKMN_STAT_SPE: val = 48  ; break;
            };
        case PKMN_SPECIES_DODRIO:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_DODUO:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_DONPHAN:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 120 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_DRAGONAIR:
            switch (stat) {
                case PKMN_STAT_HP : val = 61  ; break;
                case PKMN_STAT_ATK: val = 84  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_DRAGONITE:
            switch (stat) {
                case PKMN_STAT_HP : val = 91  ; break;
                case PKMN_STAT_ATK: val = 134 ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_DRATINI:
            switch (stat) {
                case PKMN_STAT_HP : val = 41  ; break;
                case PKMN_STAT_ATK: val = 64  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_DROWZEE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 48  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 43  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 42  ; break;
            };
        case PKMN_SPECIES_DUGTRIO:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 120 ; break;
            };
        case PKMN_SPECIES_DUNSPARCE:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_DUSCLOPS:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 130 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 130 ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_DUSKULL:
            switch (stat) {
                case PKMN_STAT_HP : val = 20  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_DUSTOX:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_EEVEE:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_EKANS:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 44  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 54  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_ELECTABUZZ:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 83  ; break;
                case PKMN_STAT_DEF: val = 57  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 105 ; break;
            };
        case PKMN_SPECIES_ELECTRIKE:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_ELECTRODE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 140 ; break;
            };
        case PKMN_SPECIES_ELEKID:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 63  ; break;
                case PKMN_STAT_DEF: val = 37  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_ENTEI:
            switch (stat) {
                case PKMN_STAT_HP : val = 115 ; break;
                case PKMN_STAT_ATK: val = 115 ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_ESPEON:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 130 ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_EXEGGCUTE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_EXEGGUTOR:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 125 ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_EXPLOUD:
            switch (stat) {
                case PKMN_STAT_HP : val = 104 ; break;
                case PKMN_STAT_ATK: val = 91  ; break;
                case PKMN_STAT_DEF: val = 63  ; break;
                case PKMN_STAT_SPA: val = 91  ; break;
                case PKMN_STAT_SPD: val = 63  ; break;
                case PKMN_STAT_SPE: val = 68  ; break;
            };
        case PKMN_SPECIES_FARFETCHD:
            switch (stat) {
                case PKMN_STAT_HP : val = 52  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 58  ; break;
                case PKMN_STAT_SPD: val = 62  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_FEAROW:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 61  ; break;
                case PKMN_STAT_SPD: val = 61  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_FEEBAS:
            switch (stat) {
                case PKMN_STAT_HP : val = 20  ; break;
                case PKMN_STAT_ATK: val = 15  ; break;
                case PKMN_STAT_DEF: val = 20  ; break;
                case PKMN_STAT_SPA: val = 10  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_FERALIGATR:
            switch (stat) {
                case PKMN_STAT_HP : val = 85  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 79  ; break;
                case PKMN_STAT_SPD: val = 83  ; break;
                case PKMN_STAT_SPE: val = 78  ; break;
            };
        case PKMN_SPECIES_FLAAFFY:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_FLAREON:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_FLYGON:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_FORRETRESS:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 140 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_FURRET:
            switch (stat) {
                case PKMN_STAT_HP : val = 85  ; break;
                case PKMN_STAT_ATK: val = 76  ; break;
                case PKMN_STAT_DEF: val = 64  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_GARDEVOIR:
            switch (stat) {
                case PKMN_STAT_HP : val = 68  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 125 ; break;
                case PKMN_STAT_SPD: val = 115 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_GASTLY:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_GENGAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 130 ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_GEODUDE:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_GIRAFARIG:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_GLALIE:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_GLIGAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_GLOOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_GOLBAT:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_GOLDEEN:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 67  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 63  ; break;
            };
        case PKMN_SPECIES_GOLDUCK:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 82  ; break;
                case PKMN_STAT_DEF: val = 78  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_GOLEM:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 130 ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_GOREBYSS:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 84  ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 114 ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 52  ; break;
            };
        case PKMN_SPECIES_GRANBULL:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_GRAVELER:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 115 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_GRIMER:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_GROUDON:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 150 ; break;
                case PKMN_STAT_DEF: val = 140 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_GROVYLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_GROWLITHE:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_GRUMPIG:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_GULPIN:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 43  ; break;
                case PKMN_STAT_DEF: val = 53  ; break;
                case PKMN_STAT_SPA: val = 43  ; break;
                case PKMN_STAT_SPD: val = 53  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_GYARADOS:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 125 ; break;
                case PKMN_STAT_DEF: val = 79  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 81  ; break;
            };
        case PKMN_SPECIES_HARIYAMA:
            switch (stat) {
                case PKMN_STAT_HP : val = 144 ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_HAUNTER:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_HERACROSS:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 125 ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_HITMONCHAN:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 79  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 76  ; break;
            };
        case PKMN_SPECIES_HITMONLEE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 53  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 87  ; break;
            };
        case PKMN_SPECIES_HITMONTOP:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_HO_OH:
            switch (stat) {
                case PKMN_STAT_HP : val = 106 ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 154 ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_HOOTHOOT:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 36  ; break;
                case PKMN_STAT_SPD: val = 56  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_HOPPIP:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_HORSEA:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_HOUNDOOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_HOUNDOUR:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_HUNTAIL:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 104 ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 94  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 52  ; break;
            };
        case PKMN_SPECIES_HYPNO:
            switch (stat) {
                case PKMN_STAT_HP : val = 85  ; break;
                case PKMN_STAT_ATK: val = 73  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 73  ; break;
                case PKMN_STAT_SPD: val = 115 ; break;
                case PKMN_STAT_SPE: val = 67  ; break;
            };
        case PKMN_SPECIES_IGGLYBUFF:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 15  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 20  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_ILLUMISE:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 47  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 73  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_IVYSAUR:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 62  ; break;
                case PKMN_STAT_DEF: val = 63  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_JIGGLYPUFF:
            switch (stat) {
                case PKMN_STAT_HP : val = 115 ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 20  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_JIRACHI:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_JOLTEON:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 130 ; break;
            };
        case PKMN_SPECIES_JUMPLUFF:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_JYNX:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_KABUTO:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_KABUTOPS:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 115 ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_KADABRA:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 120 ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 105 ; break;
            };
        case PKMN_SPECIES_KAKUNA:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 25  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_KANGASKHAN:
            switch (stat) {
                case PKMN_STAT_HP : val = 105 ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_KECLEON:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 120 ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_KINGDRA:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_KINGLER:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 115 ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_KIRLIA:
            switch (stat) {
                case PKMN_STAT_HP : val = 38  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_KOFFING:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_KRABBY:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_KYOGRE:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 150 ; break;
                case PKMN_STAT_SPD: val = 140 ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_LAIRON:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 140 ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_LANTURN:
            switch (stat) {
                case PKMN_STAT_HP : val = 125 ; break;
                case PKMN_STAT_ATK: val = 58  ; break;
                case PKMN_STAT_DEF: val = 58  ; break;
                case PKMN_STAT_SPA: val = 76  ; break;
                case PKMN_STAT_SPD: val = 76  ; break;
                case PKMN_STAT_SPE: val = 67  ; break;
            };
        case PKMN_SPECIES_LAPRAS:
            switch (stat) {
                case PKMN_STAT_HP : val = 130 ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_LARVITAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 64  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 41  ; break;
            };
        case PKMN_SPECIES_LATIAS:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 130 ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_LATIOS:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 130 ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_LEDIAN:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_LEDYBA:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_LICKITUNG:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_LILEEP:
            switch (stat) {
                case PKMN_STAT_HP : val = 66  ; break;
                case PKMN_STAT_ATK: val = 41  ; break;
                case PKMN_STAT_DEF: val = 77  ; break;
                case PKMN_STAT_SPA: val = 61  ; break;
                case PKMN_STAT_SPD: val = 87  ; break;
                case PKMN_STAT_SPE: val = 23  ; break;
            };
        case PKMN_SPECIES_LINOONE:
            switch (stat) {
                case PKMN_STAT_HP : val = 78  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 61  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 61  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_LOMBRE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_LOTAD:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_LOUDRED:
            switch (stat) {
                case PKMN_STAT_HP : val = 84  ; break;
                case PKMN_STAT_ATK: val = 71  ; break;
                case PKMN_STAT_DEF: val = 43  ; break;
                case PKMN_STAT_SPA: val = 71  ; break;
                case PKMN_STAT_SPD: val = 43  ; break;
                case PKMN_STAT_SPE: val = 48  ; break;
            };
        case PKMN_SPECIES_LUDICOLO:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_LUGIA:
            switch (stat) {
                case PKMN_STAT_HP : val = 106 ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 130 ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 154 ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_LUNATONE:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_LUVDISC:
            switch (stat) {
                case PKMN_STAT_HP : val = 43  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 97  ; break;
            };
        case PKMN_SPECIES_MACHAMP:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_MACHOKE:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_MACHOP:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_MAGBY:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 37  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 83  ; break;
            };
        case PKMN_SPECIES_MAGCARGO:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 120 ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_MAGIKARP:
            switch (stat) {
                case PKMN_STAT_HP : val = 20  ; break;
                case PKMN_STAT_ATK: val = 10  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 15  ; break;
                case PKMN_STAT_SPD: val = 20  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_MAGMAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 57  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 93  ; break;
            };
        case PKMN_SPECIES_MAGNEMITE:
            switch (stat) {
                case PKMN_STAT_HP : val = 25  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_MAGNETON:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 120 ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_MAKUHITA:
            switch (stat) {
                case PKMN_STAT_HP : val = 72  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_MANECTRIC:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 105 ; break;
            };
        case PKMN_SPECIES_MANKEY:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_MANTINE:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 140 ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_MAREEP:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_MARILL:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_MAROWAK:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 110 ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_MARSHTOMP:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_MASQUERAIN:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 62  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 82  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_MAWILE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_MEDICHAM:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_MEDITITE:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_MEGANIUM:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 82  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 83  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_MEOWTH:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_METAGROSS:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 135 ; break;
                case PKMN_STAT_DEF: val = 130 ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_METANG:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_METAPOD:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_MEW:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_MEWTWO:
            switch (stat) {
                case PKMN_STAT_HP : val = 106 ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 154 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 130 ; break;
            };
        case PKMN_SPECIES_MIGHTYENA:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_MILOTIC:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 79  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 125 ; break;
                case PKMN_STAT_SPE: val = 81  ; break;
            };
        case PKMN_SPECIES_MILTANK:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 105 ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_MINUN:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_MISDREAVUS:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_MOLTRES:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 125 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_MR_MIME:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 120 ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_MUDKIP:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_MUK:
            switch (stat) {
                case PKMN_STAT_HP : val = 105 ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_MURKROW:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 42  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 42  ; break;
                case PKMN_STAT_SPE: val = 91  ; break;
            };
        case PKMN_SPECIES_NATU:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_NIDOKING:
            switch (stat) {
                case PKMN_STAT_HP : val = 81  ; break;
                case PKMN_STAT_ATK: val = 92  ; break;
                case PKMN_STAT_DEF: val = 77  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_NIDOQUEEN:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 82  ; break;
                case PKMN_STAT_DEF: val = 87  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 76  ; break;
            };
        case PKMN_SPECIES_NIDORAN_FEMALE:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 47  ; break;
                case PKMN_STAT_DEF: val = 52  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 41  ; break;
            };
        case PKMN_SPECIES_NIDORAN_MALE:
            switch (stat) {
                case PKMN_STAT_HP : val = 46  ; break;
                case PKMN_STAT_ATK: val = 57  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_NIDORINA:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 62  ; break;
                case PKMN_STAT_DEF: val = 67  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 56  ; break;
            };
        case PKMN_SPECIES_NIDORINO:
            switch (stat) {
                case PKMN_STAT_HP : val = 61  ; break;
                case PKMN_STAT_ATK: val = 72  ; break;
                case PKMN_STAT_DEF: val = 57  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_NINCADA:
            switch (stat) {
                case PKMN_STAT_HP : val = 31  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_NINETALES:
            switch (stat) {
                case PKMN_STAT_HP : val = 73  ; break;
                case PKMN_STAT_ATK: val = 76  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 81  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_NINJASK:
            switch (stat) {
                case PKMN_STAT_HP : val = 61  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 160 ; break;
            };
        case PKMN_SPECIES_NOCTOWL:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 76  ; break;
                case PKMN_STAT_SPD: val = 96  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_NOSEPASS:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 135 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_NUMEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_NUZLEAF:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_OCTILLERY:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_ODDISH:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_OMANYTE:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_OMASTAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 125 ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_ONIX:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 160 ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_PARAS:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_PARASECT:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_PELIPPER:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_PERSIAN:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 115 ; break;
            };
        case PKMN_SPECIES_PHANPY:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_PICHU:
            switch (stat) {
                case PKMN_STAT_HP : val = 20  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 15  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_PIDGEOT:
            switch (stat) {
                case PKMN_STAT_HP : val = 83  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 91  ; break;
            };
        case PKMN_SPECIES_PIDGEOTTO:
            switch (stat) {
                case PKMN_STAT_HP : val = 63  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 71  ; break;
            };
        case PKMN_SPECIES_PIDGEY:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 56  ; break;
            };
        case PKMN_SPECIES_PIKACHU:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_PILOSWINE:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_PINECO:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_PINSIR:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 125 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_PLUSLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_POLITOED:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_POLIWAG:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_POLIWHIRL:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_POLIWRATH:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_PONYTA:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_POOCHYENA:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_PORYGON:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_PORYGON2:
            switch (stat) {
                case PKMN_STAT_HP : val = 85  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_PRIMEAPE:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_PSYDUCK:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 52  ; break;
                case PKMN_STAT_DEF: val = 48  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_PUPITAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 84  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 51  ; break;
            };
        case PKMN_SPECIES_QUAGSIRE:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_QUILAVA:
            switch (stat) {
                case PKMN_STAT_HP : val = 58  ; break;
                case PKMN_STAT_ATK: val = 64  ; break;
                case PKMN_STAT_DEF: val = 58  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_QWILFISH:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_RAICHU:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_RAIKOU:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 115 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 115 ; break;
            };
        case PKMN_SPECIES_RALTS:
            switch (stat) {
                case PKMN_STAT_HP : val = 28  ; break;
                case PKMN_STAT_ATK: val = 25  ; break;
                case PKMN_STAT_DEF: val = 25  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_RAPIDASH:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 105 ; break;
            };
        case PKMN_SPECIES_RATICATE:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 81  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 97  ; break;
            };
        case PKMN_SPECIES_RATTATA:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 56  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 72  ; break;
            };
        case PKMN_SPECIES_RAYQUAZA:
            switch (stat) {
                case PKMN_STAT_HP : val = 105 ; break;
                case PKMN_STAT_ATK: val = 150 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 150 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_REGICE:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 200 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_REGIROCK:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 200 ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_REGISTEEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 150 ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 150 ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_RELICANTH:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 130 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_REMORAID:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_RHYDON:
            switch (stat) {
                case PKMN_STAT_HP : val = 105 ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 120 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_RHYHORN:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_ROSELIA:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SABLEYE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_SALAMENCE:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 135 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_SANDSHREW:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_SANDSLASH:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 110 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SCEPTILE:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 120 ; break;
            };
        case PKMN_SPECIES_SCIZOR:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SCYTHER:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 105 ; break;
            };
        case PKMN_SPECIES_SEADRA:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_SEAKING:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 92  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 68  ; break;
            };
        case PKMN_SPECIES_SEALEO:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_SEEDOT:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SEEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_SENTRET:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 46  ; break;
                case PKMN_STAT_DEF: val = 34  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_SEVIPER:
            switch (stat) {
                case PKMN_STAT_HP : val = 73  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SHARPEDO:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 120 ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_SHEDINJA:
            switch (stat) {
                case PKMN_STAT_HP : val = 1   ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_SHELGON:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_SHELLDER:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_SHIFTRY:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_SHROOMISH:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_SHUCKLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 20  ; break;
                case PKMN_STAT_ATK: val = 10  ; break;
                case PKMN_STAT_DEF: val = 230 ; break;
                case PKMN_STAT_SPA: val = 10  ; break;
                case PKMN_STAT_SPD: val = 230 ; break;
                case PKMN_STAT_SPE: val = 5   ; break;
            };
        case PKMN_SPECIES_SHUPPET:
            switch (stat) {
                case PKMN_STAT_HP : val = 44  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 63  ; break;
                case PKMN_STAT_SPD: val = 33  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_SILCOON:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_SKARMORY:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 140 ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_SKIPLOOM:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_SKITTY:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_SLAKING:
            switch (stat) {
                case PKMN_STAT_HP : val = 150 ; break;
                case PKMN_STAT_ATK: val = 160 ; break;
                case PKMN_STAT_DEF: val = 100 ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_SLAKOTH:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SLOWBRO:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 110 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SLOWKING:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SLOWPOKE:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_SLUGMA:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_SMEARGLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 75  ; break;
            };
        case PKMN_SPECIES_SMOOCHUM:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 15  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SNEASEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 115 ; break;
            };
        case PKMN_SPECIES_SNORLAX:
            switch (stat) {
                case PKMN_STAT_HP : val = 160 ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 110 ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SNORUNT:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_SNUBBULL:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SOLROCK:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_SPEAROW:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 31  ; break;
                case PKMN_STAT_SPD: val = 31  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_SPHEAL:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 25  ; break;
            };
        case PKMN_SPECIES_SPINARAK:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SPINDA:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_SPOINK:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 25  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_SQUIRTLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 44  ; break;
                case PKMN_STAT_ATK: val = 48  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 64  ; break;
                case PKMN_STAT_SPE: val = 43  ; break;
            };
        case PKMN_SPECIES_STANTLER:
            switch (stat) {
                case PKMN_STAT_HP : val = 73  ; break;
                case PKMN_STAT_ATK: val = 95  ; break;
                case PKMN_STAT_DEF: val = 62  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_STARMIE:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 115 ; break;
            };
        case PKMN_SPECIES_STARYU:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_STEELIX:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 200 ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SUDOWOODO:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 115 ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SUICUNE:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 115 ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 115 ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_SUNFLORA:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 105 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SUNKERN:
            switch (stat) {
                case PKMN_STAT_HP : val = 30  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 30  ; break;
            };
        case PKMN_SPECIES_SURSKIT:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 32  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 52  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_SWABLU:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_SWALOT:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 73  ; break;
                case PKMN_STAT_DEF: val = 83  ; break;
                case PKMN_STAT_SPA: val = 73  ; break;
                case PKMN_STAT_SPD: val = 83  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_SWAMPERT:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 110 ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_SWELLOW:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 125 ; break;
            };
        case PKMN_SPECIES_SWINUB:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 50  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_TAILLOW:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_TANGELA:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 115 ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_TAUROS:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 95  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 110 ; break;
            };
        case PKMN_SPECIES_TEDDIURSA:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_TENTACOOL:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_TENTACRUEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 120 ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_TOGEPI:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 20  ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_TOGETIC:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 40  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 80  ; break;
                case PKMN_STAT_SPD: val = 105 ; break;
                case PKMN_STAT_SPE: val = 40  ; break;
            };
        case PKMN_SPECIES_TORCHIC:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 60  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_TORKOAL:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 85  ; break;
                case PKMN_STAT_DEF: val = 140 ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_TOTODILE:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 64  ; break;
                case PKMN_STAT_SPA: val = 44  ; break;
                case PKMN_STAT_SPD: val = 48  ; break;
                case PKMN_STAT_SPE: val = 43  ; break;
            };
        case PKMN_SPECIES_TRAPINCH:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 100 ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 45  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 10  ; break;
            };
        case PKMN_SPECIES_TREECKO:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_TROPIUS:
            switch (stat) {
                case PKMN_STAT_HP : val = 99  ; break;
                case PKMN_STAT_ATK: val = 68  ; break;
                case PKMN_STAT_DEF: val = 83  ; break;
                case PKMN_STAT_SPA: val = 72  ; break;
                case PKMN_STAT_SPD: val = 87  ; break;
                case PKMN_STAT_SPE: val = 51  ; break;
            };
        case PKMN_SPECIES_TYPHLOSION:
            switch (stat) {
                case PKMN_STAT_HP : val = 78  ; break;
                case PKMN_STAT_ATK: val = 84  ; break;
                case PKMN_STAT_DEF: val = 78  ; break;
                case PKMN_STAT_SPA: val = 109 ; break;
                case PKMN_STAT_SPD: val = 85  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_TYRANITAR:
            switch (stat) {
                case PKMN_STAT_HP : val = 100 ; break;
                case PKMN_STAT_ATK: val = 134 ; break;
                case PKMN_STAT_DEF: val = 110 ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 61  ; break;
            };
        case PKMN_SPECIES_TYROGUE:
            switch (stat) {
                case PKMN_STAT_HP : val = 35  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 35  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 35  ; break;
            };
        case PKMN_SPECIES_UMBREON:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 110 ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 130 ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_UNOWN:
            switch (stat) {
                case PKMN_STAT_HP : val = 48  ; break;
                case PKMN_STAT_ATK: val = 72  ; break;
                case PKMN_STAT_DEF: val = 48  ; break;
                case PKMN_STAT_SPA: val = 72  ; break;
                case PKMN_STAT_SPD: val = 48  ; break;
                case PKMN_STAT_SPE: val = 48  ; break;
            };
        case PKMN_SPECIES_URSARING:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 130 ; break;
                case PKMN_STAT_DEF: val = 75  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_VAPOREON:
            switch (stat) {
                case PKMN_STAT_HP : val = 130 ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 110 ; break;
                case PKMN_STAT_SPD: val = 95  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_VENOMOTH:
            switch (stat) {
                case PKMN_STAT_HP : val = 70  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_VENONAT:
            switch (stat) {
                case PKMN_STAT_HP : val = 60  ; break;
                case PKMN_STAT_ATK: val = 55  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 40  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_VENUSAUR:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 82  ; break;
                case PKMN_STAT_DEF: val = 83  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 100 ; break;
                case PKMN_STAT_SPE: val = 80  ; break;
            };
        case PKMN_SPECIES_VIBRAVA:
            switch (stat) {
                case PKMN_STAT_HP : val = 50  ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_VICTREEBEL:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 105 ; break;
                case PKMN_STAT_DEF: val = 65  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 70  ; break;
            };
        case PKMN_SPECIES_VIGOROTH:
            switch (stat) {
                case PKMN_STAT_HP : val = 80  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_VILEPLUME:
            switch (stat) {
                case PKMN_STAT_HP : val = 75  ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 100 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_VOLBEAT:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 73  ; break;
                case PKMN_STAT_DEF: val = 55  ; break;
                case PKMN_STAT_SPA: val = 47  ; break;
                case PKMN_STAT_SPD: val = 75  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_VOLTORB:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 55  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_VULPIX:
            switch (stat) {
                case PKMN_STAT_HP : val = 38  ; break;
                case PKMN_STAT_ATK: val = 41  ; break;
                case PKMN_STAT_DEF: val = 40  ; break;
                case PKMN_STAT_SPA: val = 50  ; break;
                case PKMN_STAT_SPD: val = 65  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_WAILMER:
            switch (stat) {
                case PKMN_STAT_HP : val = 130 ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 70  ; break;
                case PKMN_STAT_SPD: val = 35  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_WAILORD:
            switch (stat) {
                case PKMN_STAT_HP : val = 170 ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 90  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_WALREIN:
            switch (stat) {
                case PKMN_STAT_HP : val = 110 ; break;
                case PKMN_STAT_ATK: val = 80  ; break;
                case PKMN_STAT_DEF: val = 90  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 65  ; break;
            };
        case PKMN_SPECIES_WARTORTLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 59  ; break;
                case PKMN_STAT_ATK: val = 63  ; break;
                case PKMN_STAT_DEF: val = 80  ; break;
                case PKMN_STAT_SPA: val = 65  ; break;
                case PKMN_STAT_SPD: val = 80  ; break;
                case PKMN_STAT_SPE: val = 58  ; break;
            };
        case PKMN_SPECIES_WEEDLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 35  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 20  ; break;
                case PKMN_STAT_SPE: val = 50  ; break;
            };
        case PKMN_SPECIES_WEEPINBELL:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 50  ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
        case PKMN_SPECIES_WEEZING:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 120 ; break;
                case PKMN_STAT_SPA: val = 85  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_WHISCASH:
            switch (stat) {
                case PKMN_STAT_HP : val = 110 ; break;
                case PKMN_STAT_ATK: val = 78  ; break;
                case PKMN_STAT_DEF: val = 73  ; break;
                case PKMN_STAT_SPA: val = 76  ; break;
                case PKMN_STAT_SPD: val = 71  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_WHISMUR:
            switch (stat) {
                case PKMN_STAT_HP : val = 64  ; break;
                case PKMN_STAT_ATK: val = 51  ; break;
                case PKMN_STAT_DEF: val = 23  ; break;
                case PKMN_STAT_SPA: val = 51  ; break;
                case PKMN_STAT_SPD: val = 23  ; break;
                case PKMN_STAT_SPE: val = 28  ; break;
            };
        case PKMN_SPECIES_WIGGLYTUFF:
            switch (stat) {
                case PKMN_STAT_HP : val = 140 ; break;
                case PKMN_STAT_ATK: val = 70  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 50  ; break;
                case PKMN_STAT_SPE: val = 45  ; break;
            };
        case PKMN_SPECIES_WINGULL:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 30  ; break;
                case PKMN_STAT_SPA: val = 55  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 85  ; break;
            };
        case PKMN_SPECIES_WOBBUFFET:
            switch (stat) {
                case PKMN_STAT_HP : val = 190 ; break;
                case PKMN_STAT_ATK: val = 33  ; break;
                case PKMN_STAT_DEF: val = 58  ; break;
                case PKMN_STAT_SPA: val = 33  ; break;
                case PKMN_STAT_SPD: val = 58  ; break;
                case PKMN_STAT_SPE: val = 33  ; break;
            };
        case PKMN_SPECIES_WOOPER:
            switch (stat) {
                case PKMN_STAT_HP : val = 55  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 25  ; break;
                case PKMN_STAT_SPD: val = 25  ; break;
                case PKMN_STAT_SPE: val = 15  ; break;
            };
        case PKMN_SPECIES_WURMPLE:
            switch (stat) {
                case PKMN_STAT_HP : val = 45  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 20  ; break;
                case PKMN_STAT_SPD: val = 30  ; break;
                case PKMN_STAT_SPE: val = 20  ; break;
            };
        case PKMN_SPECIES_WYNAUT:
            switch (stat) {
                case PKMN_STAT_HP : val = 95  ; break;
                case PKMN_STAT_ATK: val = 23  ; break;
                case PKMN_STAT_DEF: val = 48  ; break;
                case PKMN_STAT_SPA: val = 23  ; break;
                case PKMN_STAT_SPD: val = 48  ; break;
                case PKMN_STAT_SPE: val = 23  ; break;
            };
        case PKMN_SPECIES_XATU:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 75  ; break;
                case PKMN_STAT_DEF: val = 70  ; break;
                case PKMN_STAT_SPA: val = 95  ; break;
                case PKMN_STAT_SPD: val = 70  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_YANMA:
            switch (stat) {
                case PKMN_STAT_HP : val = 65  ; break;
                case PKMN_STAT_ATK: val = 65  ; break;
                case PKMN_STAT_DEF: val = 45  ; break;
                case PKMN_STAT_SPA: val = 75  ; break;
                case PKMN_STAT_SPD: val = 45  ; break;
                case PKMN_STAT_SPE: val = 95  ; break;
            };
        case PKMN_SPECIES_ZANGOOSE:
            switch (stat) {
                case PKMN_STAT_HP : val = 73  ; break;
                case PKMN_STAT_ATK: val = 115 ; break;
                case PKMN_STAT_DEF: val = 60  ; break;
                case PKMN_STAT_SPA: val = 60  ; break;
                case PKMN_STAT_SPD: val = 60  ; break;
                case PKMN_STAT_SPE: val = 90  ; break;
            };
        case PKMN_SPECIES_ZAPDOS:
            switch (stat) {
                case PKMN_STAT_HP : val = 90  ; break;
                case PKMN_STAT_ATK: val = 90  ; break;
                case PKMN_STAT_DEF: val = 85  ; break;
                case PKMN_STAT_SPA: val = 125 ; break;
                case PKMN_STAT_SPD: val = 90  ; break;
                case PKMN_STAT_SPE: val = 100 ; break;
            };
        case PKMN_SPECIES_ZIGZAGOON:
            switch (stat) {
                case PKMN_STAT_HP : val = 38  ; break;
                case PKMN_STAT_ATK: val = 30  ; break;
                case PKMN_STAT_DEF: val = 41  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 41  ; break;
                case PKMN_STAT_SPE: val = 60  ; break;
            };
        case PKMN_SPECIES_ZUBAT:
            switch (stat) {
                case PKMN_STAT_HP : val = 40  ; break;
                case PKMN_STAT_ATK: val = 45  ; break;
                case PKMN_STAT_DEF: val = 35  ; break;
                case PKMN_STAT_SPA: val = 30  ; break;
                case PKMN_STAT_SPD: val = 40  ; break;
                case PKMN_STAT_SPE: val = 55  ; break;
            };
    };
    return val;
}

uint32_t
pkmn_calculate_exp(enum Pokedex species, unsigned short level) {
    const enum PkmnLevellingRate rate = pkmn_get_levelling_rate(species);

    if (level == 0)  level = 1;
    if (level > 100) level = 100;

    uint32_t exp = 0;
    switch (rate) {
        case PKMN_LEVELLING_RATE_ERRATIC:
            if      (level < 50) exp = floor((pow(level, 3) * (100.0 - level)) / 50.0);
            else if (level < 68) exp = floor((pow(level, 3) * (150.0 - level)) / 100.0);
            // TODO below is producing wrong values?
            else if (level < 98) exp = floor(((pow(level, 3) * ((1911.0 - (10.0 * level)) / 3.0)) / 500.0));
            else                 exp = floor((pow(level, 3) * (160.0 - level)) / 100.0);
            break;

        case PKMN_LEVELLING_RATE_FAST:
            exp = floor((4.0 * (pow(level, 3))) / 5.0); break;

        case PKMN_LEVELLING_RATE_FLUCTUATING:
            if      (level < 15)   exp = floor((pow(level, 3.0) * (((level + 1) / 3.0) + 24.0)) / 50.0);
            else if (level < 36)   exp = floor(((pow(level, 3.0) * (level + 14.0)) / 50.0));
            // TODO below returns incorrect values
            else if (level <= 100) exp = floor((pow(level, 3) + ((level / 2.0) + 32.0) / 50.0));
            break;

        case PKMN_LEVELLING_RATE_MEDIUM_FAST:
            exp = floor(pow(level, 3)); break;

        case PKMN_LEVELLING_RATE_MEDIUM_SLOW:
            exp = floor((6.0/5.0) * pow(level, 3) - 15.0 * pow(level, 2) + 100.0 * level - 140.0); break;

        case PKMN_LEVELLING_RATE_SLOW:
            exp = floor((5.0 * pow(level, 3)) / 4); break;
    };
    return exp;
}

/*
 * Calculate the 8-bit 'pp bonuses' value of a Pokemon.
 * This number represents how many PP boosts (think up to 3x PP-Ups or 1x PP-Max)
 * that a Pokemon's moves have.
 *
 * see: https://bulbapedia.bulbagarden.net/wiki/Pok%C3%A9mon_data_substructures_(Generation_III)#PP_bonuses
 */
uint8_t
pkmn_calculate_pp_bonuses(
    unsigned short move1bonuses,
    unsigned short move2bonuses,
    unsigned short move3bonuses,
    unsigned short move4bonuses
) {
    // Clamp if bonuses > 3 as that is the max value
    if (move1bonuses > 3) move1bonuses = 3;
    if (move2bonuses > 3) move2bonuses = 3;
    if (move3bonuses > 3) move3bonuses = 3;
    if (move4bonuses > 3) move4bonuses = 3;

    uint8_t pp_bonuses = move1bonuses;

    switch (move2bonuses) {
        case 1:  pp_bonuses += 4;  break;
        case 2:  pp_bonuses += 8;  break;
        case 3:  pp_bonuses += 12; break;
        default: break;
    };
    switch (move3bonuses) {
        case 1:  pp_bonuses += 16; break;
        case 2:  pp_bonuses += 32; break;
        case 3:  pp_bonuses += 48; break;
        default: break;
    };
    switch (move4bonuses) {
        case 1:  pp_bonuses += 64;  break;
        case 2:  pp_bonuses += 128; break;
        case 3:  pp_bonuses += 192; break;
        default: break;
    };
    return pp_bonuses;
}

/*
 * Calculate the multiplier used in stat calculations.
 *
 * A nature affects stat growth by acting as a multiplier when the stat value is calculated.
 * Natures that aren't "neutral" (that don't boost or reduce any stat) has a multiplier of just 1.0.
 *
 * For other natures, the multiplier depends on the stat we're trying to calculate.
 * One stat will be 1.1 and another 0.9.
*/
double pkmn_calculate_nature_multiplier(const enum PkmnNature nature, const enum PkmnStat stat) {
    double mul = 1.0;

    switch (nature) {
        case PKMN_NATURE_ADAMANT:
            switch (stat) {
                case PKMN_STAT_ATK: mul = 1.1; break;
                case PKMN_STAT_SPA: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_BOLD:
            switch (stat) {
                case PKMN_STAT_DEF: mul = 1.1; break;
                case PKMN_STAT_ATK: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_BRAVE:
            switch (stat) {
                case PKMN_STAT_ATK: mul = 1.1; break;
                case PKMN_STAT_SPE: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_CALM:
            switch (stat) {
                case PKMN_STAT_SPD: mul = 1.1; break;
                case PKMN_STAT_ATK: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_CAREFUL:
            switch (stat) {
                case PKMN_STAT_SPD: mul = 1.1; break;
                case PKMN_STAT_SPA: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_GENTLE:
            switch (stat) {
                case PKMN_STAT_SPD: mul = 1.1; break;
                case PKMN_STAT_DEF: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_HASTY:
            switch (stat) {
                case PKMN_STAT_SPE: mul = 1.1; break;
                case PKMN_STAT_DEF: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_IMPISH:
            switch (stat) {
                case PKMN_STAT_DEF: mul = 1.1; break;
                case PKMN_STAT_SPA: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_JOLLY:
            switch (stat) {
                case PKMN_STAT_SPE: mul = 1.1; break;
                case PKMN_STAT_SPA: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_LAX:
            switch (stat) {
                case PKMN_STAT_DEF: mul = 1.1; break;
                case PKMN_STAT_SPD: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_LONELY:
            switch (stat) {
                case PKMN_STAT_ATK: mul = 1.1; break;
                case PKMN_STAT_DEF: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_MILD:
            switch (stat) {
                case PKMN_STAT_SPA: mul = 1.1; break;
                case PKMN_STAT_DEF: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_MODEST:
            switch (stat) {
                case PKMN_STAT_SPA: mul = 1.1; break;
                case PKMN_STAT_ATK: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_NAIVE:
            switch (stat) {
                case PKMN_STAT_SPE: mul = 1.1; break;
                case PKMN_STAT_SPD: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_NAUGHTY:
            switch (stat) {
                case PKMN_STAT_ATK: mul = 1.1; break;
                case PKMN_STAT_SPD: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_QUIET:
            switch (stat) {
                case PKMN_STAT_SPA: mul = 1.1; break;
                case PKMN_STAT_SPE: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_RASH:
            switch (stat) {
                case PKMN_STAT_SPA: mul = 1.1; break;
                case PKMN_STAT_SPD: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_RELAXED:
            switch (stat) {
                case PKMN_STAT_DEF: mul = 1.1; break;
                case PKMN_STAT_SPE: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_SASSY:
            switch (stat) {
                case PKMN_STAT_SPD: mul = 1.1; break;
                case PKMN_STAT_SPE: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        case PKMN_NATURE_TIMID:
            switch (stat) {
                case PKMN_STAT_SPE: mul = 1.1; break;
                case PKMN_STAT_ATK: mul = 0.9; break;
                default:            mul = 1.0; break;
            };
            break;

        // Some natures give stat boosts and reductions to the same stat,
        // effecively making their multiplier 1.
        case PKMN_NATURE_BASHFUL : // sp.atk
        case PKMN_NATURE_DOCILE  : // def
        case PKMN_NATURE_HARDY   : // atk
        case PKMN_NATURE_QUIRKY  : // sp.def
        case PKMN_NATURE_SERIOUS : // speed
            mul = 1.0;
            break;

    };

    return mul;
}

uint16_t pkmn_calculate_stat(enum PkmnStat stat, enum Pokedex species, uint8_t ev, uint8_t iv, uint8_t level, enum PkmnNature nature) {
    uint16_t calc_stat;

    const uint8_t base              = pkmn_get_base_stat_value(species, stat);
    const double  nature_multiplier = pkmn_calculate_nature_multiplier(nature, stat);

    switch(stat) {
        case PKMN_STAT_HP:
            // Shedinja always has 1 HP.
            if (species == PKMN_SPECIES_SHEDINJA) {
                calc_stat = 1;
            } else {
                calc_stat = (((2.0 * base + iv + (ev / 4.0)) * level) / 100) + level + 10;
            }
            break;

        case PKMN_STAT_ATK:
        case PKMN_STAT_DEF:
        case PKMN_STAT_SPA:
        case PKMN_STAT_SPD:
        case PKMN_STAT_SPE:
            calc_stat = (((((2.0 * base + iv + (ev / 4.0)) * level) / 100) + 5) * nature_multiplier);
            break;

        default:
            calc_stat = 0;
            break;
    };

    return calc_stat;
}

/*
 * The substructure order, `o`, is determined by the Pokemon's personality value mod 24.
 *
 * The value of `o` corresponds to a particular order represented in the enum PkmnSubstructure.
 */
unsigned
pkmn_get_substructure_order(const Pokemon *p) {
    return p->personalityValue % 24;
}

static unsigned long
pkmn_rand_u32() {
    if (RANDOM_INITIALISED == 0) {
        srandom(time(NULL));
        RANDOM_INITIALISED = 1;
    };
    const long rnd = random();
    return rnd + ((rnd % 16) << 32);
}

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
) {
    unsigned long pval = pkmn_rand_u32();
    while ((pval % 25) != nature) { pval = pkmn_rand_u32(); }  // TODO this is very inefficient
    printf("Found pval %lu where mod 25 == %u\n", pval, nature);
    p->personalityValue = pval;

    p->otId = (otId == PKMN_INIT_RANDOM) ? pkmn_rand_u32() : otId;
    // assume nickname is already encoded
    memcpy(p->nickname, nickname, PKMN_NICKNAME_LEN);
    switch (lang) {
        case PKMN_LANG_DE:
        case PKMN_LANG_EN:
        case PKMN_LANG_ES:
        case PKMN_LANG_FR:
        case PKMN_LANG_IT:
        case PKMN_LANG_JP:
            p->language = lang;
        break;
        default:
            fprintf(stderr, "Unknown language specified: %u (defaulting to EN)", lang);
        p->language = PKMN_LANG_EN;
        break;
    }
    // TODO support for misc flag
    p->miscFlags = 2;
    memcpy(p->otName, otName, PKMN_OT_NAME_LEN);
    // TODO markings

    PokemonDataType pd = {};
    if (level > 100) level = 100;
    pd.G.species    = species;
    pd.G.exp        = pkmn_calculate_exp(species, level);
    pd.G.heldItem   = heldItem;
    pd.G.ppBonuses  = pkmn_calculate_pp_bonuses(move1bonuses, move2bonuses, move3bonuses, move4bonuses);
    pd.G.friendship = friendship;

    pd.A.move1   = move1;
    pd.A.move2   = move2;
    pd.A.move3   = move3;
    pd.A.move4   = move4;
    pd.A.ppMove1 = pkmn_get_move_pp(move1);
    pd.A.ppMove2 = pkmn_get_move_pp(move2);
    pd.A.ppMove3 = pkmn_get_move_pp(move3);
    pd.A.ppMove4 = pkmn_get_move_pp(move4);

    pkmn_init_evs(&pd.E, hpEv, atkEv, defEv, spAtkEv, spDefEv, speedEv);

    pd.E.beauty    = contestBeauty;
    pd.E.coolness  = contestCoolness;
    pd.E.cuteness  = contestCuteness;
    pd.E.smartness = contestSmartness;
    pd.E.toughness = contestToughness;

    uint8_t pokerusData = 0;
    if (pokerusDays > 0) {
        if (pokerusDays > 4) pokerusDays = 4;

        pokerusData += pokerusDays;
        pokerusData += 240; // "strain" (fixed value)
    }
    pd.M.pokerus      = pokerusData;
    pd.M.metWhere     = metLocation;

    // Clamp IVs if they exceed the max value.
    if (hpIv > 31)    hpIv    = 31;
    if (atkIv > 31)   atkIv   = 31;
    if (defIv > 31)   defIv   = 31;
    if (spAtkIv > 31) spAtkIv = 31;
    if (spDefIv > 31) spDefIv = 31;
    if (speedIv > 31) speedIv = 31;

    // Each IV is 5 bits, so 30 bits in total are used.
    uint32_t ivEggAbilityValue = 0;
    ivEggAbilityValue += hpIv;    ivEggAbilityValue <<= 5;
    ivEggAbilityValue += atkIv;   ivEggAbilityValue <<= 5;
    ivEggAbilityValue += defIv;   ivEggAbilityValue <<= 5;
    ivEggAbilityValue += speedIv; ivEggAbilityValue <<= 5;
    ivEggAbilityValue += spAtkIv; ivEggAbilityValue <<= 5;
    ivEggAbilityValue += spDefIv; ivEggAbilityValue <<= 5;

    // The remaining two bits, E and A, signify whether the mon is an egg, and what ability it has.
    // Bit E is set if the mon IS an egg.  TODO support eggs?
    // Bit A is set to 0 if the mon has its primary ability. It is set to 1 if it has its secondary ability.
    // NOTE Not every mon has a secondary ability, so we need to check the validity of this bit.
    ivEggAbilityValue <<= 2;

    pd.M.ivEggAbility = ivEggAbilityValue;

    // TODO only do encrypt/decrypt and write back when we're done initialising everything.
    memcpy(&p->data, &pd, sizeof(pd));
    pkmn_init_inner_data(&pd, p);

    uint16_t checksum = pkmn_calculate_checksum(&pd);
    p->checksum = checksum;

    uint32_t key = pkmn_get_inner_data_decryption_key(p);
    PokemonDataBlock buf = {};
    pkmn_encrypt_decrypt_inner_data(&pd, (PokemonDataType *)&buf, key);
    pkmn_write_inner_data(p, (PokemonDataType *)&buf);

    // NOTE :
    //  all 100 bytes are used, so stats must be calculated, presumably /correctly/
    //  as we currently get crashes when attempting to view stats.
    //  the hard coding to 50/15/whatever below does not work, presumably because the
    //  game is checking the stats agree with the EVs (if I had to guess?)
    //
    //  HOWEVER the complication here is that stat calculations involve IVs which we need
    //  to determine / set on the mon. yay.
    // TODO proper stat calculations
    // TODO the above is implemented but requires base stat calcs.
    // TODO we should also implement a function that maps the valid abilities to each pokemon.
    //      note that hidden abilities aren't used in gen 3 (they were new in gen 5)
    //      and not every mon has 2 abilities. most only have 1.
    uint16_t hpVal = pkmn_calculate_stat(PKMN_STAT_HP, species, hpEv, hpIv, level, nature);
    p->totalHp   = hpVal;
    p->currentHp = hpVal;

    p->attackStat         = pkmn_calculate_stat(PKMN_STAT_ATK, species, atkEv, atkIv, level, nature);
    p->defenseStat        = pkmn_calculate_stat(PKMN_STAT_DEF, species, defEv, defIv, level, nature);
    p->specialAttackStat  = pkmn_calculate_stat(PKMN_STAT_SPA, species, spAtkEv, spAtkIv, level, nature);
    p->specialDefenseStat = pkmn_calculate_stat(PKMN_STAT_SPD, species, spDefEv, spDefIv, level, nature);
    p->speedStat          = pkmn_calculate_stat(PKMN_STAT_SPE, species, speedEv, speedIv, level, nature);

    p->level = level;
    p->mailId = 0xFF;
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
        fprintf(stderr, "cur: %#X / word: %#X / pos: %p\n", ptr[i], calc, &ptr[i]);
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
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_GAME:
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_GEAM:
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_GEMA:
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_GMAE:
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_GMEA:
            memcpy((unsigned char*)&p->data[0],  &pdt->G, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_AGEM:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_AGME:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_AEGM:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_AEMG:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_AMGE:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_AMEG:
            memcpy((unsigned char*)&p->data[0],  &pdt->A, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_EGAM:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_EGMA:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_EAGM:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->M, 12);
            break;
        case PKMN_DATA_ORDER_EAMG:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_EMGA:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_EMAG:
            memcpy((unsigned char*)&p->data[0],  &pdt->E, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->M, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_MGAE:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_MGEA:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_MAGE:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->E, 12);
            break;
        case PKMN_DATA_ORDER_MAEG:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        case PKMN_DATA_ORDER_MEGA:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->G, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->A, 12);
            break;
        case PKMN_DATA_ORDER_MEAG:
            memcpy((unsigned char*)&p->data[0],  &pdt->M, 12);
            memcpy((unsigned char*)&p->data[12], &pdt->E, 12);
            memcpy((unsigned char*)&p->data[24], &pdt->A, 12);
            memcpy((unsigned char*)&p->data[36], &pdt->G, 12);
            break;
        default:
            break;
    }
};
