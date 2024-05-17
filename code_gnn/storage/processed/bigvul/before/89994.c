size_t ZSTD_CCtx_setParameter(ZSTD_CCtx* cctx, ZSTD_cParameter param, unsigned value)
{
    DEBUGLOG(4, "ZSTD_CCtx_setParameter (%u, %u)", (U32)param, value);
    if (cctx->streamStage != zcss_init) {
        if (ZSTD_isUpdateAuthorized(param)) {
            cctx->cParamsChanged = 1;
        } else {
            return ERROR(stage_wrong);
    }   }

    switch(param)
    {
    case ZSTD_p_format :
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_compressionLevel:
        if (cctx->cdict) return ERROR(stage_wrong);
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_windowLog:
    case ZSTD_p_hashLog:
    case ZSTD_p_chainLog:
    case ZSTD_p_searchLog:
    case ZSTD_p_minMatch:
    case ZSTD_p_targetLength:
    case ZSTD_p_compressionStrategy:
        if (cctx->cdict) return ERROR(stage_wrong);
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_contentSizeFlag:
    case ZSTD_p_checksumFlag:
    case ZSTD_p_dictIDFlag:
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_forceMaxWindow :   
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_forceAttachDict:
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_nbWorkers:
        if ((value>0) && cctx->staticSize) {
            return ERROR(parameter_unsupported);   
        }
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_jobSize:
    case ZSTD_p_overlapSizeLog:
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    case ZSTD_p_enableLongDistanceMatching:
    case ZSTD_p_ldmHashLog:
    case ZSTD_p_ldmMinMatch:
    case ZSTD_p_ldmBucketSizeLog:
    case ZSTD_p_ldmHashEveryLog:
        if (cctx->cdict) return ERROR(stage_wrong);
        return ZSTD_CCtxParam_setParameter(&cctx->requestedParams, param, value);

    default: return ERROR(parameter_unsupported);
    }
}