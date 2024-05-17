void h264bsdInitStorage(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

    H264SwDecMemset(pStorage, 0, sizeof(storage_t));

    pStorage->activeSpsId = MAX_NUM_SEQ_PARAM_SETS;
    pStorage->activePpsId = MAX_NUM_PIC_PARAM_SETS;

    pStorage->aub->firstCallFlag = HANTRO_TRUE;
}
