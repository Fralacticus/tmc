#define NENT_DEPRECATED
#include "entity.h"
#include "enemy.h"
#include "room.h"
#include "object.h"
#include "functions.h"

typedef struct {
    Entity base;
    u8 filler[0xe];
    u16 unk_0x76;
} BombarossaEntity;

void (*const gUnk_080CEB38[])(Entity*);
const s8 gUnk_080CEB50[];

extern void sub_080A2CC0(Entity*, void*, void*);

void sub_0803350C(BombarossaEntity* this);

void Bombarossa(Entity* this) {
    gUnk_080CEB38[GetNextFunction(this)](this);
}

void sub_080333D4(BombarossaEntity* this) {
    if (super->action == 0) {
        super->action = 1;
        super->actionDelay = Random() & 0xf;
        InitializeAnimation(super, 0);
        if (super->type != 0) {
            super->child = GetCurrentRoomProperty(super->type);
            sub_080A2CC0(super, &super->child, &this->unk_0x76);
        }
    }

    super->z.HALF.HI = gUnk_080CEB50[(((++super->actionDelay) >> 4) & 0x7) + (super->type2 << 3)];
    GetNextFrame(super);

    if (super->type != 0) {
        sub_0803350C(this);
    }
}

void sub_08033448(BombarossaEntity* this) {
    Entity* ent;
    switch (super->bitfield & 0x7f) {
        default:
            ent = CreateObject(OBJECT_20, 0, 0);
            if (ent) {
                CopyPosition(super, ent);
            }
            DeleteThisEntity();
        case 1:
        case 15:
        case 19:
        case 27:
        case 29:
        case 30:
            sub_0804AA30(super, gUnk_080CEB38);
            break;
    }
}

void nullsub_158() {
}

void sub_0803350C(BombarossaEntity* this) {
    if ((super->direction & 0x80) == 0) {
        LinearMoveUpdate(super);
    }

    if (--this->unk_0x76 == 0) {
        sub_080A2CC0(super, &super->child, &this->unk_0x76);
    }
}

void (*const gUnk_080CEB38[])(Entity*) = {
    (EntityActionPtr)sub_080333D4, (EntityActionPtr)sub_08033448, sub_08001324, sub_0804A7D4, sub_08001242, nullsub_158,
};

const s8 gUnk_080CEB50[] = {
    0, -1, -2, -3, -4, -3, -2, -1, -18, -19, -20, -21, -22, -21, -20, -19,
};
