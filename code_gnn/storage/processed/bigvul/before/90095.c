static ZSTD_CCtx_params ZSTD_makeCCtxParamsFromCParams(
        ZSTD_compressionParameters cParams)
{
    ZSTD_CCtx_params cctxParams;
    memset(&cctxParams, 0, sizeof(cctxParams));
    cctxParams.cParams = cParams;
    cctxParams.compressionLevel = ZSTD_CLEVEL_DEFAULT;   
    assert(!ZSTD_checkCParams(cParams));
    cctxParams.fParams.contentSizeFlag = 1;
    return cctxParams;
}