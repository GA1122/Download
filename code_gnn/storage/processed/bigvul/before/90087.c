ZSTD_CCtx* ZSTD_initStaticCCtx(void *workspace, size_t workspaceSize)
{
    ZSTD_CCtx* const cctx = (ZSTD_CCtx*) workspace;
    if (workspaceSize <= sizeof(ZSTD_CCtx)) return NULL;   
    if ((size_t)workspace & 7) return NULL;   
    memset(workspace, 0, workspaceSize);    
    cctx->staticSize = workspaceSize;
    cctx->workSpace = (void*)(cctx+1);
    cctx->workSpaceSize = workspaceSize - sizeof(ZSTD_CCtx);

     
    if (cctx->workSpaceSize < HUF_WORKSPACE_SIZE + 2 * sizeof(ZSTD_compressedBlockState_t)) return NULL;
    assert(((size_t)cctx->workSpace & (sizeof(void*)-1)) == 0);    
    cctx->blockState.prevCBlock = (ZSTD_compressedBlockState_t*)cctx->workSpace;
    cctx->blockState.nextCBlock = cctx->blockState.prevCBlock + 1;
    {
        void* const ptr = cctx->blockState.nextCBlock + 1;
        cctx->entropyWorkspace = (U32*)ptr;
    }
    cctx->bmi2 = ZSTD_cpuid_bmi2(ZSTD_cpuid());
    return cctx;
}
