static size_t ZSTD_resetCCtx_internal(ZSTD_CCtx* zc,
                                      ZSTD_CCtx_params params,
                                      U64 pledgedSrcSize,
                                      ZSTD_compResetPolicy_e const crp,
                                      ZSTD_buffered_policy_e const zbuff)
{
    DEBUGLOG(4, "ZSTD_resetCCtx_internal: pledgedSrcSize=%u, wlog=%u",
                (U32)pledgedSrcSize, params.cParams.windowLog);
    assert(!ZSTD_isError(ZSTD_checkCParams(params.cParams)));

    if (crp == ZSTDcrp_continue) {
        if (ZSTD_equivalentParams(zc->appliedParams, params,
                                  zc->inBuffSize,
                                  zc->seqStore.maxNbSeq, zc->seqStore.maxNbLit,
                                  zbuff, pledgedSrcSize)) {
            DEBUGLOG(4, "ZSTD_equivalentParams()==1 -> continue mode (wLog1=%u, blockSize1=%zu)",
                        zc->appliedParams.cParams.windowLog, zc->blockSize);
            zc->workSpaceOversizedDuration += (zc->workSpaceOversizedDuration > 0);    
            if (zc->workSpaceOversizedDuration <= ZSTD_WORKSPACETOOLARGE_MAXDURATION)
                return ZSTD_continueCCtx(zc, params, pledgedSrcSize);
    }   }
    DEBUGLOG(4, "ZSTD_equivalentParams()==0 -> reset CCtx");

    if (params.ldmParams.enableLdm) {
         
        ZSTD_ldm_adjustParameters(&params.ldmParams, &params.cParams);
        assert(params.ldmParams.hashLog >= params.ldmParams.bucketSizeLog);
        assert(params.ldmParams.hashEveryLog < 32);
        zc->ldmState.hashPower = ZSTD_ldm_getHashPower(params.ldmParams.minMatchLength);
    }

    {   size_t const windowSize = MAX(1, (size_t)MIN(((U64)1 << params.cParams.windowLog), pledgedSrcSize));
        size_t const blockSize = MIN(ZSTD_BLOCKSIZE_MAX, windowSize);
        U32    const divider = (params.cParams.searchLength==3) ? 3 : 4;
        size_t const maxNbSeq = blockSize / divider;
        size_t const tokenSpace = WILDCOPY_OVERLENGTH + blockSize + 11*maxNbSeq;
        size_t const buffOutSize = (zbuff==ZSTDb_buffered) ? ZSTD_compressBound(blockSize)+1 : 0;
        size_t const buffInSize = (zbuff==ZSTDb_buffered) ? windowSize + blockSize : 0;
        size_t const matchStateSize = ZSTD_sizeof_matchState(&params.cParams,   1);
        size_t const maxNbLdmSeq = ZSTD_ldm_getMaxNbSeq(params.ldmParams, blockSize);
        void* ptr;    

         
        {   size_t const entropySpace = HUF_WORKSPACE_SIZE;
            size_t const blockStateSpace = 2 * sizeof(ZSTD_compressedBlockState_t);
            size_t const bufferSpace = buffInSize + buffOutSize;
            size_t const ldmSpace = ZSTD_ldm_getTableSize(params.ldmParams);
            size_t const ldmSeqSpace = maxNbLdmSeq * sizeof(rawSeq);

            size_t const neededSpace = entropySpace + blockStateSpace + ldmSpace +
                                       ldmSeqSpace + matchStateSize + tokenSpace +
                                       bufferSpace;

            int const workSpaceTooSmall = zc->workSpaceSize < neededSpace;
            int const workSpaceTooLarge = zc->workSpaceSize > ZSTD_WORKSPACETOOLARGE_FACTOR * neededSpace;
            int const workSpaceWasteful = workSpaceTooLarge && (zc->workSpaceOversizedDuration > ZSTD_WORKSPACETOOLARGE_MAXDURATION);
            zc->workSpaceOversizedDuration = workSpaceTooLarge ? zc->workSpaceOversizedDuration+1 : 0;

            DEBUGLOG(4, "Need %zuKB workspace, including %zuKB for match state, and %zuKB for buffers",
                        neededSpace>>10, matchStateSize>>10, bufferSpace>>10);
            DEBUGLOG(4, "windowSize: %zu - blockSize: %zu", windowSize, blockSize);

            if (workSpaceTooSmall || workSpaceWasteful) {
                DEBUGLOG(4, "Need to resize workSpaceSize from %zuKB to %zuKB",
                            zc->workSpaceSize >> 10,
                            neededSpace >> 10);
                 
                if (zc->staticSize) return ERROR(memory_allocation);

                zc->workSpaceSize = 0;
                ZSTD_free(zc->workSpace, zc->customMem);
                zc->workSpace = ZSTD_malloc(neededSpace, zc->customMem);
                if (zc->workSpace == NULL) return ERROR(memory_allocation);
                zc->workSpaceSize = neededSpace;
                zc->workSpaceOversizedDuration = 0;

                 
                assert(((size_t)zc->workSpace & 3) == 0);    
                assert(zc->workSpaceSize >= 2 * sizeof(ZSTD_compressedBlockState_t));
                zc->blockState.prevCBlock = (ZSTD_compressedBlockState_t*)zc->workSpace;
                zc->blockState.nextCBlock = zc->blockState.prevCBlock + 1;
                ptr = zc->blockState.nextCBlock + 1;
                zc->entropyWorkspace = (U32*)ptr;
        }   }

         
        zc->appliedParams = params;
        zc->blockState.matchState.cParams = params.cParams;
        zc->pledgedSrcSizePlusOne = pledgedSrcSize+1;
        zc->consumedSrcSize = 0;
        zc->producedCSize = 0;
        if (pledgedSrcSize == ZSTD_CONTENTSIZE_UNKNOWN)
            zc->appliedParams.fParams.contentSizeFlag = 0;
        DEBUGLOG(4, "pledged content size : %u ; flag : %u",
            (U32)pledgedSrcSize, zc->appliedParams.fParams.contentSizeFlag);
        zc->blockSize = blockSize;

        XXH64_reset(&zc->xxhState, 0);
        zc->stage = ZSTDcs_init;
        zc->dictID = 0;

        ZSTD_reset_compressedBlockState(zc->blockState.prevCBlock);

        ptr = zc->entropyWorkspace + HUF_WORKSPACE_SIZE_U32;

         
         
        if (params.ldmParams.enableLdm) {
            size_t const ldmHSize = ((size_t)1) << params.ldmParams.hashLog;
            memset(ptr, 0, ldmHSize * sizeof(ldmEntry_t));
            assert(((size_t)ptr & 3) == 0);  
            zc->ldmState.hashTable = (ldmEntry_t*)ptr;
            ptr = zc->ldmState.hashTable + ldmHSize;
            zc->ldmSequences = (rawSeq*)ptr;
            ptr = zc->ldmSequences + maxNbLdmSeq;
            zc->maxNbLdmSequences = maxNbLdmSeq;

            memset(&zc->ldmState.window, 0, sizeof(zc->ldmState.window));
        }
        assert(((size_t)ptr & 3) == 0);  

        ptr = ZSTD_reset_matchState(&zc->blockState.matchState, ptr, &params.cParams, crp,   1);

         
        zc->seqStore.maxNbSeq = maxNbSeq;
        zc->seqStore.sequencesStart = (seqDef*)ptr;
        ptr = zc->seqStore.sequencesStart + maxNbSeq;
        zc->seqStore.llCode = (BYTE*) ptr;
        zc->seqStore.mlCode = zc->seqStore.llCode + maxNbSeq;
        zc->seqStore.ofCode = zc->seqStore.mlCode + maxNbSeq;
        zc->seqStore.litStart = zc->seqStore.ofCode + maxNbSeq;
         
        zc->seqStore.maxNbLit = blockSize;
        ptr = zc->seqStore.litStart + blockSize + WILDCOPY_OVERLENGTH;

         
        if (params.ldmParams.enableLdm) {
            size_t const ldmBucketSize =
                  ((size_t)1) << (params.ldmParams.hashLog -
                                  params.ldmParams.bucketSizeLog);
            memset(ptr, 0, ldmBucketSize);
            zc->ldmState.bucketOffsets = (BYTE*)ptr;
            ptr = zc->ldmState.bucketOffsets + ldmBucketSize;
            ZSTD_window_clear(&zc->ldmState.window);
        }
        ZSTD_referenceExternalSequences(zc, NULL, 0);

         
        zc->inBuffSize = buffInSize;
        zc->inBuff = (char*)ptr;
        zc->outBuffSize = buffOutSize;
        zc->outBuff = zc->inBuff + buffInSize;

        return 0;
    }
}