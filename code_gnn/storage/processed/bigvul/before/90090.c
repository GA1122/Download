static void ZSTD_invalidateMatchState(ZSTD_matchState_t* ms)
{
    ZSTD_window_clear(&ms->window);

    ms->nextToUpdate = ms->window.dictLimit + 1;
    ms->nextToUpdate3 = ms->window.dictLimit + 1;
    ms->loadedDictEnd = 0;
    ms->opt.litLengthSum = 0;   
    ms->dictMatchState = NULL;
}