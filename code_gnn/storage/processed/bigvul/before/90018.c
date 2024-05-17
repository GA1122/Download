size_t ZSTD_compressContinue (ZSTD_CCtx* cctx,
                              void* dst, size_t dstCapacity,
                        const void* src, size_t srcSize)
{
    DEBUGLOG(5, "ZSTD_compressContinue (srcSize=%u)", (U32)srcSize);
    return ZSTD_compressContinue_internal(cctx, dst, dstCapacity, src, srcSize, 1  , 0  );
}