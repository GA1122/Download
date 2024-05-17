size_t ZSTD_CCtxParam_setParameter(
        ZSTD_CCtx_params* CCtxParams, ZSTD_cParameter param, unsigned value)
{
    DEBUGLOG(4, "ZSTD_CCtxParam_setParameter (%u, %u)", (U32)param, value);
    switch(param)
    {
    case ZSTD_p_format :
        if (value > (unsigned)ZSTD_f_zstd1_magicless)
            return ERROR(parameter_unsupported);
        CCtxParams->format = (ZSTD_format_e)value;
        return (size_t)CCtxParams->format;

    case ZSTD_p_compressionLevel : {
        int cLevel = (int)value;   
        if (cLevel > ZSTD_maxCLevel()) cLevel = ZSTD_maxCLevel();
        if (cLevel) {   
            CCtxParams->compressionLevel = cLevel;
        }
        if (CCtxParams->compressionLevel >= 0) return CCtxParams->compressionLevel;
        return 0;   
    }

    case ZSTD_p_windowLog :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_WINDOWLOG_MIN, ZSTD_WINDOWLOG_MAX);
        CCtxParams->cParams.windowLog = value;
        return CCtxParams->cParams.windowLog;

    case ZSTD_p_hashLog :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_HASHLOG_MIN, ZSTD_HASHLOG_MAX);
        CCtxParams->cParams.hashLog = value;
        return CCtxParams->cParams.hashLog;

    case ZSTD_p_chainLog :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_CHAINLOG_MIN, ZSTD_CHAINLOG_MAX);
        CCtxParams->cParams.chainLog = value;
        return CCtxParams->cParams.chainLog;

    case ZSTD_p_searchLog :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_SEARCHLOG_MIN, ZSTD_SEARCHLOG_MAX);
        CCtxParams->cParams.searchLog = value;
        return value;

    case ZSTD_p_minMatch :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_SEARCHLENGTH_MIN, ZSTD_SEARCHLENGTH_MAX);
        CCtxParams->cParams.searchLength = value;
        return CCtxParams->cParams.searchLength;

    case ZSTD_p_targetLength :
         
        CCtxParams->cParams.targetLength = value;
        return CCtxParams->cParams.targetLength;

    case ZSTD_p_compressionStrategy :
        if (value>0)    
            CLAMPCHECK(value, (unsigned)ZSTD_fast, (unsigned)ZSTD_btultra);
        CCtxParams->cParams.strategy = (ZSTD_strategy)value;
        return (size_t)CCtxParams->cParams.strategy;

    case ZSTD_p_contentSizeFlag :
         
        DEBUGLOG(4, "set content size flag = %u", (value>0));
        CCtxParams->fParams.contentSizeFlag = value > 0;
        return CCtxParams->fParams.contentSizeFlag;

    case ZSTD_p_checksumFlag :
         
        CCtxParams->fParams.checksumFlag = value > 0;
        return CCtxParams->fParams.checksumFlag;

    case ZSTD_p_dictIDFlag :  
        DEBUGLOG(4, "set dictIDFlag = %u", (value>0));
        CCtxParams->fParams.noDictIDFlag = !value;
        return !CCtxParams->fParams.noDictIDFlag;

    case ZSTD_p_forceMaxWindow :
        CCtxParams->forceWindow = (value > 0);
        return CCtxParams->forceWindow;

    case ZSTD_p_forceAttachDict :
        CCtxParams->attachDictPref = value ?
                                    (value > 0 ? ZSTD_dictForceAttach : ZSTD_dictForceCopy) :
                                     ZSTD_dictDefaultAttach;
        return CCtxParams->attachDictPref;

    case ZSTD_p_nbWorkers :
#ifndef ZSTD_MULTITHREAD
        if (value>0) return ERROR(parameter_unsupported);
        return 0;
#else
        return ZSTDMT_CCtxParam_setNbWorkers(CCtxParams, value);
#endif

    case ZSTD_p_jobSize :
#ifndef ZSTD_MULTITHREAD
        return ERROR(parameter_unsupported);
#else
        return ZSTDMT_CCtxParam_setMTCtxParameter(CCtxParams, ZSTDMT_p_jobSize, value);
#endif

    case ZSTD_p_overlapSizeLog :
#ifndef ZSTD_MULTITHREAD
        return ERROR(parameter_unsupported);
#else
        return ZSTDMT_CCtxParam_setMTCtxParameter(CCtxParams, ZSTDMT_p_overlapSectionLog, value);
#endif

    case ZSTD_p_enableLongDistanceMatching :
        CCtxParams->ldmParams.enableLdm = (value>0);
        return CCtxParams->ldmParams.enableLdm;

    case ZSTD_p_ldmHashLog :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_HASHLOG_MIN, ZSTD_HASHLOG_MAX);
        CCtxParams->ldmParams.hashLog = value;
        return CCtxParams->ldmParams.hashLog;

    case ZSTD_p_ldmMinMatch :
        if (value>0)    
            CLAMPCHECK(value, ZSTD_LDM_MINMATCH_MIN, ZSTD_LDM_MINMATCH_MAX);
        CCtxParams->ldmParams.minMatchLength = value;
        return CCtxParams->ldmParams.minMatchLength;

    case ZSTD_p_ldmBucketSizeLog :
        if (value > ZSTD_LDM_BUCKETSIZELOG_MAX)
            return ERROR(parameter_outOfBound);
        CCtxParams->ldmParams.bucketSizeLog = value;
        return CCtxParams->ldmParams.bucketSizeLog;

    case ZSTD_p_ldmHashEveryLog :
        if (value > ZSTD_WINDOWLOG_MAX - ZSTD_HASHLOG_MIN)
            return ERROR(parameter_outOfBound);
        CCtxParams->ldmParams.hashEveryLog = value;
        return CCtxParams->ldmParams.hashEveryLog;

    default: return ERROR(parameter_unsupported);
    }
}