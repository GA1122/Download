size_t ZSTD_compressBlock(ZSTD_CCtx* cctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
    size_t const blockSizeMax = ZSTD_getBlockSize(cctx);
    if (srcSize > blockSizeMax) return ERROR(srcSize_wrong);

    return ZSTD_compressContinue_internal(cctx, dst, dstCapacity, src, srcSize, 0  , 0  );
}