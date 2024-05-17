void h264bsdFlushBuffer(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

    h264bsdFlushDpb(pStorage->dpb);

}
