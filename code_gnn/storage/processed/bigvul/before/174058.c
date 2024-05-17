u32 CropPicture(u8 *pOutImage, u8 *pInImage,
    u32 picWidth, u32 picHeight, CropParams *pCropParams)
{

    u32 i, j;
    u32 outWidth, outHeight;
    u8 *pOut, *pIn;

 if (pOutImage == NULL || pInImage == NULL || pCropParams == NULL ||
 !picWidth || !picHeight)
 {
  
        free(pOutImage);
 return(1);
 }

 if ( ((pCropParams->cropLeftOffset + pCropParams->cropOutWidth) >
           picWidth ) ||
 ((pCropParams->cropTopOffset + pCropParams->cropOutHeight) >
           picHeight ) )
 {
  
        free(pOutImage);
 return(1);
 }

    outWidth = pCropParams->cropOutWidth;
    outHeight = pCropParams->cropOutHeight;

    pIn = pInImage + pCropParams->cropTopOffset*picWidth +
        pCropParams->cropLeftOffset;
    pOut = pOutImage;

  
 for (i = outHeight; i; i--)
 {
 for (j = outWidth; j; j--)
 {
 *pOut++ = *pIn++;
 }
        pIn += picWidth - outWidth;
 }

    outWidth >>= 1;
    outHeight >>= 1;

    pIn = pInImage + picWidth*picHeight +
        pCropParams->cropTopOffset*picWidth/4 + pCropParams->cropLeftOffset/2;

  
 for (i = outHeight; i; i--)
 {
 for (j = outWidth; j; j--)
 {
 *pOut++ = *pIn++;
 }
        pIn += picWidth/2 - outWidth;
 }

    pIn = pInImage + 5*picWidth*picHeight/4 +
        pCropParams->cropTopOffset*picWidth/4 + pCropParams->cropLeftOffset/2;

  
 for (i = outHeight; i; i--)
 {
 for (j = outWidth; j; j--)
 {
 *pOut++ = *pIn++;
 }
        pIn += picWidth/2 - outWidth;
 }

 return (0);

}
