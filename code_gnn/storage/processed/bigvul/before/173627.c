u32 h264bsdIsStartOfPicture(storage_t *pStorage)
{

 


 

 if (pStorage->validSliceInAccessUnit == HANTRO_FALSE)
 return(HANTRO_TRUE);
 else
 return(HANTRO_FALSE);

}
