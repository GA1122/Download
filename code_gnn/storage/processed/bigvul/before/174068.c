u32 h264bsdPicHeight(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

 if (pStorage->activeSps)
 return(pStorage->activeSps->picHeightInMbs);
 else
 return(0);

}
