size_t ZSTD_freeCCtx(ZSTD_CCtx* cctx)
{
    if (cctx==NULL) return 0;    
    if (cctx->staticSize) return ERROR(memory_allocation);    
    ZSTD_freeCCtxContent(cctx);
    ZSTD_free(cctx, cctx->customMem);
    return 0;
}
