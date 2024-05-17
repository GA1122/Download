u32 h264bsdPicWidth(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

 if (pStorage->activeSps)
 return(pStorage->activeSps->picWidthInMbs);
 else
 return(0);

}
