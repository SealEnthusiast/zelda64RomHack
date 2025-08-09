#ifndef Z64OBJECT_H
#define Z64OBJECT_H

// allow overriding object status limit externally using compiler flags
#ifndef OBJECT_EXCHANGE_BANK_MAX
    // object status limit, this controls how many
    // game objects can reside in memory simultaneously
    //
    // some notes:
    //  - oot default = 19, mm default = 35, so 45 is quite generous
    //  - no extra allocations are required for this higher limit, so
    //    there isn't much benefit to using the lower default of 19
    //    (PAL text entries are repurposed to fit them, it's free memory)
    //  - changing it back to 19 will cause the object status system to
    //    fall back to using the original buffer
    //  - otherwise, ObjectContext struct will be patched to contain an
    //    unused[0x508] that you can repurpose if you're pressed for memory
    //  - when changing this value, make sure you trigger a full rebuild
    //    using ./z64rom.exe --force
    #if Z64ROM_VERSION >= 1007000
    #   define OBJECT_EXCHANGE_BANK_MAX 45
    #else
    #   define OBJECT_EXCHANGE_BANK_MAX 19
    #endif
#endif

#define DEFINE_OBJECT(_0, enum) enum,
#define DEFINE_OBJECT_NULL(_0, enum) enum,
#define DEFINE_OBJECT_UNSET(enum) enum,

typedef enum {
    #include "tables/object_table.h"
    /* 0x0192 */ OBJECT_ID_MAX
} ObjectID;

#undef DEFINE_OBJECT
#undef DEFINE_OBJECT_NULL
#undef DEFINE_OBJECT_UNSET

#endif
