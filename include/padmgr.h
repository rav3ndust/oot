#ifndef PADMGR_H
#define PADMGR_H

#include "ultra64.h"
#include "irqmgr.h"

typedef enum {
    CONT_PAK_NONE,
    CONT_PAK_RUMBLE,
    CONT_PAK_OTHER
} ControllerPakType;

typedef struct {
    /* 0x00 */ OSContPad cur;
    /* 0x06 */ OSContPad prev;
    /* 0x0C */ OSContPad press; // X/Y store delta from last frame
    /* 0x12 */ OSContPad rel; // X/Y store adjusted
} Input; // size = 0x18

typedef struct PadMgr {
    /* 0x0000 */ OSContStatus padStatus[MAXCONTROLLERS];
    /* 0x0010 */ OSMesg serialMsg;
    /* 0x0014 */ OSMesg lockMsg;
    /* 0x0018 */ OSMesg interruptMsgBuf[4];
    /* 0x0028 */ OSMesgQueue serialLockQueue;
    /* 0x0040 */ OSMesgQueue lockQueue;
    /* 0x0058 */ OSMesgQueue interruptQueue;
    /* 0x0070 */ IrqMgrClient irqClient;
    /* 0x0078 */ IrqMgr* irqMgr;
    /* 0x0080 */ OSThread thread;
    /* 0x0230 */ Input inputs[MAXCONTROLLERS];
    /* 0x0290 */ OSContPad pads[MAXCONTROLLERS];
    /* 0x02A8 */ vu8 validCtrlrsMask;
    /* 0x02A9 */ u8 nControllers;
    /* 0x02AA */ u8 ctrlrIsConnected[MAXCONTROLLERS];
    /* 0x02AE */ u8 pakType[MAXCONTROLLERS];
    /* 0x02B2 */ vu8 rumbleEnable[MAXCONTROLLERS];
    /* 0x02B6 */ u8 rumbleTimer[MAXCONTROLLERS];
    /* 0x02BC */ OSPfs rumblePfs[MAXCONTROLLERS];
    /* 0x045C */ vu8 rumbleOffTimer; // amount of VI retraces to not rumble for, takes priority over rumbleOnTimer
    /* 0x045D */ vu8 rumbleOnTimer; // amount of VI retraces to rumble for
    /* 0x045E */ u8 isResetting;
    /* 0x0460 */ void (*retraceCallback)(struct PadMgr* padMgr, s32 arg);
    /* 0x0464 */ s32 retraceCallbackValue;
} PadMgr; // size = 0x468

extern PadMgr gPadMgr;

// Initialization

void PadMgr_Init(PadMgr* padMgr, OSMesgQueue* serialEventQueue, IrqMgr* irqMgr, OSId id, OSPri priority, void* stack);

// Fetching inputs

// This function cannot be prototyped here without AVOID_UB because it is called incorrectly in fault.c (see bug in
// `Fault_PadCallback`)
#ifdef AVOID_UB
void PadMgr_RequestPadData(PadMgr* padmgr, Input* inputs, s32 gameRequest);
#endif

// For internal use by Controller Pak systems

OSMesgQueue* PadMgr_AcquireSerialEventQueue(PadMgr* padMgr);
void PadMgr_ReleaseSerialEventQueue(PadMgr* padMgr, OSMesgQueue* serialEventQueue);

// Rumble

void PadMgr_RumbleStop(PadMgr* padMgr);
void PadMgr_RumbleReset(PadMgr* padMgr);
void PadMgr_RumbleSetSingle(PadMgr* padMgr, u32 port, u32 rumble);
void PadMgr_RumbleSet(PadMgr* padMgr, u8* enable);

#endif
