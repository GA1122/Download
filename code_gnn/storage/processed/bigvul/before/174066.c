u32 h264bsdMatrixCoefficients(storage_t *pStorage)
{

 

 

    ASSERT(pStorage);

 if (pStorage->activeSps && pStorage->activeSps->vuiParametersPresentFlag &&
        pStorage->activeSps->vuiParameters &&
        pStorage->activeSps->vuiParameters->videoSignalTypePresentFlag &&
        pStorage->activeSps->vuiParameters->colourDescriptionPresentFlag)
 return(pStorage->activeSps->vuiParameters->matrixCoefficients);
 else  
 return(2);

}
