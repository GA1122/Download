size_t ZSTD_freeCStream(ZSTD_CStream* zcs)
{
    return ZSTD_freeCCtx(zcs);    
}