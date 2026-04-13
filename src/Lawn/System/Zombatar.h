#ifndef __ZOMBATAR_H__
#define __ZOMBATAR_H__

#include "../../ConstEnums.h"
#include "../../SexyAppFramework/Color.h"
#include "../../SexyAppFramework/Image.h"

#ifdef _ZOMBATAR

struct ZombatarDefinition {
    ZombatarItem     mZombatarItem;
    Sexy::Image**    mOutlineImage;
    Sexy::Image**    mImage;
    ZombatarCategory mCategory;
    int              mColumn;
    int              mRow;
    Sexy::Color*     mColorGroup;
};

extern ZombatarDefinition gZombatarDefinitions[NUM_ZOMBATAR_ITEMS];
extern Sexy::Color gZombatarSkinPalletes[];
extern Sexy::Color gZombatarDimPalletes[];
extern Sexy::Color gZombatarBrightPalletes[];

ZombatarDefinition& GetZombatarDefinition(ZombatarItem theItem);

#endif

#endif
