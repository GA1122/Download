u8* h264bsdAllocateDpbImage(dpbStorage_t *dpb)
{

 

 

    ASSERT( !dpb->buffer[dpb->dpbSize].toBeDisplayed &&
 !IS_REFERENCE(dpb->buffer[dpb->dpbSize]) );
    ASSERT(dpb->fullness <=  dpb->dpbSize);

    dpb->currentOut = dpb->buffer + dpb->dpbSize;

 return(dpb->currentOut->data);

}
