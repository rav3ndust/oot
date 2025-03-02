/*
 * File: z_en_okarina_tag.c
 * Overlay: ovl_En_Okarina_Tag
 * Description: Music Staff (Ocarina) spot
 */

#include "z_en_okarina_tag.h"
#include "assets/scenes/misc/hakaana_ouke/hakaana_ouke_scene.h"
#include "assets/scenes/overworld/spot02/spot02_scene.h"
#include "vt.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_25)

void EnOkarinaTag_Init(Actor* thisx, PlayState* play);
void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play);
void EnOkarinaTag_Update(Actor* thisx, PlayState* play);

void func_80ABEF2C(EnOkarinaTag* this, PlayState* play);
void func_80ABF708(EnOkarinaTag* this, PlayState* play);
void func_80ABF28C(EnOkarinaTag* this, PlayState* play);
void func_80ABF0CC(EnOkarinaTag* this, PlayState* play);
void func_80ABF4C8(EnOkarinaTag* this, PlayState* play);
void func_80ABF7CC(EnOkarinaTag* this, PlayState* play);

const ActorInit En_Okarina_Tag_InitVars = {
    ACTOR_EN_OKARINA_TAG,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnOkarinaTag),
    (ActorFunc)EnOkarinaTag_Init,
    (ActorFunc)EnOkarinaTag_Destroy,
    (ActorFunc)EnOkarinaTag_Update,
    NULL,
};

extern CutsceneData D_80ABF9D0[];
extern CutsceneData D_80ABFB40[];

void EnOkarinaTag_Destroy(Actor* thisx, PlayState* play) {
}

void EnOkarinaTag_Init(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = (EnOkarinaTag*)thisx;

    osSyncPrintf("\n\n");
    // "Ocarina tag outbreak"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ オカリナタグ発生 ☆☆☆☆☆ %x\n" VT_RST, this->actor.params);
    this->actor.flags &= ~ACTOR_FLAG_0;
    this->type = (this->actor.params >> 0xA) & 0x3F;
    this->ocarinaSong = (this->actor.params >> 6) & 0xF;
    this->switchFlag = this->actor.params & 0x3F;
    if (this->switchFlag == 0x3F) {
        this->switchFlag = -1;
    }
    if (this->ocarinaSong == 0xF) {
        this->ocarinaSong = 0;
        this->unk_158 = 1;
    }
    this->actor.targetMode = 1;
    if (this->actor.world.rot.z > 0) {
        this->interactRange = this->actor.world.rot.z * 40.0f;
    }

    // "Save information"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ セーブ情報\t ☆☆☆☆☆ %d\n" VT_RST, this->switchFlag);
    // "Type index"
    osSyncPrintf(VT_FGCOL(YELLOW) "☆☆☆☆☆ 種類インデックス ☆☆☆☆☆ %d\n" VT_RST, this->type);
    // "Correct answer information"
    osSyncPrintf(VT_FGCOL(MAGENTA) "☆☆☆☆☆ 正解情報\t ☆☆☆☆☆ %d\n" VT_RST, this->ocarinaSong);
    // "Range information"
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 範囲情報\t ☆☆☆☆☆ %d\n" VT_RST, this->actor.world.rot.z);
    // "Processing range information"
    osSyncPrintf(VT_FGCOL(CYAN) "☆☆☆☆☆ 処理範囲情報\t ☆☆☆☆☆ %f\n" VT_RST, this->interactRange);
    // "Hit?"
    osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ 当り？\t\t ☆☆☆☆☆ %d\n" VT_RST, this->unk_158);
    osSyncPrintf("\n\n");

    if ((this->switchFlag >= 0) && (Flags_GetSwitch(play, this->switchFlag))) {
        Actor_Kill(&this->actor);
    } else {
        switch (this->type) {
            case 7:
                this->actionFunc = func_80ABEF2C;
                break;
            case 2:
                if (LINK_IS_ADULT) {
                    Actor_Kill(&this->actor);
                    break;
                }
                FALLTHROUGH;
            case 1:
            case 4:
            case 6:
                this->actionFunc = func_80ABF28C;
                break;
            case 5:
                this->actor.textId = 0x5021;
                this->actionFunc = func_80ABF708;
                break;
            default:
                Actor_Kill(&this->actor);
                break;
        }
    }
}

void func_80ABEF2C(EnOkarinaTag* this, PlayState* play) {
    Player* player;
    u16 ocarinaSong;

    player = GET_PLAYER(play);
    this->unk_15A++;
    if ((this->switchFlag >= 0) && (Flags_GetSwitch(play, this->switchFlag))) {
        this->actor.flags &= ~ACTOR_FLAG_0;
    } else {
        if ((this->ocarinaSong != 6) || (gSaveContext.scarecrowSpawnSongSet)) {
            if (player->stateFlags2 & PLAYER_STATE2_24) {
                // "North! ! ! ! !"
                osSyncPrintf(VT_FGCOL(RED) "☆☆☆☆☆ 北！！！！！ ☆☆☆☆☆ %f\n" VT_RST, this->actor.xzDistToPlayer);
            }
            if ((this->actor.xzDistToPlayer < (90.0f + this->interactRange)) &&
                (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
                if (player->stateFlags2 & PLAYER_STATE2_25) {
                    ocarinaSong = this->ocarinaSong;
                    if (ocarinaSong == 6) {
                        ocarinaSong = 0xA;
                    }
                    player->stateFlags2 |= PLAYER_STATE2_23;
                    func_8010BD58(play, ocarinaSong + OCARINA_ACTION_CHECK_SARIA);
                    this->actionFunc = func_80ABF0CC;
                } else if ((this->actor.xzDistToPlayer < (50.0f + this->interactRange) &&
                            ((fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 40.0f)))) {
                    this->unk_15A = 0;
                    player->unk_6A8 = &this->actor;
                }
            }
        }
    }
}

void func_80ABF0CC(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
        this->actionFunc = func_80ABEF2C;
    } else {
        if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
            if (this->switchFlag >= 0) {
                Flags_SetSwitch(play, this->switchFlag);
            }
            if (play->sceneId == SCENE_MIZUSIN) {
                play->msgCtx.msgMode = MSGMODE_PAUSED;
            }
            if ((play->sceneId != SCENE_DAIYOUSEI_IZUMI) && (play->sceneId != SCENE_YOUSEI_IZUMI_YOKO)) {
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            }
            func_80078884(NA_SE_SY_CORRECT_CHIME);
            this->actionFunc = func_80ABEF2C;
            return;
        }
        if (this->unk_158 != 0) {
            if ((play->msgCtx.ocarinaMode == OCARINA_MODE_05) || (play->msgCtx.ocarinaMode == OCARINA_MODE_06) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_07) || (play->msgCtx.ocarinaMode == OCARINA_MODE_08) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_09) || (play->msgCtx.ocarinaMode == OCARINA_MODE_0A) ||
                (play->msgCtx.ocarinaMode == OCARINA_MODE_0D)) {
                if (this->switchFlag >= 0) {
                    Flags_SetSwitch(play, this->switchFlag);
                }
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                func_80078884(NA_SE_SY_CORRECT_CHIME);
                this->actionFunc = func_80ABEF2C;
                return;
            }
        }
        if ((play->msgCtx.ocarinaMode >= OCARINA_MODE_05) && (play->msgCtx.ocarinaMode < OCARINA_MODE_0E)) {
            play->msgCtx.ocarinaMode = OCARINA_MODE_04;
            this->actionFunc = func_80ABEF2C;
        } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_01) {
            player->stateFlags2 |= PLAYER_STATE2_23;
        }
    }
}

void func_80ABF28C(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->unk_15A++;
    if ((this->ocarinaSong != 6) || (gSaveContext.scarecrowSpawnSongSet)) {
        if ((this->switchFlag >= 0) && Flags_GetSwitch(play, this->switchFlag)) {
            this->actor.flags &= ~ACTOR_FLAG_0;
        } else if (((this->type != 4) || !GET_EVENTCHKINF(EVENTCHKINF_4B)) &&
                   ((this->type != 6) || !GET_EVENTCHKINF(EVENTCHKINF_1D)) &&
                   (this->actor.xzDistToPlayer < (90.0f + this->interactRange)) &&
                   (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 80.0f)) {
            if (player->stateFlags2 & PLAYER_STATE2_24) {
                switch (this->type) {
                    case 1:
                        func_8010BD58(play, OCARINA_ACTION_CHECK_LULLABY);
                        break;
                    case 2:
                        func_8010BD58(play, OCARINA_ACTION_CHECK_STORMS);
                        break;
                    case 4:
                        func_8010BD58(play, OCARINA_ACTION_CHECK_TIME);
                        break;
                    case 6:
                        func_8010BD58(play, OCARINA_ACTION_CHECK_LULLABY);
                        break;
                    default:
                        // "Ocarina Invisible-kun demo start check error source"
                        osSyncPrintf(VT_FGCOL(GREEN) "☆☆☆☆☆ オカリナ透明君デモ開始チェックエラー原 ☆☆☆☆☆ %d\n" VT_RST,
                                     this->type);
                        Actor_Kill(&this->actor);
                        break;
                }
                player->stateFlags2 |= PLAYER_STATE2_23;
                this->actionFunc = func_80ABF4C8;
            } else if ((this->actor.xzDistToPlayer < (50.0f + this->interactRange)) &&
                       (fabsf(player->actor.world.pos.y - this->actor.world.pos.y) < 40.0f)) {
                this->unk_15A = 0;
                player->stateFlags2 |= PLAYER_STATE2_23;
            }
        }
    }
}

void func_80ABF4C8(EnOkarinaTag* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_04) {
        this->actionFunc = func_80ABF28C;
    } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_03) {
        func_80078884(NA_SE_SY_CORRECT_CHIME);
        if (this->switchFlag >= 0) {
            Flags_SetSwitch(play, this->switchFlag);
        }
        switch (this->type) {
            case 1:
                Flags_SetSwitch(play, this->switchFlag);
                SET_EVENTCHKINF(EVENTCHKINF_39);
                break;
            case 2:
                play->csCtx.segment = D_80ABF9D0;
                gSaveContext.cutsceneTrigger = 1;
                func_800F574C(1.18921f, 0x5A);
                break;
            case 4:
                play->csCtx.segment = D_80ABFB40;
                gSaveContext.cutsceneTrigger = 1;
                break;
            case 6:
                play->csCtx.segment = LINK_IS_ADULT ? SEGMENTED_TO_VIRTUAL(&spot02_scene_Cs_003C80)
                                                    : SEGMENTED_TO_VIRTUAL(&spot02_scene_Cs_005020);
                gSaveContext.cutsceneTrigger = 1;
                SET_EVENTCHKINF(EVENTCHKINF_1D);
                func_80078884(NA_SE_SY_CORRECT_CHIME);
                break;
            default:
                break;
        }
        play->msgCtx.ocarinaMode = OCARINA_MODE_04;
        this->actionFunc = func_80ABF28C;
    } else {
        if (play->msgCtx.ocarinaMode >= OCARINA_MODE_05) {
            if (play->msgCtx.ocarinaMode < OCARINA_MODE_0E) {
                play->msgCtx.ocarinaMode = OCARINA_MODE_04;
                this->actionFunc = func_80ABF28C;
                return;
            }
        }
        if (play->msgCtx.ocarinaMode == OCARINA_MODE_01) {
            player->stateFlags2 |= PLAYER_STATE2_23;
        }
    }
}

void func_80ABF708(EnOkarinaTag* this, PlayState* play) {
    s16 yawDiff;
    s16 yawDiffNew;

    if (Actor_ProcessTalkRequest(&this->actor, play)) {
        this->actionFunc = func_80ABF7CC;
    } else {
        yawDiff = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
        this->unk_15A++;
        if (!(this->actor.xzDistToPlayer > 120.0f)) {
            if (CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
                this->actor.textId = 0x5021;
            }
            yawDiffNew = ABS(yawDiff);
            if (yawDiffNew < 0x4300) {
                this->unk_15A = 0;
                func_8002F2CC(&this->actor, play, 70.0f);
            }
        }
    }
}

void func_80ABF7CC(EnOkarinaTag* this, PlayState* play) {
    // "Open sesame sesame!"
    osSyncPrintf(VT_FGCOL(MAGENTA) "☆☆☆☆☆ 開けゴマゴマゴマ！ ☆☆☆☆☆ %d\n" VT_RST, Message_GetState(&play->msgCtx));

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        Message_CloseTextbox(play);
        if (!CHECK_QUEST_ITEM(QUEST_SONG_SUN)) {
            play->csCtx.segment = SEGMENTED_TO_VIRTUAL(&gSunSongGraveSunSongTeachCs);
            gSaveContext.cutsceneTrigger = 1;
        }
        this->actionFunc = func_80ABF708;
    }
}

void EnOkarinaTag_Update(Actor* thisx, PlayState* play) {
    EnOkarinaTag* this = (EnOkarinaTag*)thisx;

    this->actionFunc(this, play);
    if (BREG(0) != 0) {
        if (this->unk_15A != 0) {
            if (!(this->unk_15A & 1)) {
                DebugDisplay_AddObject(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                                       this->actor.world.rot.x, this->actor.world.rot.y, this->actor.world.rot.z, 1.0f,
                                       1.0f, 1.0f, 120, 120, 120, 255, 4, play->state.gfxCtx);
            }
        } else {
            DebugDisplay_AddObject(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z,
                                   this->actor.world.rot.x, this->actor.world.rot.y, this->actor.world.rot.z, 1.0f,
                                   1.0f, 1.0f, 255, 0, 0, 255, 4, play->state.gfxCtx);
        }
    }
}
