size_t ZSTD_writeLastEmptyBlock(void* dst, size_t dstCapacity)
{
    if (dstCapacity < ZSTD_blockHeaderSize) return ERROR(dstSize_tooSmall);
    {   U32 const cBlockHeader24 = 1   + (((U32)bt_raw)<<1);   
        MEM_writeLE24(dst, cBlockHeader24);
        return ZSTD_blockHeaderSize;
    }
}