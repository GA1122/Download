static size_t ZSTD_resetCCtx_byAttachingCDict(
    ZSTD_CCtx* cctx,
    const ZSTD_CDict* cdict,
    ZSTD_CCtx_params params,
    U64 pledgedSrcSize,
    ZSTD_buffered_policy_e zbuff)
{
    {
        const ZSTD_compressionParameters *cdict_cParams = &cdict->matchState.cParams;
        unsigned const windowLog = params.cParams.windowLog;
        assert(windowLog != 0);
         
        params.cParams = ZSTD_adjustCParams_internal(*cdict_cParams, pledgedSrcSize, 0);
        params.cParams.windowLog = windowLog;
        ZSTD_resetCCtx_internal(cctx, params, pledgedSrcSize,
                                ZSTDcrp_continue, zbuff);
        assert(cctx->appliedParams.cParams.strategy == cdict_cParams->strategy);
    }

    {
        const U32 cdictEnd = (U32)( cdict->matchState.window.nextSrc
                                  - cdict->matchState.window.base);
        const U32 cdictLen = cdictEnd - cdict->matchState.window.dictLimit;
        if (cdictLen == 0) {
             
            DEBUGLOG(4, "skipping attaching empty dictionary");
        } else {
            DEBUGLOG(4, "attaching dictionary into context");
            cctx->blockState.matchState.dictMatchState = &cdict->matchState;

             
            if (cctx->blockState.matchState.window.dictLimit < cdictEnd) {
                cctx->blockState.matchState.window.nextSrc =
                    cctx->blockState.matchState.window.base + cdictEnd;
                ZSTD_window_clear(&cctx->blockState.matchState.window);
            }
            cctx->blockState.matchState.loadedDictEnd = cctx->blockState.matchState.window.dictLimit;
        }
    }

    cctx->dictID = cdict->dictID;

     
    memcpy(cctx->blockState.prevCBlock, &cdict->cBlockState, sizeof(cdict->cBlockState));

    return 0;
}
