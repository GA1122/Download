static ZSTD_CCtx_params ZSTD_assignParamsToCCtxParams(
        ZSTD_CCtx_params cctxParams, ZSTD_parameters params)
{
    ZSTD_CCtx_params ret = cctxParams;
    ret.cParams = params.cParams;
    ret.fParams = params.fParams;
    ret.compressionLevel = ZSTD_CLEVEL_DEFAULT;    
    assert(!ZSTD_checkCParams(params.cParams));
    return ret;
}