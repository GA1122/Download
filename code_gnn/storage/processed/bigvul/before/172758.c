void h264bsdInitRefPicList(dpbStorage_t *dpb)
{

 

    u32 i;

 

 for (i = 0; i < dpb->numRefFrames; i++)
        dpb->list[i] = &dpb->buffer[i];

}
