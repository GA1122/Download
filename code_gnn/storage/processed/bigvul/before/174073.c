u32 h264bsdVideoRange(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

 if (pStorage->activeSps && pStorage->activeSps->vuiParametersPresentFlag &&
        pStorage->activeSps->vuiParameters &&
        pStorage->activeSps->vuiParameters->videoSignalTypePresentFlag &&
        pStorage->activeSps->vuiParameters->videoFullRangeFlag)
 return(1);
 else  
 return(0);

}
