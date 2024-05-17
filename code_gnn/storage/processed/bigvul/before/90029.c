size_t ZSTD_compress_generic (ZSTD_CCtx* cctx,
                              ZSTD_outBuffer* output,
                              ZSTD_inBuffer* input,
                              ZSTD_EndDirective endOp)
{
    DEBUGLOG(5, "ZSTD_compress_generic, endOp=%u ", (U32)endOp);
     
    if (output->pos > output->size) return ERROR(GENERIC);
    if (input->pos  > input->size)  return ERROR(GENERIC);
    assert(cctx!=NULL);

     
    if (cctx->streamStage == zcss_init) {
        ZSTD_CCtx_params params = cctx->requestedParams;
        ZSTD_prefixDict const prefixDict = cctx->prefixDict;
        memset(&cctx->prefixDict, 0, sizeof(cctx->prefixDict));   
        assert(prefixDict.dict==NULL || cctx->cdict==NULL);    
        DEBUGLOG(4, "ZSTD_compress_generic : transparent init stage");
        if (endOp == ZSTD_e_end) cctx->pledgedSrcSizePlusOne = input->size + 1;   
        params.cParams = ZSTD_getCParamsFromCCtxParams(
                &cctx->requestedParams, cctx->pledgedSrcSizePlusOne-1, 0  );


#ifdef ZSTD_MULTITHREAD
        if ((cctx->pledgedSrcSizePlusOne-1) <= ZSTDMT_JOBSIZE_MIN) {
            params.nbWorkers = 0;  
        }
        if (params.nbWorkers > 0) {
             
            if (cctx->mtctx == NULL) {
                DEBUGLOG(4, "ZSTD_compress_generic: creating new mtctx for nbWorkers=%u",
                            params.nbWorkers);
                cctx->mtctx = ZSTDMT_createCCtx_advanced(params.nbWorkers, cctx->customMem);
                if (cctx->mtctx == NULL) return ERROR(memory_allocation);
            }
             
            DEBUGLOG(4, "call ZSTDMT_initCStream_internal as nbWorkers=%u", params.nbWorkers);
            CHECK_F( ZSTDMT_initCStream_internal(
                        cctx->mtctx,
                        prefixDict.dict, prefixDict.dictSize, ZSTD_dct_rawContent,
                        cctx->cdict, params, cctx->pledgedSrcSizePlusOne-1) );
            cctx->streamStage = zcss_load;
            cctx->appliedParams.nbWorkers = params.nbWorkers;
        } else
#endif
        {   CHECK_F( ZSTD_resetCStream_internal(cctx,
                            prefixDict.dict, prefixDict.dictSize, prefixDict.dictContentType,
                            cctx->cdict,
                            params, cctx->pledgedSrcSizePlusOne-1) );
            assert(cctx->streamStage == zcss_load);
            assert(cctx->appliedParams.nbWorkers == 0);
    }   }

     
#ifdef ZSTD_MULTITHREAD
    if (cctx->appliedParams.nbWorkers > 0) {
        if (cctx->cParamsChanged) {
            ZSTDMT_updateCParams_whileCompressing(cctx->mtctx, &cctx->requestedParams);
            cctx->cParamsChanged = 0;
        }
        {   size_t const flushMin = ZSTDMT_compressStream_generic(cctx->mtctx, output, input, endOp);
            if ( ZSTD_isError(flushMin)
              || (endOp == ZSTD_e_end && flushMin == 0) ) {  
                ZSTD_CCtx_reset(cctx);
            }
            DEBUGLOG(5, "completed ZSTD_compress_generic delegating to ZSTDMT_compressStream_generic");
            return flushMin;
    }   }
#endif
    CHECK_F( ZSTD_compressStream_generic(cctx, output, input, endOp) );
    DEBUGLOG(5, "completed ZSTD_compress_generic");
    return cctx->outBuffContentSize - cctx->outBuffFlushedSize;  
}