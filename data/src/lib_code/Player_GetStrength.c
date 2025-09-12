#include <uLib.h>

// z64ram = 0x8008F034
// z64rom = 0xB061D4
// z64next = 0x8008F080

s32 Player_GetStrength(void) {
    s32 strengthUpgrade = CUR_UPG_VALUE(UPG_STRENGTH);

    return strengthUpgrade;

/*    if (LINK_IS_ADULT) {
         return strengthUpgrade;
    } else if (strengthUpgrade != 0) {
        return PLAYER_STR_BRACELET;
    } else {
        return PLAYER_STR_NONE;
    }*/
}