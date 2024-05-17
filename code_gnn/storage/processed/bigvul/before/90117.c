static int ZSTD_shouldAttachDict(const ZSTD_CDict* cdict,
                                 ZSTD_CCtx_params params,
                                 U64 pledgedSrcSize)
{
    size_t cutoff = attachDictSizeCutoffs[cdict->matchState.cParams.strategy];
    return ( pledgedSrcSize <= cutoff
          || pledgedSrcSize == ZSTD_CONTENTSIZE_UNKNOWN
          || params.attachDictPref == ZSTD_dictForceAttach )
        && params.attachDictPref != ZSTD_dictForceCopy
        && !params.forceWindow;  
}