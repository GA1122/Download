size_t ZSTD_compress_usingCDict_advanced(ZSTD_CCtx* cctx,
                                void* dst, size_t dstCapacity,
                                const void* src, size_t srcSize,
                                const ZSTD_CDict* cdict, ZSTD_frameParameters fParams)
{
    CHECK_F (ZSTD_compressBegin_usingCDict_advanced(cctx, cdict, fParams, srcSize));    
    return ZSTD_compressEnd(cctx, dst, dstCapacity, src, srcSize);
}