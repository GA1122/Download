size_t ZSTD_CCtx_refCDict(ZSTD_CCtx* cctx, const ZSTD_CDict* cdict)
{
    if (cctx->streamStage != zcss_init) return ERROR(stage_wrong);
    cctx->cdict = cdict;
    memset(&cctx->prefixDict, 0, sizeof(cctx->prefixDict));   
    return 0;
}