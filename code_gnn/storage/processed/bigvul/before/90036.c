static size_t ZSTD_continueCCtx(ZSTD_CCtx* cctx, ZSTD_CCtx_params params, U64 pledgedSrcSize)
{
    size_t const windowSize = MAX(1, (size_t)MIN(((U64)1 << params.cParams.windowLog), pledgedSrcSize));
    size_t const blockSize = MIN(ZSTD_BLOCKSIZE_MAX, windowSize);
    DEBUGLOG(4, "ZSTD_continueCCtx: re-use context in place");

    cctx->blockSize = blockSize;    
    cctx->appliedParams = params;
    cctx->blockState.matchState.cParams = params.cParams;
    cctx->pledgedSrcSizePlusOne = pledgedSrcSize+1;
    cctx->consumedSrcSize = 0;
    cctx->producedCSize = 0;
    if (pledgedSrcSize == ZSTD_CONTENTSIZE_UNKNOWN)
        cctx->appliedParams.fParams.contentSizeFlag = 0;
    DEBUGLOG(4, "pledged content size : %u ; flag : %u",
        (U32)pledgedSrcSize, cctx->appliedParams.fParams.contentSizeFlag);
    cctx->stage = ZSTDcs_init;
    cctx->dictID = 0;
    if (params.ldmParams.enableLdm)
        ZSTD_window_clear(&cctx->ldmState.window);
    ZSTD_referenceExternalSequences(cctx, NULL, 0);
    ZSTD_invalidateMatchState(&cctx->blockState.matchState);
    ZSTD_reset_compressedBlockState(cctx->blockState.prevCBlock);
    XXH64_reset(&cctx->xxhState, 0);
    return 0;
}