ZSTD_reset_matchState(ZSTD_matchState_t* ms,
                      void* ptr,
                const ZSTD_compressionParameters* cParams,
                      ZSTD_compResetPolicy_e const crp, U32 const forCCtx)
{
    size_t const chainSize = (cParams->strategy == ZSTD_fast) ? 0 : ((size_t)1 << cParams->chainLog);
    size_t const hSize = ((size_t)1) << cParams->hashLog;
    U32    const hashLog3 = (forCCtx && cParams->searchLength==3) ? MIN(ZSTD_HASHLOG3_MAX, cParams->windowLog) : 0;
    size_t const h3Size = ((size_t)1) << hashLog3;
    size_t const tableSpace = (chainSize + hSize + h3Size) * sizeof(U32);

    assert(((size_t)ptr & 3) == 0);

    ms->hashLog3 = hashLog3;
    memset(&ms->window, 0, sizeof(ms->window));
    ms->window.dictLimit = 1;     
    ms->window.lowLimit = 1;      
    ms->window.nextSrc = ms->window.base + 1;    
    ZSTD_invalidateMatchState(ms);

     
    if (forCCtx && ((cParams->strategy == ZSTD_btopt) | (cParams->strategy == ZSTD_btultra))) {
        DEBUGLOG(4, "reserving optimal parser space");
        ms->opt.litFreq = (U32*)ptr;
        ms->opt.litLengthFreq = ms->opt.litFreq + (1<<Litbits);
        ms->opt.matchLengthFreq = ms->opt.litLengthFreq + (MaxLL+1);
        ms->opt.offCodeFreq = ms->opt.matchLengthFreq + (MaxML+1);
        ptr = ms->opt.offCodeFreq + (MaxOff+1);
        ms->opt.matchTable = (ZSTD_match_t*)ptr;
        ptr = ms->opt.matchTable + ZSTD_OPT_NUM+1;
        ms->opt.priceTable = (ZSTD_optimal_t*)ptr;
        ptr = ms->opt.priceTable + ZSTD_OPT_NUM+1;
    }

     
    DEBUGLOG(4, "reset table : %u", crp!=ZSTDcrp_noMemset);
    assert(((size_t)ptr & 3) == 0);   
    if (crp!=ZSTDcrp_noMemset) memset(ptr, 0, tableSpace);    
    ms->hashTable = (U32*)(ptr);
    ms->chainTable = ms->hashTable + hSize;
    ms->hashTable3 = ms->chainTable + chainSize;
    ptr = ms->hashTable3 + h3Size;

    ms->cParams = *cParams;

    assert(((size_t)ptr & 3) == 0);
    return ptr;
}