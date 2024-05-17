ZSTD_compressSequences(seqStore_t* seqStorePtr,
                       const ZSTD_entropyCTables_t* prevEntropy,
                             ZSTD_entropyCTables_t* nextEntropy,
                       const ZSTD_CCtx_params* cctxParams,
                             void* dst, size_t dstCapacity,
                             size_t srcSize,
                             void* workspace, size_t wkspSize,
                             int bmi2)
{
    size_t const cSize = ZSTD_compressSequences_internal(
                            seqStorePtr, prevEntropy, nextEntropy, cctxParams,
                            dst, dstCapacity,
                            workspace, wkspSize, bmi2);
    if (cSize == 0) return 0;
     
    if ((cSize == ERROR(dstSize_tooSmall)) & (srcSize <= dstCapacity))
        return 0;   
    if (ZSTD_isError(cSize)) return cSize;

     
    {   size_t const maxCSize = srcSize - ZSTD_minGain(srcSize, cctxParams->cParams.strategy);
        if (cSize >= maxCSize) return 0;   
    }

    return cSize;
}
