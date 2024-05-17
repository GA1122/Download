ZSTD_adjustCParams_internal(ZSTD_compressionParameters cPar,
                            unsigned long long srcSize,
                            size_t dictSize)
{
    static const U64 minSrcSize = 513;  
    static const U64 maxWindowResize = 1ULL << (ZSTD_WINDOWLOG_MAX-1);
    assert(ZSTD_checkCParams(cPar)==0);

    if (dictSize && (srcSize+1<2)   )
        srcSize = minSrcSize;   
    else if (srcSize == 0)
        srcSize = ZSTD_CONTENTSIZE_UNKNOWN;   

     
    if ( (srcSize < maxWindowResize)
      && (dictSize < maxWindowResize) )  {
        U32 const tSize = (U32)(srcSize + dictSize);
        static U32 const hashSizeMin = 1 << ZSTD_HASHLOG_MIN;
        U32 const srcLog = (tSize < hashSizeMin) ? ZSTD_HASHLOG_MIN :
                            ZSTD_highbit32(tSize-1) + 1;
        if (cPar.windowLog > srcLog) cPar.windowLog = srcLog;
    }
    if (cPar.hashLog > cPar.windowLog+1) cPar.hashLog = cPar.windowLog+1;
    {   U32 const cycleLog = ZSTD_cycleLog(cPar.chainLog, cPar.strategy);
        if (cycleLog > cPar.windowLog)
            cPar.chainLog -= (cycleLog - cPar.windowLog);
    }

    if (cPar.windowLog < ZSTD_WINDOWLOG_ABSOLUTEMIN)
        cPar.windowLog = ZSTD_WINDOWLOG_ABSOLUTEMIN;   

    return cPar;
}