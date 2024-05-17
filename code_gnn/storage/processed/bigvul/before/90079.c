static size_t ZSTD_initCDict_internal(
                    ZSTD_CDict* cdict,
              const void* dictBuffer, size_t dictSize,
                    ZSTD_dictLoadMethod_e dictLoadMethod,
                    ZSTD_dictContentType_e dictContentType,
                    ZSTD_compressionParameters cParams)
{
    DEBUGLOG(3, "ZSTD_initCDict_internal (dictContentType:%u)", (U32)dictContentType);
    assert(!ZSTD_checkCParams(cParams));
    cdict->matchState.cParams = cParams;
    if ((dictLoadMethod == ZSTD_dlm_byRef) || (!dictBuffer) || (!dictSize)) {
        cdict->dictBuffer = NULL;
        cdict->dictContent = dictBuffer;
    } else {
        void* const internalBuffer = ZSTD_malloc(dictSize, cdict->customMem);
        cdict->dictBuffer = internalBuffer;
        cdict->dictContent = internalBuffer;
        if (!internalBuffer) return ERROR(memory_allocation);
        memcpy(internalBuffer, dictBuffer, dictSize);
    }
    cdict->dictContentSize = dictSize;

     
    ZSTD_reset_compressedBlockState(&cdict->cBlockState);
    {   void* const end = ZSTD_reset_matchState(
                &cdict->matchState,
                (U32*)cdict->workspace + HUF_WORKSPACE_SIZE_U32,
                &cParams, ZSTDcrp_continue,   0);
        assert(end == (char*)cdict->workspace + cdict->workspaceSize);
        (void)end;
    }
     
    {   ZSTD_CCtx_params params;
        memset(&params, 0, sizeof(params));
        params.compressionLevel = ZSTD_CLEVEL_DEFAULT;
        params.fParams.contentSizeFlag = 1;
        params.cParams = cParams;
        {   size_t const dictID = ZSTD_compress_insertDictionary(
                    &cdict->cBlockState, &cdict->matchState, &params,
                    cdict->dictContent, cdict->dictContentSize,
                    dictContentType, ZSTD_dtlm_full, cdict->workspace);
            if (ZSTD_isError(dictID)) return dictID;
            assert(dictID <= (size_t)(U32)-1);
            cdict->dictID = (U32)dictID;
        }
    }

    return 0;
}