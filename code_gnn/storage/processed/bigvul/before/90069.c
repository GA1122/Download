size_t ZSTD_freeCDict(ZSTD_CDict* cdict)
{
    if (cdict==NULL) return 0;    
    {   ZSTD_customMem const cMem = cdict->customMem;
        ZSTD_free(cdict->workspace, cMem);
        ZSTD_free(cdict->dictBuffer, cMem);
        ZSTD_free(cdict, cMem);
        return 0;
    }
}
