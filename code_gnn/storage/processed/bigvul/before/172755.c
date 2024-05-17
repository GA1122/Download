void h264bsdFlushDpb(dpbStorage_t *dpb)
{

  
 if (dpb->buffer)
 {
        dpb->flushed = 1;
  
 while (OutputPicture(dpb) == HANTRO_OK)
 ;
 }

}
