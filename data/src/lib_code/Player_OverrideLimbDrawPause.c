#include <uLib.h>
#include "code/z_player_lib.h"

// z64ram = 0x80091880
// z64rom = 0xB08A20
// z64next = 0x80091A24

s32 Player_OverrideLimbDrawPause(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void* arg) {
    u8* playerSwordAndShield = arg;
    u8 modelGroup = sPauseModelGroupBySword[playerSwordAndShield[0] - PLAYER_SWORD_KOKIRI];

    //PLAYER_MODELGROUP_0 is replaced to allow adult kokiri sword so new pointer to different modelgroup using no sword.
    if (playerSwordAndShield[0] == PLAYER_SWORD_NONE) {
        modelGroup = PLAYER_MODELGROUP_DEFAULT;
    }

    s32 type;
    s32 dListOffset = 0;
    Gfx** dLists;

    if (limbIndex == PLAYER_LIMB_L_HAND) {
        type = gPlayerModelTypes[modelGroup][PLAYER_MODELGROUPENTRY_LEFT_HAND];
        sLeftHandType = type;
        if ((type == PLAYER_MODELTYPE_LH_BGS) && (gSaveContext.swordHealth <= 0.0f)) {
            dListOffset = 4;
        }
    } else if (limbIndex == PLAYER_LIMB_R_HAND) {
        type = gPlayerModelTypes[modelGroup][PLAYER_MODELGROUPENTRY_RIGHT_HAND];
        sRightHandType = type;
        if (type == PLAYER_MODELTYPE_RH_SHIELD) {
            dListOffset = playerSwordAndShield[1] * 4;
        }
    } else if (limbIndex == PLAYER_LIMB_SHEATH) {
        type = gPlayerModelTypes[modelGroup][PLAYER_MODELGROUPENTRY_SHEATH];
        if ((type == PLAYER_MODELTYPE_SHEATH_18) || (type == PLAYER_MODELTYPE_SHEATH_19)) {
            dListOffset = playerSwordAndShield[1] * 4;
        }
    } else if (limbIndex == PLAYER_LIMB_WAIST) {
        type = gPlayerModelTypes[modelGroup][PLAYER_MODELGROUPENTRY_WAIST];
    } else {
        return false;
    }

    dLists = sPlayerDListGroups[type] + ((void)0, gSaveContext.linkAge);
    *dList = *(dLists + dListOffset);

    return false;
}