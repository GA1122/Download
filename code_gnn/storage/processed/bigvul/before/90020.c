size_t ZSTD_compressEnd (ZSTD_CCtx* cctx,
                         void* dst, size_t dstCapacity,
                   const void* src, size_t srcSize)
{
    size_t endResult;
    size_t const cSize = ZSTD_compressContinue_internal(cctx,
                                dst, dstCapacity, src, srcSize,
                                1  , 1  );
    if (ZSTD_isError(cSize)) return cSize;
    endResult = ZSTD_writeEpilogue(cctx, (char*)dst + cSize, dstCapacity-cSize);
    if (ZSTD_isError(endResult)) return endResult;
    assert(!(cctx->appliedParams.fParams.contentSizeFlag && cctx->pledgedSrcSizePlusOne == 0));
    if (cctx->pledgedSrcSizePlusOne != 0) {   
        ZSTD_STATIC_ASSERT(ZSTD_CONTENTSIZE_UNKNOWN == (unsigned long long)-1);
        DEBUGLOG(4, "end of frame : controlling src size");
        if (cctx->pledgedSrcSizePlusOne != cctx->consumedSrcSize+1) {
            DEBUGLOG(4, "error : pledgedSrcSize = %u, while realSrcSize = %u",
                (U32)cctx->pledgedSrcSizePlusOne-1, (U32)cctx->consumedSrcSize);
            return ERROR(srcSize_wrong);
    }   }
    return cSize + endResult;
}