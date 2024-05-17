u32 CheckPps(picParamSet_t *pps, seqParamSet_t *sps)
{

    u32 i;
    u32 picSize;

    picSize = sps->picWidthInMbs * sps->picHeightInMbs;

  
 if (pps->numSliceGroups > 1)
 {
 if (pps->sliceGroupMapType == 0)
 {
            ASSERT(pps->runLength);
 for (i = 0; i < pps->numSliceGroups; i++)
 {
 if (pps->runLength[i] > picSize)
 return(HANTRO_NOK);
 }
 }
 else if (pps->sliceGroupMapType == 2)
 {
            ASSERT(pps->topLeft);
            ASSERT(pps->bottomRight);
 for (i = 0; i < pps->numSliceGroups-1; i++)
 {
 if (pps->topLeft[i] > pps->bottomRight[i] ||
                    pps->bottomRight[i] >= picSize)
 return(HANTRO_NOK);

 if ( (pps->topLeft[i] % sps->picWidthInMbs) >
 (pps->bottomRight[i] % sps->picWidthInMbs) )
 return(HANTRO_NOK);
 }
 }
 else if (pps->sliceGroupMapType > 2 && pps->sliceGroupMapType < 6)
 {
 if (pps->sliceGroupChangeRate > picSize)
 return(HANTRO_NOK);
 }
 else if (pps->sliceGroupMapType == 6 &&
                 pps->picSizeInMapUnits < picSize)
 return(HANTRO_NOK);
 }

 return(HANTRO_OK);
}
