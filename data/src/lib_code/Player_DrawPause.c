#include <uLib.h>
#include "code/z_player_lib.h"
#include "z64player.h"

// z64ram = 0x8009214C
// z64rom = 0xB092EC
// z64next = 0x80092320

void Player_DrawPause(PlayState* play, u8* segment, SkelAnime* skelAnime, Vec3f* pos, Vec3s* rot, f32 scale, s32 sword,
                      s32 tunic, s32 shield, s32 boots) {
    static Vec3f eye = { 0.0f, 0.0f, -400.0f };
    static Vec3f at = { 0.0f, 0.0f, 0.0f };
    Vec3s* destTable;
    Vec3s* srcTable;
    s32 i;

    gSegments[4] = VIRTUAL_TO_PHYSICAL(segment + PAUSE_EQUIP_BUFFER_SIZE);
    gSegments[6] = VIRTUAL_TO_PHYSICAL(segment + PAUSE_EQUIP_BUFFER_SIZE + PAUSE_PLAYER_SEGMENT_GAMEPLAY_KEEP_BUFFER_SIZE);

    // Changed to facilitate no turtle shield for child on pause menu
    if (!LINK_IS_ADULT) {
            srcTable = gLinkPauseChildJointTable;
    } else {
        if (sword == PLAYER_SWORD_BGS) {
            srcTable = gLinkPauseAdultBgsJointTable;
        } else if (shield != PLAYER_SHIELD_NONE) {
            srcTable = gLinkPauseAdultShieldJointTable;
        } else {
            srcTable = gLinkPauseAdultJointTable;
        }
    }

    srcTable = SEGMENTED_TO_VIRTUAL(srcTable);
    destTable = skelAnime->jointTable;
    for (i = 0; i < skelAnime->limbCount; i++) {
        *destTable++ = *srcTable++;
    }

    Player_DrawPauseImpl(play, segment + PAUSE_EQUIP_BUFFER_SIZE,
                         segment + PAUSE_EQUIP_BUFFER_SIZE + PAUSE_PLAYER_SEGMENT_GAMEPLAY_KEEP_BUFFER_SIZE, skelAnime,
                         pos, rot, scale, sword, tunic, shield, boots, PAUSE_EQUIP_PLAYER_WIDTH,
                         PAUSE_EQUIP_PLAYER_HEIGHT, &eye, &at, 60.0f, play->state.gfxCtx->curFrameBuffer,
                         play->state.gfxCtx->curFrameBuffer + (PAUSE_EQUIP_PLAYER_WIDTH * PAUSE_EQUIP_PLAYER_HEIGHT));
}