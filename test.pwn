#define RUN_TESTS
#define YSI_YES_HEAP_MALLOC
#include <a_samp>
#include "idgen.inc"
#include <YSI_Core\y_testing>

Test:New() {
    new Idgen:generator = Idgen_New(50);
    ASSERT_EQ(Vec_Get(Vec:generator, 0), 0);
	ASSERT_EQ(Vec_Get(Vec:generator, 1), 50);
}

Test:NewIds() {
    new Idgen:generator = Idgen_New(10);
    new ids[3];
    ids[0] = Idgen_NewID(generator);
    ids[1] = Idgen_NewID(generator);
    ids[2] = Idgen_NewID(generator);

    ASSERT_EQ(ids[0], 0);
    ASSERT_EQ(ids[1], 1);
    ASSERT_EQ(ids[2], 2);
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

    ASSERT_FALSE(Idgen_IsValidID(generator, ids[0]));
    ASSERT_FALSE(Idgen_IsValidID(generator, ids[1]));
    ASSERT_FALSE(Idgen_IsValidID(generator, ids[2]));
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

    ASSERT_EQ(Idgen_NewID(generator), ids[0]);
    ASSERT_EQ(Idgen_NewID(generator), ids[1]);
    ASSERT_EQ(Idgen_NewID(generator), ids[2]);
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

Test:MaxCapacity() {
    new Idgen:generator = Idgen_New(2);

    ASSERT_NE(Idgen_NewID(generator), INVALID_IDGEN_ID);
    ASSERT_NE(Idgen_NewID(generator), INVALID_IDGEN_ID);
    ASSERT_EQ(Idgen_NewID(generator), INVALID_IDGEN_ID);
}

Test:Clear()
{
	new Idgen:generator = Idgen_New(10);
	new id1 = Idgen_NewID(generator);
	new id2 = Idgen_NewID(generator);

	Idgen_Clear(generator);

	ASSERT_EQ(Idgen_GetReleasedCount(generator), 0);
	ASSERT_FALSE(Idgen_IsValidID(generator, id1));
	ASSERT_FALSE(Idgen_IsValidID(generator, id2));
	ASSERT_EQ(Idgen_GetCapacity(generator), 10);
}

Test:ReduceStack()
{
    new Idgen:generator = Idgen_New(10);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_ReleaseId(generator, 2);
    Idgen_ReleaseId(generator, 1);
    Idgen_ReleaseId(generator, 3);

    new count = Idgen_ReduceReleasedStack(generator);

    ASSERT_EQ(count, 2);
    ASSERT_EQ(Idgen_GetReleasedCount(generator), 0);
    ASSERT_EQ(IdGen_GetAssignedCount(generator), 1);
}

Test:ReduceStack_Fail()
{
    new Idgen:generator = Idgen_New(10);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_ReleaseId(generator, 2);
    Idgen_ReleaseId(generator, 1);

    new count = Idgen_ReduceReleasedStack(generator);

    ASSERT_EQ(count, 0);
    ASSERT_EQ(Idgen_GetReleasedCount(generator), 2);
    ASSERT_EQ(IdGen_GetAssignedCount(generator), 4);
}

Test:Foreach()
{
    new Idgen:generator = Idgen_New(10);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);

    new i = 0;

    foreach(new id : IdGen(generator))
    {
        ASSERT_EQ(id, i++);
    }
}

Test:Foreach_Release()
{
    new Idgen:generator = Idgen_New(10);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);
    Idgen_NewID(generator);

    Idgen_ReleaseId(generator, 2);

    new expected[] = {0, 3}, i = 0;

    foreach(new id : IdGen(generator))
    {
        ASSERT_NE(id, 2);
        ASSERT_EQ(id, expected[i++]);
    }
}