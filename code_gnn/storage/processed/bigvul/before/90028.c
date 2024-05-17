static size_t ZSTD_compress_frameChunk (ZSTD_CCtx* cctx,
                                     void* dst, size_t dstCapacity,
                               const void* src, size_t srcSize,
                                     U32 lastFrameChunk)
{
    size_t blockSize = cctx->blockSize;
    size_t remaining = srcSize;
    const BYTE* ip = (const BYTE*)src;
    BYTE* const ostart = (BYTE*)dst;
    BYTE* op = ostart;
    U32 const maxDist = (U32)1 << cctx->appliedParams.cParams.windowLog;
    assert(cctx->appliedParams.cParams.windowLog <= 31);

    DEBUGLOG(5, "ZSTD_compress_frameChunk (blockSize=%u)", (U32)blockSize);
    if (cctx->appliedParams.fParams.checksumFlag && srcSize)
        XXH64_update(&cctx->xxhState, src, srcSize);

    while (remaining) {
        ZSTD_matchState_t* const ms = &cctx->blockState.matchState;
        U32 const lastBlock = lastFrameChunk & (blockSize >= remaining);

        if (dstCapacity < ZSTD_blockHeaderSize + MIN_CBLOCK_SIZE)
            return ERROR(dstSize_tooSmall);    
        if (remaining < blockSize) blockSize = remaining;

        if (ZSTD_window_needOverflowCorrection(ms->window, ip + blockSize)) {
            U32 const cycleLog = ZSTD_cycleLog(cctx->appliedParams.cParams.chainLog, cctx->appliedParams.cParams.strategy);
            U32 const correction = ZSTD_window_correctOverflow(&ms->window, cycleLog, maxDist, ip);
            ZSTD_STATIC_ASSERT(ZSTD_CHAINLOG_MAX <= 30);
            ZSTD_STATIC_ASSERT(ZSTD_WINDOWLOG_MAX_32 <= 30);
            ZSTD_STATIC_ASSERT(ZSTD_WINDOWLOG_MAX <= 31);
            ZSTD_reduceIndex(cctx, correction);
            if (ms->nextToUpdate < correction) ms->nextToUpdate = 0;
            else ms->nextToUpdate -= correction;
            ms->loadedDictEnd = 0;
            ms->dictMatchState = NULL;
        }
        ZSTD_window_enforceMaxDist(&ms->window, ip + blockSize, maxDist, &ms->loadedDictEnd, &ms->dictMatchState);
        if (ms->nextToUpdate < ms->window.lowLimit) ms->nextToUpdate = ms->window.lowLimit;

        {   size_t cSize = ZSTD_compressBlock_internal(cctx,
                                op+ZSTD_blockHeaderSize, dstCapacity-ZSTD_blockHeaderSize,
                                ip, blockSize);
            if (ZSTD_isError(cSize)) return cSize;

            if (cSize == 0) {   
                cSize = ZSTD_noCompressBlock(op, dstCapacity, ip, blockSize, lastBlock);
                if (ZSTD_isError(cSize)) return cSize;
            } else {
                U32 const cBlockHeader24 = lastBlock + (((U32)bt_compressed)<<1) + (U32)(cSize << 3);
                MEM_writeLE24(op, cBlockHeader24);
                cSize += ZSTD_blockHeaderSize;
            }

            ip += blockSize;
            assert(remaining >= blockSize);
            remaining -= blockSize;
            op += cSize;
            assert(dstCapacity >= cSize);
            dstCapacity -= cSize;
            DEBUGLOG(5, "ZSTD_compress_frameChunk: adding a block of size %u",
                        (U32)cSize);
    }   }

    if (lastFrameChunk && (op>ostart)) cctx->stage = ZSTDcs_ending;
    return op-ostart;
}