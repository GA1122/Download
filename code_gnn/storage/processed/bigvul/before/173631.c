u32 h264bsdValidParamSets(storage_t *pStorage)
{

 

    u32 i;

 

    ASSERT(pStorage);

 for (i = 0; i < MAX_NUM_PIC_PARAM_SETS; i++)
 {
 if ( pStorage->pps[i] &&
             pStorage->sps[pStorage->pps[i]->seqParameterSetId] &&
 CheckPps(pStorage->pps[i],
                      pStorage->sps[pStorage->pps[i]->seqParameterSetId]) ==
                 HANTRO_OK)
 {
 return(HANTRO_OK);
 }
 }

 return(HANTRO_NOK);

}
