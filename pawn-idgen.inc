#if defined pawn_idgen_included
    #endinput
#endif

#define pawn_idgen_included

#include <pawn-vector>

/*
    internal
*/

static Vec:releasedIds;
static Vec:assignedCount;
static Vec:maxCapacity;

static Idgen:internalGenerator = Idgen:0;

static stock IdGen_SetAssignedCount(Idgen:generator, value) {
    Vec_SetValue(assignedCount, _:generator, value);
}

static stock IdGen_SetReleasedIds(Idgen:generator, Vec:value) {
    Vec_SetValue(releasedIds, _:generator, _:value);
}

static stock Vec:IdGen_GetReleasedIds(Idgen:generator) {
    return Vec:Vec_GetValue(releasedIds, _:generator);
}

static stock Idgen_GetReleasedByIndex(Idgen:generator, index) {
    return Vec_GetValue(IdGen_GetReleasedIds(generator), index);
}

static stock Idgen_AppendReleased(Idgen:generator, value) {
    Vec_Append(IdGen_GetReleasedIds(generator), value);
}

#define UNLIMITED_CAPACITY  (-1)

#include <YSI_Coding\y_hooks>
hook OnScriptInit() {
    releasedIds = Vec_New(10);
    if(releasedIds == INVALID_VECTOR_ID) {
        P:F("PAWN-IDGEN couldn't create vector for released ids");
    }
    maxCapacity = Vec_New(10);
    if(maxCapacity == INVALID_VECTOR_ID) {
        P:F("AWN-IDGEN couldn't create vector for maxCapacity");
    }
    assignedCount = Vec_New(10);
    if(assignedCount == INVALID_VECTOR_ID) {
        P:F("AWN-IDGEN couldn't create vector for assigned count");
    }
    IdGen_SetAssignedCount(internalGenerator, 1);
    
    Vec_SetValue(maxCapacity, _:internalGenerator, UNLIMITED_CAPACITY);
    IdGen_SetReleasedIds(internalGenerator, Vec_New());
}

hook OnScriptExit() {
    Vec_Delete(maxCapacity);
    Vec_Delete(assignedCount);
    for(new i = 0; i < Vec_GetLength(releasedIds); i++) {
        Vec_Delete(Vec:Vec_GetValue(releasedIds, i));
    }
    Vec_Delete(releasedIds);
}

/*
    API
*/

stock IdGen_GetAssignedCount(Idgen:generator) {
    return Vec_GetValue(assignedCount, _:generator);
}

stock Idgen_GetReleasedCount(Idgen:generator) {
    return Vec_GetLength(IdGen_GetReleasedIds(generator));
}

#define INVALID_IDGEN_ID   (-1)

stock Idgen_NewID(Idgen:generator) {
    new assignCount = IdGen_GetAssignedCount(generator);
    new capacity = Vec_GetValue(maxCapacity, _:generator);
    
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
    Vec_SetValue(assignedCount, _:generator, assignCount+1);
    return assignCount;
}

stock bool:Idgen_IsValidID(Idgen:generator, id) {
    if(id >= IdGen_GetAssignedCount(generator)) {
        return false;
    }
    for(new i =0; i < Idgen_GetReleasedCount(generator); i++) {
        if(Idgen_GetReleasedByIndex(generator, i) == id) {
            return false;
        }
    }
    return true;
}

stock bool:Idgen_IsValidGenerator(Idgen:generator) {
    return Idgen_IsValidID(internalGenerator, _:generator);
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
    new Idgen:id = Idgen:Idgen_NewID(internalGenerator);
    if(_:id == INVALID_IDGEN_ID) {
        return INVALID_IDGEN;
    }
    Vec_SetValue(maxCapacity, _:id, capacity);
    IdGen_SetReleasedIds(id, Vec_New());
    return id;
}


stock Idgen_Delete(Idgen:generator) {
    Vec_Delete(Vec:Vec_GetValue(releasedIds, _:generator));
    //edit pawn-vector to allow to set beyond length
    Vec_SetValue(maxCapacity, _:generator, 0);
    Vec_SetValue(assignedCount, _:generator, 0);
    return Idgen_ReleaseId(internalGenerator, _:generator);
}