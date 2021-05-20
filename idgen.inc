#if defined _inc_idgen
    #undef _inc_idgen
#endif

#if defined pawn_idgen_included
    #endinput
#endif

#define pawn_idgen_included

#include <vector>

/*
    internal
*/

static const ASSIGN_COUNT_INDEX = 0;
static const MAX_CAPACITY_INDEX = 1;
static const RELEASED_INDEX     = 2;


static stock IdGen_SetAssignedCount(Idgen:generator, value) {
    Vec_Set(Vec:generator, ASSIGN_COUNT_INDEX, value);
}

static stock Idgen_GetReleasedByIndex(Idgen:generator, index) {
    return Vec_Get(Vec:generator, index + RELEASED_INDEX);
}

#define UNLIMITED_CAPACITY  (-1)

/*
    API
*/

stock IdGen_GetAssignedCount(Idgen:generator) {
    return Vec_Get(Vec:generator, ASSIGN_COUNT_INDEX);
}

stock Idgen_GetReleasedCount(Idgen:generator) {
    return Vec_Len(Vec:generator) - RELEASED_INDEX;
}

stock Idgen_GetCapacity(Idgen:generator) {
    return Vec_Get(Vec:generator, MAX_CAPACITY_INDEX);
}

#define INVALID_IDGEN_ID   (-1)

stock Idgen_NewID(Idgen:generator) {
    new assignCount = IdGen_GetAssignedCount(generator);
    new capacity = Idgen_GetCapacity(generator);

    new relCount = Vec_Len(Vec:generator);
    if(relCount - RELEASED_INDEX > 0) {
		--relCount;
        new id = Vec_Get(Vec:generator, relCount);
        Vec_RemoveAt(Vec:generator, relCount);
        return id;
    }

    if(assignCount >= capacity && capacity != UNLIMITED_CAPACITY) {
        return INVALID_IDGEN_ID;
    }

    IdGen_SetAssignedCount(generator, assignCount+1);
    return assignCount;
}

stock bool:Idgen_IsValidID(Idgen:generator, id) {
    if(id >= IdGen_GetAssignedCount(generator)) {
        return false;
    }
    for(new i = 0; i < Idgen_GetReleasedCount(generator); i++) {
        if(Idgen_GetReleasedByIndex(generator, i) == id) {
            return false;
        }
    }
    return true;
}

stock Idgen_ReleaseId(Idgen:generator, id) {
    Vec_Append(Vec:generator, id);
    return 1;
}

stock Idgen_Clear(Idgen:generator)
{
	new maxCapacity = Idgen_GetCapacity(generator);
	Vec_Clear(Vec:generator);
	Vec_Set(Vec:generator, MAX_CAPACITY_INDEX, maxCapacity);
}

#define INVALID_IDGEN (Idgen:-1)

stock Idgen:Idgen_New(capacity = UNLIMITED_CAPACITY) {
    new Vec:generator = Vec_New(capacity);

    if(generator == INVALID_VECTOR) {
        return INVALID_IDGEN;
    }

    Vec_Set(generator, MAX_CAPACITY_INDEX, capacity);
    Vec_Set(generator, ASSIGN_COUNT_INDEX, 0);
    return Idgen:generator;
}

stock Idgen_Delete(Idgen:generator) {
    Vec_Delete(Vec:generator);
}