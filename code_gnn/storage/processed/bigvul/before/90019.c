static size_t ZSTD_compressContinue_internal (ZSTD_CCtx* cctx,
                              void* dst, size_t dstCapacity,
                        const void* src, size_t srcSize,
                               U32 frame, U32 lastFrameChunk)
{
    ZSTD_matchState_t* const ms = &cctx->blockState.matchState;
    size_t fhSize = 0;

    DEBUGLOG(5, "ZSTD_compressContinue_internal, stage: %u, srcSize: %u",
                cctx->stage, (U32)srcSize);
    if (cctx->stage==ZSTDcs_created) return ERROR(stage_wrong);    

    if (frame && (cctx->stage==ZSTDcs_init)) {
        fhSize = ZSTD_writeFrameHeader(dst, dstCapacity, cctx->appliedParams,
                                       cctx->pledgedSrcSizePlusOne-1, cctx->dictID);
        if (ZSTD_isError(fhSize)) return fhSize;
        dstCapacity -= fhSize;
        dst = (char*)dst + fhSize;
        cctx->stage = ZSTDcs_ongoing;
    }

    if (!srcSize) return fhSize;   

    if (!ZSTD_window_update(&ms->window, src, srcSize)) {
        ms->nextToUpdate = ms->window.dictLimit;
    }
    if (cctx->appliedParams.ldmParams.enableLdm) {
        ZSTD_window_update(&cctx->ldmState.window, src, srcSize);
    }

    if (!frame) {
         
        if (ZSTD_window_needOverflowCorrection(ms->window, (const char*)src + srcSize)) {
            U32 const cycleLog = ZSTD_cycleLog(cctx->appliedParams.cParams.chainLog, cctx->appliedParams.cParams.strategy);
            U32 const correction = ZSTD_window_correctOverflow(&ms->window, cycleLog, 1 << cctx->appliedParams.cParams.windowLog, src);
            ZSTD_STATIC_ASSERT(ZSTD_CHAINLOG_MAX <= 30);
            ZSTD_STATIC_ASSERT(ZSTD_WINDOWLOG_MAX_32 <= 30);
            ZSTD_STATIC_ASSERT(ZSTD_WINDOWLOG_MAX <= 31);
            ZSTD_reduceIndex(cctx, correction);
            if (ms->nextToUpdate < correction) ms->nextToUpdate = 0;
            else ms->nextToUpdate -= correction;
            ms->loadedDictEnd = 0;
            ms->dictMatchState = NULL;
        }
    }

    DEBUGLOG(5, "ZSTD_compressContinue_internal (blockSize=%u)", (U32)cctx->blockSize);
    {   size_t const cSize = frame ?
                             ZSTD_compress_frameChunk (cctx, dst, dstCapacity, src, srcSize, lastFrameChunk) :
                             ZSTD_compressBlock_internal (cctx, dst, dstCapacity, src, srcSize);
        if (ZSTD_isError(cSize)) return cSize;
        cctx->consumedSrcSize += srcSize;
        cctx->producedCSize += (cSize + fhSize);
        assert(!(cctx->appliedParams.fParams.contentSizeFlag && cctx->pledgedSrcSizePlusOne == 0));
        if (cctx->pledgedSrcSizePlusOne != 0) {   
            ZSTD_STATIC_ASSERT(ZSTD_CONTENTSIZE_UNKNOWN == (unsigned long long)-1);
            if (cctx->consumedSrcSize+1 > cctx->pledgedSrcSizePlusOne) {
                DEBUGLOG(4, "error : pledgedSrcSize = %u, while realSrcSize >= %u",
                    (U32)cctx->pledgedSrcSizePlusOne-1, (U32)cctx->consumedSrcSize);
                return ERROR(srcSize_wrong);
            }
        }
        return cSize + fhSize;
    }
}