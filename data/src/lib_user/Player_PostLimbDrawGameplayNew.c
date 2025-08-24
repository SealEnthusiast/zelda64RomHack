#include "code/z_player_lib.h"
#include "playas/playas.h"
#include "uLib.h"
#include "z64player.h"
#include "../system/kaleido/0x01-Player/playas_adult.h"
#include "../system/kaleido/0x01-Player/playas_child.h"

void Player_PostLimbDrawGameplayNew(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* thisx) {
    Player* this = (Player*)thisx;

    Gfx* hackStringDLists[]= {
        gPlayasAdult_DlStringBow,
        gPlayasAdult_DlStringSlingshot,
        gPlayasChild_DlStringBow,
        gPlayasChild_DlStringSlingshot,
    };

    Vec3f hackStringPos[] = {
        { 0.0f,  -360.4f, 0.0f }, // Adult Bow
        { 750.0f, 400.0f, 0.0f }, // Adult Slingshot  (up/down,towards/away from player, left/right)
        { 0.0f,  -360.4f, 0.0f }, // Child Bow
        { 606.0f, 236.0f, 0.0f }, // Child Slingshot
    };

    Vec3f childArrowPos = { 520.0f, 1470.0f, 740.0f }; //(up/down,towards/away from player, left/right)

    Vec3f Hookshot1 = { -80.0f, 1500.0f, 0.0f }; //(up/down,towards/away from player, left/right)
    Vec3f Hookshot2 = { -80.0f, 1640.0f, 0.0f }; //(up/down,towards/away from player, left/right)

    u8 rangedStringSelection = 0;

    Camera cam = *Play_GetCamera(play,CAM_ID_MAIN);

    if (*dList != NULL) {
        Matrix_MultVec3f(&sZeroVec, sCurBodyPartPos);
    }

    //hack gauntlet/bracelets not in first person view
    if (CUR_UPG_VALUE(UPG_STRENGTH) != 0 && cam.mode != 6 && cam.mode != 7 && cam.mode != 11){
        DrawGauntletDList(play,this,limbIndex);
    }

    if (limbIndex == PLAYER_LIMB_L_HAND) {
        MtxF sp14C;
        Actor* hookedActor;

        Math_Vec3f_Copy(&this->leftHandPos, sCurBodyPartPos);

        if (this->itemActionParam == PLAYER_AP_STICK) {
            Vec3f sp124[3];

            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2633);

            if (this->actor.scale.y >= 0.0f) {
                D_80126080.x = this->unk_85C * 5000.0f;
                func_80090A28(this, sp124);

                if (this->meleeWeaponState != 0) {
                    func_800906D4(play, this, sp124);
                } else {
                    Math_Vec3f_Copy(&this->meleeWeaponInfo[0].tip, &sp124[0]);
                }
            }

            //Shrinking of stick when burning down
            if (this->unk_85C > 0.5f) {
                Matrix_Scale(this->unk_85C, 1.0f, 1.0f, MTXMODE_APPLY);
            }

            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2653),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            //Stick Display Lists
            //D_80126080.x = 2500.0f; //Used to assign length manually if changed
            if (LINK_IS_ADULT) {
                if (this->unk_85C > 0.5f) {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasAdult_DlDekuStick);
                } else {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasAdult_DlDekuStickBrokenHeld);
                }

            } else {
                if (this->unk_85C > 0.5f) {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasChild_DlDekuStick);
                } else {
                    gSPDisplayList(POLY_OPA_DISP++, gPlayasChild_DlDekuStickBrokenHeld);
                }
            }

            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2656);
        } else if ((this->actor.scale.y >= 0.0f) && (this->meleeWeaponState != 0)) {
            Vec3f spE4[3];

            if (this->heldItemActionParam != PLAYER_AP_STICK){
                if (Player_HoldsBrokenKnife(this)){
                    D_80126080.x = 1500.0f;
                } else {
                    D_80126080.x = sMeleeWeaponLengths[Player_GetMeleeWeaponHeld(this)];
                }
            }

            func_80090A28(this, spE4);
            func_800906D4(play, this, spE4);
        } else if ((*dList != NULL) && (this->leftHandType == PLAYER_MODELTYPE_LH_BOTTLE)) {

            // fixes a vanilla bug for shielding with an empty bottle showing a bottle color out of range
            s8 bottlemap = Player_ActionToBottle(this, this->itemActionParam);
            static s8 savebottle = 0;

            if (this->itemActionParam >= 30){
                savebottle = bottlemap;
                if (bottlemap < 0) {
                    bottlemap = 0;
                }
                if (savebottle != 0){
                    bottlemap = savebottle;
                }
            }

            Color_RGB8* bottleColor = &sBottleColors[savebottle];
            //Debug_Text(bottleColor->r,bottleColor->g,bottleColor->b,4,10,"color?");

            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2710);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2712),
                    G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gsDPSetPrimColor(0x00, 0x00, 255, 255, 255, 178);
            gDPSetEnvColor(POLY_XLU_DISP++, bottleColor->r, bottleColor->g, bottleColor->b, 0);
            gSPDisplayList(POLY_XLU_DISP++, sBottleDLists[((void)0, gSaveContext.linkAge)]);

            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2717);
        }

        if (this->actor.scale.y >= 0.0f) {
            if (!Player_HoldsHookshot(this) && ((hookedActor = this->heldActor) != NULL)) {
                if (this->stateFlags1 & PLAYER_STATE1_9) {
                    // Adjustment for child Arrow
                    if (play->mainCamera.mode == CAM_MODE_SLINGSHOT && this->itemActionParam != PLAYER_AP_SLINGSHOT){
                        Matrix_MultVec3f(&childArrowPos, &hookedActor->world.pos);
                    } else {
                        Matrix_MultVec3f(&D_80126128, &hookedActor->world.pos);
                    }
                    Matrix_RotateZYX(0x69E8, -0x5708, 0x458E, MTXMODE_APPLY);

                    Matrix_Get(&sp14C);
                    Matrix_MtxFToYXZRotS(&sp14C, &hookedActor->world.rot, 0);
                    hookedActor->shape.rot = hookedActor->world.rot;
                } else if (this->stateFlags1 & PLAYER_STATE1_11) {
                    Vec3s spB8;

                    Matrix_Get(&sp14C);
                    Matrix_MtxFToYXZRotS(&sp14C, &spB8, 0);

                    if (hookedActor->flags & ACTOR_FLAG_17) {
                        hookedActor->world.rot.x = hookedActor->shape.rot.x = spB8.x - this->unk_3BC.x;
                    } else {
                        hookedActor->world.rot.y = hookedActor->shape.rot.y = this->actor.shape.rot.y + this->unk_3BC.y;
                    }
                }
            } else {
                Matrix_Get(&this->mf_9E0);
                Matrix_MtxFToYXZRotS(&this->mf_9E0, &this->unk_3BC, 0);
            }
        }
    } else if (limbIndex == PLAYER_LIMB_R_HAND) {
        Actor* heldActor = this->heldActor;

        if (this->rightHandType == PLAYER_MODELTYPE_RH_FF) {
            Matrix_Get(&this->shieldMf);
        } else if ((this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT) ||
                   (this->rightHandType == PLAYER_MODELTYPE_RH_BOW_SLINGSHOT_2)) {

            // Hack String Selection per item
            rangedStringSelection = LINK_IS_ADULT ? (this->itemActionParam == PLAYER_AP_SLINGSHOT ? 1 : 0) : (this->itemActionParam == PLAYER_AP_SLINGSHOT ? 3 : 2);

            Vec3f stringhackData = hackStringPos[rangedStringSelection];
            Vec3f projectedHeadPos;
            SkinMatrix_Vec3fMtxFMultXYZ(&play->viewProjectionMtxF, &this->actor.focus.pos, &projectedHeadPos);

            OPEN_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2783);


            // Adult Slingshot Translates/ Rotates
            if (play->mainCamera.mode == CAM_MODE_BOWARROW && projectedHeadPos.z < -4.0f && this->itemActionParam == PLAYER_AP_SLINGSHOT){
                Matrix_Push();
                Matrix_RotateY(DEG_TO_RAD(15), MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2804),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++,gPlayAsLUT_Adult_SlingshotFps);
                Matrix_Pop();
            }

            // Child Bow Translates/ Rotates
            if (play->mainCamera.mode == CAM_MODE_SLINGSHOT && projectedHeadPos.z < -4.0f && this->itemActionParam != PLAYER_AP_SLINGSHOT) {
                Matrix_Push();
                Matrix_Translate(-100.0f, 1000.0f, 270.0f, MTXMODE_APPLY); //(up/down,towards/away from player, left/right)
                Matrix_RotateZ(DEG_TO_RAD(-10), MTXMODE_APPLY);
                gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2804),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_XLU_DISP++,gPlayAsLUT_Child_BowFpsHandR);
                Matrix_Pop();
            }

            Matrix_Push();
            // Magic values to make slingshot/ bow look good first person
            if (play->mainCamera.mode == CAM_MODE_BOWARROW && projectedHeadPos.z < -4.0f  && this->itemActionParam == PLAYER_AP_SLINGSHOT){
                Matrix_Translate(725.0f, 400.0f, -200.0f, MTXMODE_APPLY); //(up/down,towards/away from player, left/right)
            }else if (play->mainCamera.mode == CAM_MODE_SLINGSHOT && projectedHeadPos.z < -4.0f  && this->itemActionParam != PLAYER_AP_SLINGSHOT){
                Matrix_Translate(-300.0f, 240.0f, 115.0f, MTXMODE_APPLY); //(up/down,towards/away from player, left/right)
                Matrix_RotateY(DEG_TO_RAD(-6), MTXMODE_APPLY);
            }else{
                if (LINK_IS_ADULT && this->itemActionParam == PLAYER_AP_SLINGSHOT){
                    Matrix_RotateY(DEG_TO_RAD(-8), MTXMODE_APPLY);
                    Matrix_Translate(0.0f, 0.0f,-150.0f, MTXMODE_APPLY);
                }
                Matrix_Translate(stringhackData.x, stringhackData.y, stringhackData.z, MTXMODE_APPLY); 
            }

            if ((this->stateFlags1 & PLAYER_STATE1_9) && (this->unk_860 >= 0) && (this->unk_834 <= 10)) {
                Vec3f sp90;
                f32 distXYZ;

                Matrix_MultVec3f(&sZeroVec, &sp90);
                distXYZ = Math_Vec3f_DistXYZ(sCurBodyPartPos, &sp90);

                this->unk_858 = distXYZ - 3.0f;
                if (distXYZ < 3.0f) {
                    this->unk_858 = 0.0f;
                } else {
                    this->unk_858 *= 1.6f;
                    if (this->unk_858 > 1.0f) {
                        this->unk_858 = 1.0f;
                    }
                }

                this->unk_85C = -0.5f;
            }

            Matrix_Scale(1.0f, this->unk_858, 1.0f, MTXMODE_APPLY);

            // Additional rotation for slingshot so string displays at bottom of screen
            if (this->itemActionParam == PLAYER_AP_SLINGSHOT){
                if (LINK_IS_ADULT){
                    Matrix_RotateY(DEG_TO_RAD(15), MTXMODE_APPLY);
                    Matrix_RotateZ(this->unk_858 * -0.15f, MTXMODE_APPLY);
                }else{
                    Matrix_RotateZ(this->unk_858 * -0.2f, MTXMODE_APPLY);
                }
            }
            // Additional rotation for bow as child so string displays at bottom of screen
            if (this->itemActionParam != PLAYER_AP_SLINGSHOT){
                if (!LINK_IS_ADULT){
                    Matrix_RotateZ(this->unk_858 * -0.1f, MTXMODE_APPLY);
                }
            }

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, "../z_player_lib.c", 2804),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++,hackStringDLists[rangedStringSelection]);

            Matrix_Pop();
            CLOSE_DISPS(play->state.gfxCtx, "../z_player_lib.c", 2809);

        } else if ((this->actor.scale.y >= 0.0f) && (this->rightHandType == PLAYER_MODELTYPE_RH_SHIELD)) {
            Matrix_Get(&this->shieldMf);
            Player_UpdateShieldCollider(play, this, &this->shieldQuad, sRightHandLimbModelShieldQuadVertices);
        }

        if (this->actor.scale.y >= 0.0f) {
            if ((this->heldItemActionParam == PLAYER_AP_HOOKSHOT) ||
                (this->heldItemActionParam == PLAYER_AP_LONGSHOT)) {
                    
                    // Adjustment for child hookshot
                    if (LINK_IS_CHILD && play->mainCamera.mode == CAM_MODE_SLINGSHOT){
                        Matrix_MultVec3f(&Hookshot1, &this->unk_3C8);
                    }else{
                        Matrix_MultVec3f(&D_80126184, &this->unk_3C8);
                    }

                if (heldActor != NULL) {
                    MtxF sp44;

                    if (LINK_IS_CHILD && play->mainCamera.mode == CAM_MODE_SLINGSHOT){
                        Matrix_MultVec3f(&Hookshot2, &heldActor->world.pos);
                    }else{
                        Matrix_MultVec3f(&D_80126190, &heldActor->world.pos);
                    }

                    Matrix_RotateZYX(0, -0x4000, -0x4000, MTXMODE_APPLY);
                    Matrix_Get(&sp44);
                    Matrix_MtxFToYXZRotS(&sp44, &heldActor->world.rot, 0);
                    heldActor->shape.rot = heldActor->world.rot;

                    if (func_8002DD78(this) != 0) {
                        Matrix_Translate(500.0f, 300.0f, 0.0f, MTXMODE_APPLY);
                        Player_DrawHookshotReticle(
                            play, this, (this->heldItemActionParam == PLAYER_AP_HOOKSHOT) ? 38600.0f : 77600.0f);
                    }
                }
            }
        }

        if ((this->unk_862 != 0) || ((func_8002DD6C(this) == 0) && (heldActor != NULL))) {
            if (!(this->stateFlags1 & PLAYER_STATE1_10) && (this->unk_862 != 0) &&
                (this->exchangeItemId != EXCH_ITEM_NONE)) {
                Math_Vec3f_Copy(&sGetItemRefPos, &this->leftHandPos);
            } else {
                sGetItemRefPos.x = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].x + this->leftHandPos.x) * 0.5f;
                sGetItemRefPos.y = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].y + this->leftHandPos.y) * 0.5f;
                sGetItemRefPos.z = (this->bodyPartsPos[PLAYER_BODYPART_R_HAND].z + this->leftHandPos.z) * 0.5f;
            }

            if (this->unk_862 == 0) {
                Math_Vec3f_Copy(&heldActor->world.pos, &sGetItemRefPos);
            }
        }
        
    } else if (this->actor.scale.y >= 0.0f) {
        if (limbIndex == PLAYER_LIMB_SHEATH) {
            if ((this->rightHandType != PLAYER_MODELTYPE_RH_SHIELD) &&
                (this->rightHandType != PLAYER_MODELTYPE_RH_FF)) {

                Matrix_TranslateRotateZYX(&sSheathLimbModelShieldOnBackPos, &sSheathLimbModelShieldOnBackZyxRot);
                Matrix_Get(&this->shieldMf);
            }
        } else if (limbIndex == PLAYER_LIMB_HEAD) {
            Matrix_MultVec3f(&sPlayerFocusHeadLimbModelPos, &this->actor.focus.pos);

        } else if (limbIndex == PLAYER_LIMB_L_FOOT || limbIndex == PLAYER_LIMB_R_FOOT) {
            Vec3f footpos = {200.0f, 300.0f, 0.0f};
            Actor_SetFeetPos(&this->actor, limbIndex, PLAYER_LIMB_L_FOOT, &footpos, PLAYER_LIMB_R_FOOT, &footpos);
        }
    }
}