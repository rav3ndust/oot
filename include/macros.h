#ifndef MACROS_H
#define MACROS_H

#ifndef __GNUC__
#define __attribute__(x)
#endif

#ifndef AVOID_UB
#define BAD_RETURN(type) type
#else
#define BAD_RETURN(type) void
#endif

#define UNUSED __attribute__((unused))
#define FALLTHROUGH __attribute__((fallthrough))

#define ARRAY_COUNT(arr) (s32)(sizeof(arr) / sizeof(arr[0]))
#define ARRAY_COUNTU(arr) (u32)(sizeof(arr) / sizeof(arr[0]))

#define PHYSICAL_TO_VIRTUAL(addr) (void*)((u32)(addr) + 0x80000000)
#define VIRTUAL_TO_PHYSICAL(addr) (u32)((u8*)(addr) - 0x80000000)
#define SEGMENTED_TO_VIRTUAL(addr) PHYSICAL_TO_VIRTUAL(gSegments[SEGMENT_NUMBER(addr)] + SEGMENT_OFFSET(addr))

#define SQ(x) ((x)*(x))
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define DECR(x) ((x) == 0 ? 0 : --(x))
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define CLAMP_MAX(x, max) ((x) > (max) ? (max) : (x))
#define CLAMP_MIN(x, min) ((x) < (min) ? (min) : (x))

#define RGBA8(r, g, b, a) ((((r) & 0xFF) << 24) | (((g) & 0xFF) << 16) | (((b) & 0xFF) << 8) | (((a) & 0xFF) << 0))

#define GET_PLAYER(play) ((Player*)(play)->actorCtx.actorLists[ACTORCAT_PLAYER].head)

#define GET_ACTIVE_CAM(play) ((play)->cameraPtrs[(play)->activeCamId])

#define LINK_IS_ADULT (gSaveContext.linkAge == LINK_AGE_ADULT)
#define LINK_IS_CHILD (gSaveContext.linkAge == LINK_AGE_CHILD)

#define YEARS_CHILD 5
#define YEARS_ADULT 17
#define LINK_AGE_IN_YEARS (!LINK_IS_ADULT ? YEARS_CHILD : YEARS_ADULT)

#define CLOCK_TIME(hr, min) ((s32)(((hr) * 60 + (min)) * (f32)0x10000 / (24 * 60) + 0.5f))

#define IS_DAY (gSaveContext.nightFlag == 0)
#define IS_NIGHT (gSaveContext.nightFlag == 1)

#define SLOT(item) gItemSlots[item]
#define INV_CONTENT(item) gSaveContext.inventory.items[SLOT(item)]
#define AMMO(item) gSaveContext.inventory.ammo[SLOT(item)]
#define BEANS_BOUGHT AMMO(ITEM_BEAN + 1)

#define ALL_EQUIP_VALUE(equip) ((s32)(gSaveContext.inventory.equipment & gEquipMasks[equip]) >> gEquipShifts[equip])
#define CUR_EQUIP_VALUE(equip) ((s32)(gSaveContext.equips.equipment & gEquipMasks[equip]) >> gEquipShifts[equip])
#define OWNED_EQUIP_FLAG(equip, value) (gBitFlags[value] << gEquipShifts[equip])
#define OWNED_EQUIP_FLAG_ALT(equip, value) ((1 << (value)) << gEquipShifts[equip])
#define CHECK_OWNED_EQUIP(equip, value) (OWNED_EQUIP_FLAG(equip, value) & gSaveContext.inventory.equipment)
#define CHECK_OWNED_EQUIP_ALT(equip, value) (gBitFlags[(value) + (equip) * 4] & gSaveContext.inventory.equipment)

#define SWORD_EQUIP_TO_PLAYER(swordEquip) (swordEquip)
#define SHIELD_EQUIP_TO_PLAYER(shieldEquip) (shieldEquip)
#define TUNIC_EQUIP_TO_PLAYER(tunicEquip) ((tunicEquip) - 1)
#define BOOTS_EQUIP_TO_PLAYER(bootsEquip) ((bootsEquip) - 1)

#define CUR_UPG_VALUE(upg) ((s32)(gSaveContext.inventory.upgrades & gUpgradeMasks[upg]) >> gUpgradeShifts[upg])
#define CAPACITY(upg, value) gUpgradeCapacities[upg][value]
#define CUR_CAPACITY(upg) CAPACITY(upg, CUR_UPG_VALUE(upg))

#define CHECK_QUEST_ITEM(item) (gBitFlags[item] & gSaveContext.inventory.questItems)
#define CHECK_DUNGEON_ITEM(item, dungeonIndex) (gSaveContext.inventory.dungeonItems[dungeonIndex] & gBitFlags[item])

#define GET_GS_FLAGS(index) \
    ((gSaveContext.gsFlags[(index) >> 2] & gGsFlagsMasks[(index) & 3]) >> gGsFlagsShifts[(index) & 3])
#define SET_GS_FLAGS(index, value) \
    (gSaveContext.gsFlags[(index) >> 2] |= (value) << gGsFlagsShifts[(index) & 3])

#define HIGH_SCORE(score) (gSaveContext.highScores[score])

#define GET_EVENTCHKINF(flag) (gSaveContext.eventChkInf[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define SET_EVENTCHKINF(flag) (gSaveContext.eventChkInf[(flag) >> 4] |= (1 << ((flag) & 0xF)))
#define CLEAR_EVENTCHKINF(flag) (gSaveContext.eventChkInf[(flag) >> 4] &= ~(1 << ((flag) & 0xF)))

#define GET_ITEMGETINF(flag) (gSaveContext.itemGetInf[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define SET_ITEMGETINF(flag) (gSaveContext.itemGetInf[(flag) >> 4] |= (1 << ((flag) & 0xF)))

#define GET_INFTABLE(flag) (gSaveContext.infTable[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define SET_INFTABLE(flag) (gSaveContext.infTable[(flag) >> 4] |= (1 << ((flag) & 0xF)))
#define CLEAR_INFTABLE(flag) (gSaveContext.infTable[(flag) >> 4] &= ~(1 << ((flag) & 0xF)))

#define GET_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] & (1 << ((flag) & 0xF)))
#define SET_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] |= (1 << ((flag) & 0xF)))
#define CLEAR_EVENTINF(flag) (gSaveContext.eventInf[(flag) >> 4] &= ~(1 << ((flag) & 0xF)))

#define B_BTN_ITEM ((gSaveContext.buttonStatus[0] == ITEM_NONE)                    \
                        ? ITEM_NONE                                                \
                        : (gSaveContext.equips.buttonItems[0] == ITEM_SWORD_KNIFE) \
                            ? ITEM_SWORD_BGS                                       \
                            : gSaveContext.equips.buttonItems[0])

#define C_BTN_ITEM(button) ((gSaveContext.buttonStatus[(button) + 1] != BTN_DISABLED) \
                                ? gSaveContext.equips.buttonItems[(button) + 1]       \
                                : ITEM_NONE)

#define CHECK_BTN_ALL(state, combo) (~((state) | ~(combo)) == 0)
#define CHECK_BTN_ANY(state, combo) (((state) & (combo)) != 0)

#define CHECK_FLAG_ALL(flags, mask) (((flags) & (mask)) == (mask))


#define LOG(exp, value, format, file, line)         \
    do {                                            \
        LogUtils_LogThreadId(file, line);           \
        osSyncPrintf(exp " = " format "\n", value); \
    } while (0)

#define LOG_STRING(string, file, line) LOG(#string, string, "%s", file, line)
#define LOG_ADDRESS(exp, value, file, line) LOG(exp, value, "%08x", file, line)
#define LOG_TIME(exp, value, file, line) LOG(exp, value, "%lld", file, line)
#define LOG_NUM(exp, value, file, line) LOG(exp, value, "%d", file, line)
#define LOG_HEX(exp, value, file, line) LOG(exp, value, "%x", file, line)
#define LOG_FLOAT(exp, value, file, line) LOG(exp, value, "%f", file, line)

#define SET_NEXT_GAMESTATE(curState, newInit, newStruct) \
    do {                                                 \
        (curState)->init = newInit;                      \
        (curState)->size = sizeof(newStruct);            \
    } while (0)

#define SET_FULLSCREEN_VIEWPORT(view)      \
    {                                      \
        Viewport viewport;                 \
        viewport.bottomY = SCREEN_HEIGHT;  \
        viewport.rightX = SCREEN_WIDTH;    \
        viewport.topY = 0;                 \
        viewport.leftX = 0;                \
        View_SetViewport(view, &viewport); \
    }                                      \
    (void)0

extern GraphicsContext* __gfxCtx;

#define WORK_DISP       __gfxCtx->work.p
#define POLY_OPA_DISP   __gfxCtx->polyOpa.p
#define POLY_XLU_DISP   __gfxCtx->polyXlu.p
#define OVERLAY_DISP    __gfxCtx->overlay.p

// __gfxCtx shouldn't be used directly.
// Use the DISP macros defined above when writing to display buffers.
#define OPEN_DISPS(gfxCtx, file, line) \
    {                                  \
        GraphicsContext* __gfxCtx;     \
        Gfx* dispRefs[4];              \
        __gfxCtx = gfxCtx;             \
        (void)__gfxCtx;                \
        Graph_OpenDisps(dispRefs, gfxCtx, file, line)

#define CLOSE_DISPS(gfxCtx, file, line)                 \
        Graph_CloseDisps(dispRefs, gfxCtx, file, line); \
    }                                                   \
    (void)0

/**
 * `x` vertex x
 * `y` vertex y
 * `z` vertex z
 * `s` texture s coordinate
 * `t` texture t coordinate
 * `crnx` red component of color vertex, or x component of normal vertex
 * `cgny` green component of color vertex, or y component of normal vertex
 * `cbnz` blue component of color vertex, or z component of normal vertex
 * `a` alpha
 */
#define VTX(x,y,z,s,t,crnx,cgny,cbnz,a) { { { x, y, z }, 0, { s, t }, { crnx, cgny, cbnz, a } } }

#define VTX_T(x,y,z,s,t,cr,cg,cb,a) { { x, y, z }, 0, { s, t }, { cr, cg, cb, a } }

#ifdef NDEBUG
#define ASSERT(cond, msg, file, line) ((void)0)
#elif defined(REAL_ASSERT_MACRO)
#define ASSERT(cond, msg, file, line) ((cond) ? ((void)0) : __assert(#cond, __FILE__, __LINE__))
#else
#define ASSERT(cond, msg, file, line) ((cond) ? ((void)0) : __assert(msg, file, line))
#endif

#define gDPSetTileCustom(pkt, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt)                    \
    do {                                                                                                               \
        gDPPipeSync(pkt);                                                                                              \
        gDPTileSync(pkt);                                                                                              \
        gDPSetTile(pkt, fmt, siz, (((width)*siz##_TILE_BYTES) + 7) >> 3, 0, G_TX_LOADTILE, 0, cmt, maskt, shiftt, cms, \
                   masks, shifts);                                                                                     \
        gDPTileSync(pkt);                                                                                              \
        gDPSetTile(pkt, fmt, siz, (((width)*siz##_TILE_BYTES) + 7) >> 3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt,  \
                   cms, masks, shifts);                                                                                \
        gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC,                                \
                       ((height)-1) << G_TEXTURE_IMAGE_FRAC);                                                          \
    } while (0)

#endif
