static size_t ZSTD_noCompressLiterals (void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
    BYTE* const ostart = (BYTE* const)dst;
    U32   const flSize = 1 + (srcSize>31) + (srcSize>4095);

    if (srcSize + flSize > dstCapacity) return ERROR(dstSize_tooSmall);

    switch(flSize)
    {
        case 1:  
            ostart[0] = (BYTE)((U32)set_basic + (srcSize<<3));
            break;
        case 2:  
            MEM_writeLE16(ostart, (U16)((U32)set_basic + (1<<2) + (srcSize<<4)));
            break;
        case 3:  
            MEM_writeLE32(ostart, (U32)((U32)set_basic + (3<<2) + (srcSize<<4)));
            break;
        default:    
            assert(0);
    }

    memcpy(ostart + flSize, src, srcSize);
    return srcSize + flSize;
}