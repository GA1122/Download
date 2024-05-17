size_t ZSTD_sizeof_CDict(const ZSTD_CDict* cdict)
{
    if (cdict==NULL) return 0;    
    DEBUGLOG(5, "sizeof(*cdict) : %u", (U32)sizeof(*cdict));
    return cdict->workspaceSize + (cdict->dictBuffer ? cdict->dictContentSize : 0) + sizeof(*cdict);
}