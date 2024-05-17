static size_t ZSTD_compressBlock_internal(ZSTD_CCtx* zc,
                                        void* dst, size_t dstCapacity,
                                        const void* src, size_t srcSize)
{
    ZSTD_matchState_t* const ms = &zc->blockState.matchState;
    size_t cSize;
    DEBUGLOG(5, "ZSTD_compressBlock_internal (dstCapacity=%zu, dictLimit=%u, nextToUpdate=%u)",
                dstCapacity, ms->window.dictLimit, ms->nextToUpdate);
    assert(srcSize <= ZSTD_BLOCKSIZE_MAX);

     
    ZSTD_assertEqualCParams(zc->appliedParams.cParams, ms->cParams);

    if (srcSize < MIN_CBLOCK_SIZE+ZSTD_blockHeaderSize+1) {
        ZSTD_ldm_skipSequences(&zc->externSeqStore, srcSize, zc->appliedParams.cParams.searchLength);
        cSize = 0;
        goto out;   
    }
    ZSTD_resetSeqStore(&(zc->seqStore));
    ms->opt.symbolCosts = &zc->blockState.prevCBlock->entropy;    

     
    assert(ms->dictMatchState == NULL || ms->loadedDictEnd == ms->window.dictLimit);

     
    {   const BYTE* const base = ms->window.base;
        const BYTE* const istart = (const BYTE*)src;
        const U32 current = (U32)(istart-base);
        if (sizeof(ptrdiff_t)==8) assert(istart - base < (ptrdiff_t)(U32)(-1));    
        if (current > ms->nextToUpdate + 384)
            ms->nextToUpdate = current - MIN(192, (U32)(current - ms->nextToUpdate - 384));
    }

     
    {   ZSTD_dictMode_e const dictMode = ZSTD_matchState_dictMode(ms);
        size_t lastLLSize;
        {   int i;
            for (i = 0; i < ZSTD_REP_NUM; ++i)
                zc->blockState.nextCBlock->rep[i] = zc->blockState.prevCBlock->rep[i];
        }
        if (zc->externSeqStore.pos < zc->externSeqStore.size) {
            assert(!zc->appliedParams.ldmParams.enableLdm);
             
            lastLLSize =
                ZSTD_ldm_blockCompress(&zc->externSeqStore,
                                       ms, &zc->seqStore,
                                       zc->blockState.nextCBlock->rep,
                                       src, srcSize);
            assert(zc->externSeqStore.pos <= zc->externSeqStore.size);
        } else if (zc->appliedParams.ldmParams.enableLdm) {
            rawSeqStore_t ldmSeqStore = {NULL, 0, 0, 0};

            ldmSeqStore.seq = zc->ldmSequences;
            ldmSeqStore.capacity = zc->maxNbLdmSequences;
             
            CHECK_F(ZSTD_ldm_generateSequences(&zc->ldmState, &ldmSeqStore,
                                               &zc->appliedParams.ldmParams,
                                               src, srcSize));
             
            lastLLSize =
                ZSTD_ldm_blockCompress(&ldmSeqStore,
                                       ms, &zc->seqStore,
                                       zc->blockState.nextCBlock->rep,
                                       src, srcSize);
            assert(ldmSeqStore.pos == ldmSeqStore.size);
        } else {    
            ZSTD_blockCompressor const blockCompressor = ZSTD_selectBlockCompressor(zc->appliedParams.cParams.strategy, dictMode);
            lastLLSize = blockCompressor(ms, &zc->seqStore, zc->blockState.nextCBlock->rep, src, srcSize);
        }
        {   const BYTE* const lastLiterals = (const BYTE*)src + srcSize - lastLLSize;
            ZSTD_storeLastLiterals(&zc->seqStore, lastLiterals, lastLLSize);
    }   }

     
    cSize = ZSTD_compressSequences(&zc->seqStore,
            &zc->blockState.prevCBlock->entropy, &zc->blockState.nextCBlock->entropy,
            &zc->appliedParams,
            dst, dstCapacity,
            srcSize,
            zc->entropyWorkspace, HUF_WORKSPACE_SIZE  ,
            zc->bmi2);

out:
    if (!ZSTD_isError(cSize) && cSize != 0) {
         
        ZSTD_compressedBlockState_t* const tmp = zc->blockState.prevCBlock;
        zc->blockState.prevCBlock = zc->blockState.nextCBlock;
        zc->blockState.nextCBlock = tmp;
    }
     
    if (zc->blockState.prevCBlock->entropy.fse.offcode_repeatMode == FSE_repeat_valid)
        zc->blockState.prevCBlock->entropy.fse.offcode_repeatMode = FSE_repeat_check;

    return cSize;
}
