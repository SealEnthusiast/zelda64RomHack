#include <z64hdr.h>
#include "code/z_player_lib.h"
#include "functions.h"
#include "playas/playas.h"

void Player_DrawNew(PlayState* play, void** skeleton, Vec3s* jointTable, s32 dListCount, s32 lod, s32 tunic, s32 boots,
                     s32 face, OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, void* data) {
    Color_RGB8* color;
    s32 eyeIndex = (jointTable[22].x & 0xF) - 1;
    s32 mouthIndex = (jointTable[22].x >> 4) - 1;

        //Adult
    Gfx* hackAdultBootDLists[] = {
        gPlayasAdult_DlFootIronL,
        gPlayasAdult_DlFootHoverL, 
        gPlayasAdult_DlFootIronR,
        gPlayasAdult_DlFootHoverR,
    };

    //Child
    Gfx* hackChildBootDLists[] = {
        gPlayasChild_DlFootIronL,
        gPlayasChild_DlFootHoverL,
        gPlayasChild_DlFootIronR,
        gPlayasChild_DlFootHoverR,
    };

    OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 1721);

    if (eyeIndex < 0) {
        eyeIndex = sEyeMouthIndices[face][0];
    }

#ifndef AVOID_UB
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[eyeIndex]));
#else
    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(sEyeTextures[gSaveContext.linkAge][eyeIndex]));
#endif

    if (mouthIndex < 0) {
        mouthIndex = sEyeMouthIndices[face][1];
    }

#ifndef AVOID_UB
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[mouthIndex]));
#else
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(sMouthTextures[gSaveContext.linkAge][mouthIndex]));
#endif
    color = &sTunicColors[tunic];
    gDPSetEnvColor(POLY_OPA_DISP++, color->r, color->g, color->b, 0);

    sDListsLodOffset = lod * 2;
    SkelAnime_DrawFlexLod(play, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, data, lod);

    if ((overrideLimbDraw != Player_OverrideLimbDrawGameplayFirstPerson) &&
        (overrideLimbDraw != Player_OverrideLimbDrawGameplay_80090440) &&
        (gSaveContext.gameMode != GAMEMODE_END_CREDITS)) {

        if (boots != PLAYER_BOOTS_KOKIRI) {
            Gfx** bootDListsToUse = LINK_IS_ADULT ? hackAdultBootDLists : hackChildBootDLists;

            gSPDisplayList(POLY_OPA_DISP++, bootDListsToUse[boots - 1]);
            gSPDisplayList(POLY_OPA_DISP++, bootDListsToUse[boots + 1]);
        }
    }
    CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 1803);
}