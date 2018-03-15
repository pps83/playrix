#ifndef LIBETC2_H_
#define LIBETC2_H_

#ifndef ETC2LIB_API
#define ETC2LIB_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct etc2comp* ETC2LIB_API etc2_compressNew();
void ETC2LIB_API etc2_compressDelete(struct etc2comp* comp);

int ETC2LIB_API etc2_compressEtc1(struct etc2comp* comp, void *output, const void* src, int srcStride = 16);
int ETC2LIB_API etc2_compressEtc2_RGB(struct etc2comp* comp, void *output, const void* src, int srcStride = 16);
int ETC2LIB_API etc2_compressEtc2_A(struct etc2comp* comp, void *output, const void* src, int srcStride = 16);
int ETC2LIB_API etc2_compressEtc2_RGBA(struct etc2comp* comp, void *output, const void* src, int srcStride = 16);

void ETC2LIB_API etc2_decompressEtc2_RGB(void *output, const void* src, int dstStride = 16);
void ETC2LIB_API etc2_decompressEtc2_A(void *output, const void* src, int dstStride = 16);
void ETC2LIB_API etc2_decompressEtc2_RGBA(void *output, const void* src, int dstStride = 16);

static inline void etc2_decompressEtc1(void *output, const void* src, int dstStride = 16)
{
    return etc2_decompressEtc2_RGB(output, src, dstStride);
}

#ifdef __cplusplus
}
#endif

#endif /* LIBETC2_H_ */
