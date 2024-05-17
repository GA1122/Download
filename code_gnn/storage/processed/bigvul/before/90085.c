size_t ZSTD_initCStream_usingCDict_advanced(ZSTD_CStream* zcs,
                                            const ZSTD_CDict* cdict,
                                            ZSTD_frameParameters fParams,
                                            unsigned long long pledgedSrcSize)
{
    DEBUGLOG(4, "ZSTD_initCStream_usingCDict_advanced");
    if (!cdict) return ERROR(dictionary_wrong);  
    {   ZSTD_CCtx_params params = zcs->requestedParams;
        params.cParams = ZSTD_getCParamsFromCDict(cdict);
        params.fParams = fParams;
        return ZSTD_initCStream_internal(zcs,
                                NULL, 0, cdict,
                                params, pledgedSrcSize);
    }
}