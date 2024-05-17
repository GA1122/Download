static size_t ZSTD_loadDictionaryContent(ZSTD_matchState_t* ms,
                                         ZSTD_CCtx_params const* params,
                                         const void* src, size_t srcSize,
                                         ZSTD_dictTableLoadMethod_e dtlm)
{
    const BYTE* const ip = (const BYTE*) src;
    const BYTE* const iend = ip + srcSize;

    ZSTD_window_update(&ms->window, src, srcSize);
    ms->loadedDictEnd = params->forceWindow ? 0 : (U32)(iend - ms->window.base);

     
    ZSTD_assertEqualCParams(params->cParams, ms->cParams);

    if (srcSize <= HASH_READ_SIZE) return 0;

    switch(params->cParams.strategy)
    {
    case ZSTD_fast:
        ZSTD_fillHashTable(ms, iend, dtlm);
        break;
    case ZSTD_dfast:
        ZSTD_fillDoubleHashTable(ms, iend, dtlm);
        break;

    case ZSTD_greedy:
    case ZSTD_lazy:
    case ZSTD_lazy2:
        if (srcSize >= HASH_READ_SIZE)
            ZSTD_insertAndFindFirstIndex(ms, iend-HASH_READ_SIZE);
        break;

    case ZSTD_btlazy2:    
    case ZSTD_btopt:
    case ZSTD_btultra:
        if (srcSize >= HASH_READ_SIZE)
            ZSTD_updateTree(ms, iend-HASH_READ_SIZE, iend);
        break;

    default:
        assert(0);   
    }

    ms->nextToUpdate = (U32)(iend - ms->window.base);
    return 0;
}