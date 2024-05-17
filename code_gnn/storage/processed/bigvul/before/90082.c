size_t ZSTD_initCStream_internal(ZSTD_CStream* zcs,
                    const void* dict, size_t dictSize, const ZSTD_CDict* cdict,
                    ZSTD_CCtx_params params, unsigned long long pledgedSrcSize)
{
    DEBUGLOG(4, "ZSTD_initCStream_internal");
    params.cParams = ZSTD_getCParamsFromCCtxParams(&params, pledgedSrcSize, dictSize);
    assert(!ZSTD_isError(ZSTD_checkCParams(params.cParams)));
    assert(!((dict) && (cdict)));   

    if (dict && dictSize >= 8) {
        DEBUGLOG(4, "loading dictionary of size %u", (U32)dictSize);
        if (zcs->staticSize) {    
             
            return ERROR(memory_allocation);
        }
        ZSTD_freeCDict(zcs->cdictLocal);
        zcs->cdictLocal = ZSTD_createCDict_advanced(dict, dictSize,
                                            ZSTD_dlm_byCopy, ZSTD_dct_auto,
                                            params.cParams, zcs->customMem);
        zcs->cdict = zcs->cdictLocal;
        if (zcs->cdictLocal == NULL) return ERROR(memory_allocation);
    } else {
        if (cdict) {
            params.cParams = ZSTD_getCParamsFromCDict(cdict);   
        }
        ZSTD_freeCDict(zcs->cdictLocal);
        zcs->cdictLocal = NULL;
        zcs->cdict = cdict;
    }

    return ZSTD_resetCStream_internal(zcs, NULL, 0, ZSTD_dct_auto, zcs->cdict, params, pledgedSrcSize);
}