#include "global.h"
#include "entity.h"
#include "coord.h"
#include "room.h"
#include "object.h"
#include "functions.h"
#include "definitions.h"

extern const Hitbox* const gObjectHitboxes[];

u32 LoadObjectSprite(Entity* this, s32 type, const ObjectDefinition* definition);
extern const ObjectDefinition gObjectDefinitions[];
extern Entity* sub_0805E744(void);

Entity* sub_080A276C(Entity* parent, u32 type, u32 type2) {
    Entity* e = sub_0805E744();
    if (e != NULL) {
        e->id = 0xC1;
        e->kind = 6;
        e->type = type;
        e->type2 = type2;
        e->parent = parent;
        AppendEntityToList(e, 6);
        PrependEntityToList(e, 6);
        CopyPosition(&gPlayerEntity, e);
        gPriorityHandler.sys_priority = 6;
        gUnk_02034490[0] = 1;
        e->field_0x68.HALF.HI = gPlayerEntity.flags;
        e->field_0x68.HALF.LO = gPlayerEntity.spriteSettings.draw;
        e->field_0x6a.HALF.LO = gPlayerEntity.iframes;
        e->field_0x6a.HALF.HI = gPlayerState.field_0x7;
        e->field_0x6c.HALF.LO = gPlayerState.keepFacing;
        e->field_0x6c.HALF.HI = gPlayerState.field_0xa;
        e->field_0x6e.HALF.LO = gPlayerState.field_0x27[0];
        e->field_0x6e.HALF.HI = gPlayerState.mobility;
        e->field_0x70.WORD = gPlayerState.flags;
        e->field_0x74.HALF.LO = gPlayerState.field_0x82[8];
        gPlayerEntity.flags &= ~ENT_COLLIDE;
        gPlayerEntity.spriteSettings.draw = 0;
    }
    return e;
}

void ObjectInit(Entity* this) {
    const ObjectDefinition* definition = &gObjectDefinitions[this->id];
    if (LoadObjectSprite(this, this->type, definition) == 2) {
        UpdateSpriteForCollisionLayer(this);
    }
}

u32 LoadObjectSprite(Entity* this, s32 type, const ObjectDefinition* definition) {
    u16 tmp1;
    u8 tmp2;

    if ((this->flags & ENT_DID_INIT) != 0) {
        // Sprite already loaded
        return 1;
    }
    if ((definition->bitfield.type) == 0) {
        // Object has no sprite
        this->flags |= ENT_DID_INIT | definition->bitfield.flags << 2;
        return 1;
    }

    if ((u8)(((*((u8*)definition) & 0xff & 3) - 2)) < 2) {
        // Multiple forms
        definition = &definition->data.definition[type];
    }
    tmp1 = definition->bitfield.gfx;
    switch (definition->bitfield.gfx_type) {
        case 2:
            this->spriteVramOffset = definition->bitfield.gfx;
            break;
        case 1:
            if (!LoadSwapGFX(this, tmp1, 0)) {
                return 0;
            }
            break;
        default:
            if (!LoadFixedGFX(this, tmp1)) {
                return 0;
            }
            break;
    }
    tmp1 = definition->data.sprite.paletteIndex;
    LoadObjPalette(this, tmp1);
    tmp2 = 0xff;
    this->animIndex = tmp2;
    this->hurtType = 0x48;
    this->spriteIndex = definition->data.sprite.spriteIndex;
    this->spriteSettings.shadow = definition->data.sprite.shadow;
    this->spritePriority.b1 = definition->data.sprite.spritePriority;
    this->spriteSettings.draw = definition->data.sprite.draw;
    this->hitbox = (Hitbox*)gObjectHitboxes[definition->bitfield.hitbox];
    this->flags |= ENT_DID_INIT | definition->bitfield.flags << 2;

    return 2;
}

Entity* CreateObject(u32 subtype, u32 form, u32 parameter) {
    Entity* entity = GetEmptyEntity();
    if (entity != NULL) {
        entity->kind = 6;
        entity->id = subtype;
        entity->type = form;
        entity->type2 = parameter;
        AppendEntityToList(entity, 6);
    }
    return entity;
}

Entity* CreateObjectWithParent(Entity* parent, u32 subtype, u32 form, u32 parameter) {
    Entity* entity = CreateObject(subtype, form, parameter);
    if (entity != NULL) {
        entity->parent = parent;
        CopyPosition(parent, entity);
    }

    return entity;
}

Entity* CreateFx(Entity* parentEnt, u32 form, u32 parameter) {
    return CreateObjectWithParent(parentEnt, SPECIAL_FX, form, parameter);
}

void CreateDust(Entity* parent) {
    CreateFx(parent, FX_DEATH, 0);
}

void CreateDustAt(s32 xOff, s32 yOff, u32 layer) {
    Entity* ent;

    ent = CreateObject(SPECIAL_FX, FX_DEATH, 0);
    if (ent != NULL) {
        ent->x.HALF.HI = gRoomControls.origin_x + xOff;
        ent->y.HALF.HI = gRoomControls.origin_y + yOff;
        ent->collisionLayer = layer;
    }
}

void CreateDustSmall(Entity* parent) {
    CreateFx(parent, FX_DASH, 0);
}

void CreateExplosionBroken(Entity* parent) {
    CreateFx(parent, FX_6, 0);
}

void CreateWaterSplash(Entity* parent) {
    CreateFx(parent, FX_WATER_SPLASH, 0);
}

Entity* sub_080A2A20(Entity* parent, u32 form, u32 parameter) {
    Entity* ent;

    ent = CreateObjectWithParent(parent, GROUND_ITEM, form, parameter);
    if (ent != NULL) {
        ent->actionDelay = 5;
    }
    return ent;
}

Entity* sub_080A2A3C(Entity* parent, u32 form, u32 subtype, u32 param_4) {
    Entity* ent;

    ent = CreateObjectWithParent(parent, GROUND_ITEM, form, subtype);
    if (ent != NULL) {
        ent->actionDelay = 5;
        ent->field_0x86.HWORD = param_4;
    }
    return ent;
}

Entity* CreateWaterTrace(Entity* parent) {
    Entity* ent;

    ent = CreateFx(parent, FX_RIPPLE, 0);
    if (ent != NULL) {
        ent->spritePriority.b0 = 7;
    }
    return ent;
}
