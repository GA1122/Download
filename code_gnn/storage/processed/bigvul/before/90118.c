size_t ZSTD_sizeof_CCtx(const ZSTD_CCtx* cctx)
{
    if (cctx==NULL) return 0;    
    return sizeof(*cctx) + cctx->workSpaceSize
           + ZSTD_sizeof_CDict(cctx->cdictLocal)
           + ZSTD_sizeof_mtctx(cctx);
}
