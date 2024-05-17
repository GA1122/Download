void h264bsdShutdown(storage_t *pStorage)
{

 

    u32 i;

 

    ASSERT(pStorage);

 for (i = 0; i < MAX_NUM_SEQ_PARAM_SETS; i++)
 {
 if (pStorage->sps[i])
 {
            FREE(pStorage->sps[i]->offsetForRefFrame);
            FREE(pStorage->sps[i]->vuiParameters);
            FREE(pStorage->sps[i]);
 }
 }

 for (i = 0; i < MAX_NUM_PIC_PARAM_SETS; i++)
 {
 if (pStorage->pps[i])
 {
            FREE(pStorage->pps[i]->runLength);
            FREE(pStorage->pps[i]->topLeft);
            FREE(pStorage->pps[i]->bottomRight);
            FREE(pStorage->pps[i]->sliceGroupId);
            FREE(pStorage->pps[i]);
 }
 }

    FREE(pStorage->mbLayer);
    FREE(pStorage->mb);
    FREE(pStorage->sliceGroupMap);

    h264bsdFreeDpb(pStorage->dpb);

}
