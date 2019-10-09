#define RUN_TESTS
#include <a_samp>
#include "idgen.inc"
#include <YSI_Core\y_testing>

Test:New() {
    new Idgen:generator = Idgen_New(50);
    ASSERT(Idgen_IsValidGenerator(generator));
}

Test:NewIds() {
    new Idgen:generator = Idgen_New(10);
    new ids[3];
    ids[0] = Idgen_NewID(generator);
    ids[1] = Idgen_NewID(generator);
    ids[2] = Idgen_NewID(generator);

    ASSERT(ids[0] == 0);
    ASSERT(ids[1] == 1);
    ASSERT(ids[2] == 2);
}

Test:ReleaseIds() {
    new Idgen:generator = Idgen_New(10);
    new ids[3];
    ids[0] = Idgen_NewID(generator);
    ids[1] = Idgen_NewID(generator);
    ids[2] = Idgen_NewID(generator);

    Idgen_ReleaseId(generator, ids[0]);
    Idgen_ReleaseId(generator, ids[1]);
    Idgen_ReleaseId(generator, ids[2]);

    ASSERT(!Idgen_IsValidID(generator, ids[0]));
    ASSERT(!Idgen_IsValidID(generator, ids[1]));
    ASSERT(!Idgen_IsValidID(generator, ids[2]));
}

Test:NewIdAfterRelease() {
    new Idgen:generator = Idgen_New(10);
    new ids[3];
    ids[0] = Idgen_NewID(generator);
    ids[1] = Idgen_NewID(generator);
    ids[2] = Idgen_NewID(generator);

    Idgen_ReleaseId(generator, ids[2]);
    Idgen_ReleaseId(generator, ids[1]);
    Idgen_ReleaseId(generator, ids[0]);

    ASSERT(Idgen_NewID(generator) == ids[0]);
    ASSERT(Idgen_NewID(generator) == ids[1]);
    ASSERT(Idgen_NewID(generator) == ids[2]);
}

Test:TwoGenerators() {
    new Idgen:generator1 = Idgen_New(10);
    new Idgen:generator2 = Idgen_New(10);

    new ids1[3], ids2[3];

    ids1[0] = Idgen_NewID(generator1);
    ids1[1] = Idgen_NewID(generator1);
    ids1[2] = Idgen_NewID(generator1);

    ids2[0] = Idgen_NewID(generator2);
    ids2[1] = Idgen_NewID(generator2);
    ids2[2] = Idgen_NewID(generator2);

    ASSERT(ids1[0] == 0 && ids1[1] == 1 && ids1[2] == 2);
    ASSERT(ids2[0] == 0 && ids2[1] == 1 && ids2[2] == 2);
}

Test:Delete() {
    new Idgen:generator = Idgen_New(10);

    Idgen_Delete(generator);
    ASSERT(!Idgen_IsValidGenerator(generator));
}

Test:MaxCapacity() {
    new Idgen:generator = Idgen_New(2);

    ASSERT(Idgen_NewID(generator) != INVALID_IDGEN_ID);
    ASSERT(Idgen_NewID(generator) != INVALID_IDGEN_ID);
    ASSERT(Idgen_NewID(generator) == INVALID_IDGEN_ID);
}