static size_t ZSTD_compressLiterals (ZSTD_hufCTables_t const* prevHuf,
                                     ZSTD_hufCTables_t* nextHuf,
                                     ZSTD_strategy strategy, int disableLiteralCompression,
                                     void* dst, size_t dstCapacity,
                               const void* src, size_t srcSize,
                                     void* workspace, size_t wkspSize,
                               const int bmi2)
{
    size_t const minGain = ZSTD_minGain(srcSize, strategy);
    size_t const lhSize = 3 + (srcSize >= 1 KB) + (srcSize >= 16 KB);
    BYTE*  const ostart = (BYTE*)dst;
    U32 singleStream = srcSize < 256;
    symbolEncodingType_e hType = set_compressed;
    size_t cLitSize;

    DEBUGLOG(5,"ZSTD_compressLiterals (disableLiteralCompression=%i)",
                disableLiteralCompression);

     
    memcpy(nextHuf, prevHuf, sizeof(*prevHuf));

    if (disableLiteralCompression)
        return ZSTD_noCompressLiterals(dst, dstCapacity, src, srcSize);

     
#   define COMPRESS_LITERALS_SIZE_MIN 63
    {   size_t const minLitSize = (prevHuf->repeatMode == HUF_repeat_valid) ? 6 : COMPRESS_LITERALS_SIZE_MIN;
        if (srcSize <= minLitSize) return ZSTD_noCompressLiterals(dst, dstCapacity, src, srcSize);
    }

    if (dstCapacity < lhSize+1) return ERROR(dstSize_tooSmall);    
    {   HUF_repeat repeat = prevHuf->repeatMode;
        int const preferRepeat = strategy < ZSTD_lazy ? srcSize <= 1024 : 0;
        if (repeat == HUF_repeat_valid && lhSize == 3) singleStream = 1;
        cLitSize = singleStream ? HUF_compress1X_repeat(ostart+lhSize, dstCapacity-lhSize, src, srcSize, 255, 11,
                                      workspace, wkspSize, (HUF_CElt*)nextHuf->CTable, &repeat, preferRepeat, bmi2)
                                : HUF_compress4X_repeat(ostart+lhSize, dstCapacity-lhSize, src, srcSize, 255, 11,
                                      workspace, wkspSize, (HUF_CElt*)nextHuf->CTable, &repeat, preferRepeat, bmi2);
        if (repeat != HUF_repeat_none) {
             
            hType = set_repeat;
        }
    }

    if ((cLitSize==0) | (cLitSize >= srcSize - minGain) | ERR_isError(cLitSize)) {
        memcpy(nextHuf, prevHuf, sizeof(*prevHuf));
        return ZSTD_noCompressLiterals(dst, dstCapacity, src, srcSize);
    }
    if (cLitSize==1) {
        memcpy(nextHuf, prevHuf, sizeof(*prevHuf));
        return ZSTD_compressRleLiteralsBlock(dst, dstCapacity, src, srcSize);
    }

    if (hType == set_compressed) {
         
        nextHuf->repeatMode = HUF_repeat_check;
    }

     
    switch(lhSize)
    {
    case 3:  
        {   U32 const lhc = hType + ((!singleStream) << 2) + ((U32)srcSize<<4) + ((U32)cLitSize<<14);
            MEM_writeLE24(ostart, lhc);
            break;
        }
    case 4:  
        {   U32 const lhc = hType + (2 << 2) + ((U32)srcSize<<4) + ((U32)cLitSize<<18);
            MEM_writeLE32(ostart, lhc);
            break;
        }
    case 5:  
        {   U32 const lhc = hType + (3 << 2) + ((U32)srcSize<<4) + ((U32)cLitSize<<22);
            MEM_writeLE32(ostart, lhc);
            ostart[4] = (BYTE)(cLitSize >> 10);
            break;
        }
    default:   
        assert(0);
    }
    return lhSize+cLitSize;
}