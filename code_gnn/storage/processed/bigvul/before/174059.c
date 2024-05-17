void CropWriteOutput(FILE *foutput, u8 *imageData, u32 cropDisplay,
        H264SwDecInfo *decInfo)
{
    u8 *tmpImage = NULL;
    u32 tmp, picSize;

 if (cropDisplay && decInfo->croppingFlag)
 {
        picSize = decInfo->cropParams.cropOutWidth *
                  decInfo->cropParams.cropOutHeight;
        picSize = (3 * picSize)/2;
        tmpImage = malloc(picSize);
 if (tmpImage == NULL)
            exit(1);
        tmp = CropPicture(tmpImage, imageData,
            decInfo->picWidth, decInfo->picHeight,
 &(decInfo->cropParams));
 if (tmp)
            exit(1);
 WriteOutput(foutput, tmpImage, picSize);
        free(tmpImage);
 }
 else
 {
        picSize = decInfo->picWidth * decInfo->picHeight;
        picSize = (3 * picSize)/2;
 WriteOutput(foutput, imageData, picSize);
 }

}
