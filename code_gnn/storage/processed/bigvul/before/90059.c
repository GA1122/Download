size_t ZSTD_estimateCCtxSize_usingCCtxParams(const ZSTD_CCtx_params* params)
{
     
    if (params->nbWorkers > 0) { return ERROR(GENERIC); }
    {   ZSTD_compressionParameters const cParams =
                ZSTD_getCParamsFromCCtxParams(params, 0, 0);
        size_t const blockSize = MIN(ZSTD_BLOCKSIZE_MAX, (size_t)1 << cParams.windowLog);
        U32    const divider = (cParams.searchLength==3) ? 3 : 4;
        size_t const maxNbSeq = blockSize / divider;
        size_t const tokenSpace = WILDCOPY_OVERLENGTH + blockSize + 11*maxNbSeq;
        size_t const entropySpace = HUF_WORKSPACE_SIZE;
        size_t const blockStateSpace = 2 * sizeof(ZSTD_compressedBlockState_t);
        size_t const matchStateSize = ZSTD_sizeof_matchState(&cParams,   1);

        size_t const ldmSpace = ZSTD_ldm_getTableSize(params->ldmParams);
        size_t const ldmSeqSpace = ZSTD_ldm_getMaxNbSeq(params->ldmParams, blockSize) * sizeof(rawSeq);

        size_t const neededSpace = entropySpace + blockStateSpace + tokenSpace +
                                   matchStateSize + ldmSpace + ldmSeqSpace;

        DEBUGLOG(5, "sizeof(ZSTD_CCtx) : %u", (U32)sizeof(ZSTD_CCtx));
        DEBUGLOG(5, "estimate workSpace : %u", (U32)neededSpace);
        return sizeof(ZSTD_CCtx) + neededSpace;
    }
}