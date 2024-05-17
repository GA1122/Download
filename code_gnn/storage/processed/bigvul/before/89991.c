size_t ZSTD_CCtx_refPrefix_advanced(
        ZSTD_CCtx* cctx, const void* prefix, size_t prefixSize, ZSTD_dictContentType_e dictContentType)
{
    if (cctx->streamStage != zcss_init) return ERROR(stage_wrong);
    cctx->cdict = NULL;    
    cctx->prefixDict.dict = prefix;
    cctx->prefixDict.dictSize = prefixSize;
    cctx->prefixDict.dictContentType = dictContentType;
    return 0;
}