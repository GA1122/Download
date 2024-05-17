size_t ZSTD_compressBegin_advanced(ZSTD_CCtx* cctx,
                             const void* dict, size_t dictSize,
                                   ZSTD_parameters params, unsigned long long pledgedSrcSize)
{
    ZSTD_CCtx_params const cctxParams =
            ZSTD_assignParamsToCCtxParams(cctx->requestedParams, params);
    return ZSTD_compressBegin_advanced_internal(cctx,
                                            dict, dictSize, ZSTD_dct_auto, ZSTD_dtlm_fast,
                                            NULL  ,
                                            cctxParams, pledgedSrcSize);
}