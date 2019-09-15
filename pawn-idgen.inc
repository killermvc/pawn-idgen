#if defined pawn_idgen_included
    #endinput
#endif

#define pawn_idgen_included

#include <pawn-vector>

/*
    internal
*/

static const ASSIGN_COUNT_INDEX = 0;
static const MAX_CAPACITY_INDEX = 1;
static const RELEASED_INDEX     = 2;


static stock IdGen_SetAssignedCount(Idgen:generator, value) {
    Vec_SetValue(Vec:generator, ASSIGN_COUNT_INDEX, value);
}

static stock Vec:IdGen_GetReleasedIds(Idgen:generator) {
    return Vec_GetValueVec(Vec:generator, RELEASED_INDEX);
}

static stock Idgen_GetReleasedByIndex(Idgen:generator, index) {
    return Vec_GetValue(IdGen_GetReleasedIds(generator), index);
}

static stock Idgen_AppendReleased(Idgen:generator, value) {
    Vec_Append(IdGen_GetReleasedIds(generator), value);
}

#define UNLIMITED_CAPACITY  (-1)

/*
    API
*/

stock IdGen_GetAssignedCount(Idgen:generator) {
    return Vec_GetValue(Vec:generator, ASSIGN_COUNT_INDEX);
}

stock Idgen_GetReleasedCount(Idgen:generator) {
    return Vec_GetLength(IdGen_GetReleasedIds(generator));
}

stock Idgen_GetCapacity(Idgen:generator) {
    return Vec_GetValue(Vec:generator, MAX_CAPACITY_INDEX);
}

#define INVALID_IDGEN_ID   (-1)

stock Idgen_NewID(Idgen:generator) {
    new assignCount = IdGen_GetAssignedCount(generator);
    new capacity = Idgen_GetCapacity(generator);
    
    if(assignCount >= capacity && capacity != UNLIMITED_CAPACITY) {
        return INVALID_IDGEN_ID;
    }
    new Vec:released = IdGen_GetReleasedIds(generator);
    new relCount = Vec_GetLength(released);
    if(relCount > 0) {
        relCount--;
        new id = Vec_GetValue(released, relCount);
        Vec_Remove(released, relCount);
        return id;
    }
    IdGen_SetAssignedCount(generator, assignCount+1);
    return assignCount;
}

stock bool:Idgen_IsValidID(Idgen:generator, id) {
    printf("id: %d; assign: %d", id, IdGen_GetAssignedCount(generator));
    if(id >= IdGen_GetAssignedCount(generator)) {
        print("id >= assign");
        return false;
    }
    printf("releasedCount: %d", Idgen_GetReleasedCount(generator));
    for(new i = 0; i < Idgen_GetReleasedCount(generator); i++) {
        printf("released[%d] = %d", i, Idgen_GetReleasedByIndex(generator, i));
        if(Idgen_GetReleasedByIndex(generator, i) == id) {
            return false;
        }
    }
    return true;
}

stock bool:Idgen_IsValidGenerator(Idgen:generator) {
    if(!IsValidVec(Vec:generator) || !IsValidVec(IdGen_GetReleasedIds(generator))) {
        return false;
    }
    return true;
}

stock Idgen_ReleaseId(Idgen:generator, id) {
    if(!Idgen_IsValidID(generator, id)) {
        return 0;
    }
    Idgen_AppendReleased(generator, id);
    return 1;
}

#define INVALID_IDGEN (Idgen:-1)

stock Idgen:Idgen_New(capacity) {
    new Vec:generator = Vec_New(capacity);

    if(generator == INVALID_VECTOR_ID) {
        return INVALID_IDGEN;
    }
    
    Vec_SetValue(generator, MAX_CAPACITY_INDEX, capacity);
    Vec_SetValue(generator, ASSIGN_COUNT_INDEX, 0);
    Vec_SetValueVec(generator, RELEASED_INDEX, Vec_New(5));
    return Idgen:generator;
}


stock Idgen_Delete(Idgen:generator) {
    Vec_Delete(IdGen_GetReleasedIds(generator));
    Vec_Delete(Vec:generator);    
}