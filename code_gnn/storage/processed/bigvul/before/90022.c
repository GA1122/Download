static size_t ZSTD_compressRleLiteralsBlock (void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
    BYTE* const ostart = (BYTE* const)dst;
    U32   const flSize = 1 + (srcSize>31) + (srcSize>4095);

    (void)dstCapacity;   

    switch(flSize)
    {
        case 1:  
            ostart[0] = (BYTE)((U32)set_rle + (srcSize<<3));
            break;
        case 2:  
            MEM_writeLE16(ostart, (U16)((U32)set_rle + (1<<2) + (srcSize<<4)));
            break;
        case 3:  
            MEM_writeLE32(ostart, (U32)((U32)set_rle + (3<<2) + (srcSize<<4)));
            break;
        default:    
            assert(0);
    }

    ostart[flSize] = *(const BYTE*)src;
    return flSize+1;
}
