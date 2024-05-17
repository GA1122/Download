static size_t ZSTD_resetCStream_internal(ZSTD_CStream* cctx,
                    const void* const dict, size_t const dictSize, ZSTD_dictContentType_e const dictContentType,
                    const ZSTD_CDict* const cdict,
                    ZSTD_CCtx_params params, unsigned long long const pledgedSrcSize)
{
    DEBUGLOG(4, "ZSTD_resetCStream_internal");
     
    params.cParams = ZSTD_getCParamsFromCCtxParams(&params, pledgedSrcSize, dictSize);
     
    assert(!ZSTD_isError(ZSTD_checkCParams(params.cParams)));
    assert(!((dict) && (cdict)));   

    CHECK_F( ZSTD_compressBegin_internal(cctx,
                                         dict, dictSize, dictContentType, ZSTD_dtlm_fast,
                                         cdict,
                                         params, pledgedSrcSize,
                                         ZSTDb_buffered) );

    cctx->inToCompress = 0;
    cctx->inBuffPos = 0;
    cctx->inBuffTarget = cctx->blockSize
                      + (cctx->blockSize == pledgedSrcSize);    
    cctx->outBuffContentSize = cctx->outBuffFlushedSize = 0;
    cctx->streamStage = zcss_load;
    cctx->frameEnded = 0;
    return 0;    
}
