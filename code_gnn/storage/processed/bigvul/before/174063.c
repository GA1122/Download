void h264bsdCroppingParams(storage_t *pStorage, u32 *croppingFlag,
    u32 *leftOffset, u32 *width, u32 *topOffset, u32 *height)
{

 

 

    ASSERT(pStorage);

 if (pStorage->activeSps && pStorage->activeSps->frameCroppingFlag)
 {
 *croppingFlag = 1;
 *leftOffset = 2 * pStorage->activeSps->frameCropLeftOffset;
 *width = 16 * pStorage->activeSps->picWidthInMbs -
 2 * (pStorage->activeSps->frameCropLeftOffset +
                      pStorage->activeSps->frameCropRightOffset);
 *topOffset = 2 * pStorage->activeSps->frameCropTopOffset;
 *height = 16 * pStorage->activeSps->picHeightInMbs -
 2 * (pStorage->activeSps->frameCropTopOffset +
                       pStorage->activeSps->frameCropBottomOffset);
 }
 else
 {
 *croppingFlag = 0;
 *leftOffset = 0;
 *width = 0;
 *topOffset = 0;
 *height = 0;
 }

}
