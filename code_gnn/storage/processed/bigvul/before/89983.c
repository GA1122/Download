size_t ZSTD_CCtxParams_init_advanced(ZSTD_CCtx_params* cctxParams, ZSTD_parameters params)
{
    if (!cctxParams) { return ERROR(GENERIC); }
    CHECK_F( ZSTD_checkCParams(params.cParams) );
    memset(cctxParams, 0, sizeof(*cctxParams));
    cctxParams->cParams = params.cParams;
    cctxParams->fParams = params.fParams;
    cctxParams->compressionLevel = ZSTD_CLEVEL_DEFAULT;    
    assert(!ZSTD_checkCParams(params.cParams));
    return 0;
}