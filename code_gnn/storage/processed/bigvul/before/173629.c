u32 h264bsdStorePicParamSet(storage_t *pStorage, picParamSet_t *pPicParamSet)
{

 

    u32 id;

 

    ASSERT(pStorage);
    ASSERT(pPicParamSet);
    ASSERT(pPicParamSet->picParameterSetId < MAX_NUM_PIC_PARAM_SETS);
    ASSERT(pPicParamSet->seqParameterSetId < MAX_NUM_SEQ_PARAM_SETS);

    id = pPicParamSet->picParameterSetId;

  
 if (pStorage->pps[id] == NULL)
 {
        ALLOCATE(pStorage->pps[id], 1, picParamSet_t);
 if (pStorage->pps[id] == NULL)
 return(MEMORY_ALLOCATION_ERROR);
 }
  
 else if (id == pStorage->activePpsId)
 {
  
 if (pPicParamSet->seqParameterSetId != pStorage->activeSpsId)
 {
            pStorage->activePpsId = MAX_NUM_PIC_PARAM_SETS + 1;
 }
  
        FREE(pStorage->pps[id]->runLength);
        FREE(pStorage->pps[id]->topLeft);
        FREE(pStorage->pps[id]->bottomRight);
        FREE(pStorage->pps[id]->sliceGroupId);
 }
  
 else
 {
        FREE(pStorage->pps[id]->runLength);
        FREE(pStorage->pps[id]->topLeft);
        FREE(pStorage->pps[id]->bottomRight);
        FREE(pStorage->pps[id]->sliceGroupId);
 }

 *pStorage->pps[id] = *pPicParamSet;

 return(HANTRO_OK);

}
