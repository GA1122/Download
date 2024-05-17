void h264bsdFreeDpb(dpbStorage_t *dpb)
{

 

    u32 i;

 

    ASSERT(dpb);

 if (dpb->buffer)
 {
 for (i = 0; i < dpb->dpbSize+1; i++)
 {
            FREE(dpb->buffer[i].pAllocatedData);
 }
 }
    FREE(dpb->buffer);
    FREE(dpb->list);
    FREE(dpb->outBuf);

}
