ZSTD_CStream* ZSTD_createCStream_advanced(ZSTD_customMem customMem)
{    
    return ZSTD_createCCtx_advanced(customMem);
}
