u32 h264bsdStoreSeqParamSet(storage_t *pStorage, seqParamSet_t *pSeqParamSet)
{

 

    u32 id;

 

    ASSERT(pStorage);
    ASSERT(pSeqParamSet);
    ASSERT(pSeqParamSet->seqParameterSetId < MAX_NUM_SEQ_PARAM_SETS);

    id = pSeqParamSet->seqParameterSetId;

  
 if (pStorage->sps[id] == NULL)
 {
        ALLOCATE(pStorage->sps[id], 1, seqParamSet_t);
 if (pStorage->sps[id] == NULL)
 return(MEMORY_ALLOCATION_ERROR);
 }
  
 else if (id == pStorage->activeSpsId)
 {
  
 if (h264bsdCompareSeqParamSets(pSeqParamSet, pStorage->activeSps) != 0)
 {
            FREE(pStorage->sps[id]->offsetForRefFrame);
            FREE(pStorage->sps[id]->vuiParameters);
            pStorage->activeSpsId = MAX_NUM_SEQ_PARAM_SETS + 1;
            pStorage->activePpsId = MAX_NUM_PIC_PARAM_SETS + 1;
            pStorage->activeSps = NULL;
            pStorage->activePps = NULL;
 }
 else
 {
            FREE(pSeqParamSet->offsetForRefFrame);
            FREE(pSeqParamSet->vuiParameters);
 return(HANTRO_OK);
 }
 }
  
 else
 {
        FREE(pStorage->sps[id]->offsetForRefFrame);
        FREE(pStorage->sps[id]->vuiParameters);
 }

 *pStorage->sps[id] = *pSeqParamSet;

 return(HANTRO_OK);

}
