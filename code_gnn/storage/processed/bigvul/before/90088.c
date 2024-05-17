const ZSTD_CDict* ZSTD_initStaticCDict(
                                 void* workspace, size_t workspaceSize,
                           const void* dict, size_t dictSize,
                                 ZSTD_dictLoadMethod_e dictLoadMethod,
                                 ZSTD_dictContentType_e dictContentType,
                                 ZSTD_compressionParameters cParams)
{
    size_t const matchStateSize = ZSTD_sizeof_matchState(&cParams,   0);
    size_t const neededSize = sizeof(ZSTD_CDict) + (dictLoadMethod == ZSTD_dlm_byRef ? 0 : dictSize)
                            + HUF_WORKSPACE_SIZE + matchStateSize;
    ZSTD_CDict* const cdict = (ZSTD_CDict*) workspace;
    void* ptr;
    if ((size_t)workspace & 7) return NULL;   
    DEBUGLOG(4, "(workspaceSize < neededSize) : (%u < %u) => %u",
        (U32)workspaceSize, (U32)neededSize, (U32)(workspaceSize < neededSize));
    if (workspaceSize < neededSize) return NULL;

    if (dictLoadMethod == ZSTD_dlm_byCopy) {
        memcpy(cdict+1, dict, dictSize);
        dict = cdict+1;
        ptr = (char*)workspace + sizeof(ZSTD_CDict) + dictSize;
    } else {
        ptr = cdict+1;
    }
    cdict->workspace = ptr;
    cdict->workspaceSize = HUF_WORKSPACE_SIZE + matchStateSize;

    if (ZSTD_isError( ZSTD_initCDict_internal(cdict,
                                              dict, dictSize,
                                              ZSTD_dlm_byRef, dictContentType,
                                              cParams) ))
        return NULL;

    return cdict;
}