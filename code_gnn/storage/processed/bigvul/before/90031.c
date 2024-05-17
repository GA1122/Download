ZSTD_compress_insertDictionary(ZSTD_compressedBlockState_t* bs,
                               ZSTD_matchState_t* ms,
                         const ZSTD_CCtx_params* params,
                         const void* dict, size_t dictSize,
                               ZSTD_dictContentType_e dictContentType,
                               ZSTD_dictTableLoadMethod_e dtlm,
                               void* workspace)
{
    DEBUGLOG(4, "ZSTD_compress_insertDictionary (dictSize=%u)", (U32)dictSize);
    if ((dict==NULL) || (dictSize<=8)) return 0;

    ZSTD_reset_compressedBlockState(bs);

     
    if (dictContentType == ZSTD_dct_rawContent)
        return ZSTD_loadDictionaryContent(ms, params, dict, dictSize, dtlm);

    if (MEM_readLE32(dict) != ZSTD_MAGIC_DICTIONARY) {
        if (dictContentType == ZSTD_dct_auto) {
            DEBUGLOG(4, "raw content dictionary detected");
            return ZSTD_loadDictionaryContent(ms, params, dict, dictSize, dtlm);
        }
        if (dictContentType == ZSTD_dct_fullDict)
            return ERROR(dictionary_wrong);
        assert(0);    
    }

     
    return ZSTD_loadZstdDictionary(bs, ms, params, dict, dictSize, dtlm, workspace);
}