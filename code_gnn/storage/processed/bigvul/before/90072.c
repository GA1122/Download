ZSTD_compressionParameters ZSTD_getCParams(int compressionLevel, unsigned long long srcSizeHint, size_t dictSize)
{
    size_t const addedSize = srcSizeHint ? 0 : 500;
    U64 const rSize = srcSizeHint+dictSize ? srcSizeHint+dictSize+addedSize : (U64)-1;
    U32 const tableID = (rSize <= 256 KB) + (rSize <= 128 KB) + (rSize <= 16 KB);    
    int row = compressionLevel;
    DEBUGLOG(5, "ZSTD_getCParams (cLevel=%i)", compressionLevel);
    if (compressionLevel == 0) row = ZSTD_CLEVEL_DEFAULT;    
    if (compressionLevel < 0) row = 0;    
    if (compressionLevel > ZSTD_MAX_CLEVEL) row = ZSTD_MAX_CLEVEL;
    {   ZSTD_compressionParameters cp = ZSTD_defaultCParameters[tableID][row];
        if (compressionLevel < 0) cp.targetLength = (unsigned)(-compressionLevel);    
        return ZSTD_adjustCParams_internal(cp, srcSizeHint, dictSize); }

}
