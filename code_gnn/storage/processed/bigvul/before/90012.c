size_t ZSTD_compressBegin_usingCDict_advanced(
    ZSTD_CCtx* const cctx, const ZSTD_CDict* const cdict,
    ZSTD_frameParameters const fParams, unsigned long long const pledgedSrcSize)
{
    DEBUGLOG(4, "ZSTD_compressBegin_usingCDict_advanced");
    if (cdict==NULL) return ERROR(dictionary_wrong);
    {   ZSTD_CCtx_params params = cctx->requestedParams;
        params.cParams = ZSTD_getCParamsFromCDict(cdict);
         
        if (pledgedSrcSize != ZSTD_CONTENTSIZE_UNKNOWN) {
            U32 const limitedSrcSize = (U32)MIN(pledgedSrcSize, 1U << 19);
            U32 const limitedSrcLog = limitedSrcSize > 1 ? ZSTD_highbit32(limitedSrcSize - 1) + 1 : 1;
            params.cParams.windowLog = MAX(params.cParams.windowLog, limitedSrcLog);
        }
        params.fParams = fParams;
        return ZSTD_compressBegin_internal(cctx,
                                           NULL, 0, ZSTD_dct_auto, ZSTD_dtlm_fast,
                                           cdict,
                                           params, pledgedSrcSize,
                                           ZSTDb_not_buffered);
    }
}