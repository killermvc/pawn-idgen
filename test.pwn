#include <a_samp>

#include "pawn-idgen.inc"

main() {
    new Idgen:generator = Idgen_New(50);
    if(!Idgen_IsValidID(Idgen:0, _:generator)) {
        print("Idgen_New failed");
    } else {
        print("Idgen_New passed");
    }

    new ids[3];

    ids[0] = Idgen_NewID(generator);
    ids[1] = Idgen_NewID(generator);
    ids[2] = Idgen_NewID(generator);

    if(ids[0] != 0 || ids[1] != 1 || ids[2] != 2) {
        print("Idgen_NewID failed");
        printf("%d,%d,%d", ids[0], ids[1], ids[2]);
    } else {
        print("Idgen_NewID passed");
    }

    Idgen_ReleaseId(generator, ids[1]);
    if(Idgen_IsValidID(generator, ids[1])) {
        print("Idgen_ReleaseId failed");
    } else {
        print("Idgen_ReleaseId passed");
    }

    Idgen_ReleaseId(generator, ids[0]);

    new id[2];
    id[1] = Idgen_NewID(generator);
    id[0] = Idgen_NewID(generator);

    if(id[0] != ids[0] || id[1] != ids[1]) {
        print("Idgen_NewID with released ids failed");
    } else {
        print("Idgen_NewID with released ids passed");
    }

    new Idgen:generator2 = Idgen_New(UNLIMITED_CAPACITY);

    new id2 = Idgen_NewID(generator2);
    if(id2 != 0) {
        print("Idgen_NewID with two different generators failed");
    } else {
        print("Idgen_NewID with two different generators passed");
    }

    Idgen_Delete(generator2);

    if(Idgen_IsValidGenerator(generator2)) {
        print("Idgen_Delete failed");
    } else {
        print("Idgen_Delete passed");
    }

}