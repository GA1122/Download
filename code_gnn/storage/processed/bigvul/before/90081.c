size_t ZSTD_initCStream_advanced(ZSTD_CStream* zcs,
                                 const void* dict, size_t dictSize,
                                 ZSTD_parameters params, unsigned long long pledgedSrcSize)
{
    DEBUGLOG(4, "ZSTD_initCStream_advanced: pledgedSrcSize=%u, flag=%u",
                (U32)pledgedSrcSize, params.fParams.contentSizeFlag);
    CHECK_F( ZSTD_checkCParams(params.cParams) );
    if ((pledgedSrcSize==0) && (params.fParams.contentSizeFlag==0)) pledgedSrcSize = ZSTD_CONTENTSIZE_UNKNOWN;   
    zcs->requestedParams = ZSTD_assignParamsToCCtxParams(zcs->requestedParams, params);
    return ZSTD_initCStream_internal(zcs, dict, dictSize, NULL  , zcs->requestedParams, pledgedSrcSize);
}
