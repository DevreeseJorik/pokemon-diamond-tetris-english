/*
 * Concatenates data stored in multiple eggs and copies it to the specified address. Executes if specified.
 *
 * Program used for marking ACE. If the egg with this program is marked, it will copy the data from the following eggs
 * to address 0x023A0000.
 * The number of eggs to copy and whether to execute after copying is specified in the header of the first egg;
 * `eggNum` specifies the number of eggs to copy.
 * When `isAutoRun` is 1, it will execute after copying.
 */

#include "acehelper.h"
#include "eggcommon.h"

#define defaultTarget 0x023A0000
#define offsetScale 0x100

#define call(funcAddr, mark) (((void (*)(u8))funcAddr)(mark))
static inline void _memcpy(void *target, const void *source, u32 size);

void main(BoxPokemon *selected, u8 mark)
{
    // Address where data will be unpacked
    EggsData *target = (EggsData *)defaultTarget;
    // `source` points to the Pokémon immediately after the marked Pokémon
    BoxPokemonAsData *source = (BoxPokemonAsData *)(selected + 1);
    u8 eggNum = source[0].header.eggNum;

    // Copy egg data to the specified address
    for (int i = 0; i < eggNum; i++)
    {
        // Assuming shuffle type is 0 (ABCD).
        // To avoid crashes, species IDs (0x8~0xc) and move IDs (0x28~0x30) are not used.
        // Copy 0xc~0x28
        _memcpy(
            (u8 *)&target->data[i],
            (u8 *)&source[i].data + EGG_DATA1_OFFSET,
            EGG_DATA1_SIZE);
        // Copy 0x30~0x88
        _memcpy(
            (u8 *)&target->data[i] + EGG_DATA1_SIZE,
            (u8 *)&source[i].data + EGG_DATA2_OFFSET,
            EGG_DATA2_SIZE);
    }

    // Execute
    if (source[0].header.isAutoRun == 1)
    {
        // Write back cache lines
        DC_StoreAll();
        IC_InvalidateAll();
        
        call(((u32)target->data) + 1, mark);    // +1 for thumb mode
    }
}

static inline void _memcpy(void *target, const void *source, u32 size)
{
    register const void *_source __asm__("r0") = source;
    register void *_target __asm__("r1") = target;
    register u32 _hwordCount __asm__("r2") = size / 2;
    __asm__ volatile(
        "swi #0xb"
        :
        : "r"(_source), "r"(_target), "r"(_hwordCount)
        : "r3");
}