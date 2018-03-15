#define LIBETC2
#define ETC2LIB_API __cdecl
#include "libetc2.h"
#include "Etc2Compress.cpp"


static const int CompareBlocksColorMaxError = (kGreen + kRed + kBlue) * 16 * (255 * 255);
static const int CompareBlocksAlphaMaxError = 16 * (255 * 255);
static const int ETC2_SCRATCHMEMSIZE = (sizeof(AdjustStateColorGroup) * 2 + 0x1FFFE) / 0x1FFFE * 0x1FFFE; // ~ 2.5MB
static bool InitLevelErrorsDone = false;

struct etc2comp* __cdecl etc2_compressNew()
{
    struct etc2comp* ret = (struct etc2comp*)malloc(ETC2_SCRATCHMEMSIZE);
    if (!InitLevelErrorsDone)
    {
        InitLevelErrors();
        InitLevelErrorsDone = true;
    }
    return ret;
}

void __cdecl etc2_compressDelete(struct etc2comp* comp)
{
    free(comp);
}

int __cdecl etc2_compressEtc1(struct etc2comp* comp, void *output, const void* src, int srcStride)
{
    uintptr_t scratchAddr = (uintptr_t)((void*)comp);
    scratchAddr = (scratchAddr + 64) & ~uintptr_t(63); // make scratchAddr is 64-bytes aligned
    return CompressBlockColor1((uint8_t*)output, (const uint8_t*)src, srcStride, CompareBlocksColorMaxError, (AdjustStateColorGroup*)(void*)scratchAddr);
}

int __cdecl etc2_compressEtc2_RGB(struct etc2comp* comp, void *output, const void* src, int srcStride)
{
    int err = etc2_compressEtc1(comp, output, src, srcStride);
    if (err > 0)
        err = CompressBlockColorEnhanced((uint8_t*)output, (const uint8_t*)src, srcStride, err);
    return err;
}

int __cdecl etc2_compressEtc2_A(struct etc2comp*, void *output, const void* src, int srcStride)
{
    return CompressBlockAlphaEnhanced((uint8_t*)output, (const uint8_t*)src, srcStride, CompareBlocksAlphaMaxError);
}

int __cdecl etc2_compressEtc2_RGBA(struct etc2comp* comp, void *output, const void* src, int srcStride)
{
    CompressBlockAlphaEnhanced((uint8_t*)output, (const uint8_t*)src, srcStride, CompareBlocksAlphaMaxError);
    return etc2_compressEtc2_RGB(comp, (uint8_t*)output + 8, src, srcStride);
}

void __cdecl etc2_decompressEtc2_RGB(void *output, const void* src, int dstStride)
{
    DecompressBlockColorEnhanced((const uint8_t*)src, (uint8_t*)output, dstStride);
}

void __cdecl etc2_decompressEtc2_A(void *output, const void* src, int dstStride)
{
    DecompressBlockAlphaEnhanced((const uint8_t*)src, (uint8_t*)output, dstStride);
}

void __cdecl etc2_decompressEtc2_RGBA(void *output, const void* src, int dstStride)
{
    DecompressBlockAlphaEnhanced((const uint8_t*)src, (uint8_t*)output, dstStride);
    DecompressBlockColorEnhanced((const uint8_t*)src, (uint8_t*)output, dstStride);
}
