#ifndef Z64COMMON_H
#define Z64COMMON_H

// just in case new z64hdr is paired with old z64rom
#ifndef Z64ROM_VERSION
#   define Z64ROM_VERSION 0
#endif

// just in case this is ever needed in the future, versioning starts now
#define Z64HDR_VERSION 1000000

#define F3DEX_GBI_2
#define GS2DEX_H

#ifndef GLOBAL_H

#include "ultra64.h"
#include "global.h"

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 category;
    /* 0x04 */ u32 flags;
    /* 0x08 */ s16 objectId;
    /* 0x0A */ u16 padding;
    /* 0x0C */ u32 instanceSize;
    /* 0x10 */ ActorFunc init;
    /* 0x14 */ ActorFunc destroy;
    /* 0x18 */ ActorFunc update;
    /* 0x1C */ ActorFunc draw;
} ActorInitExplPad; // size = 0x20

#endif

extern struct GraphicsContext* __gfxCtx;

#endif