u32 h264bsdCheckAccessUnitBoundary(
 strmData_t *strm,
 nalUnit_t *nuNext,
 storage_t *storage,
  u32 *accessUnitBoundaryFlag)
{

 

    u32 tmp, ppsId, frameNum, idrPicId, picOrderCntLsb;
    i32 deltaPicOrderCntBottom, deltaPicOrderCnt[2];
 seqParamSet_t *sps;
 picParamSet_t *pps;

 

    ASSERT(strm);
    ASSERT(nuNext);
    ASSERT(storage);
    ASSERT(storage->sps);
    ASSERT(storage->pps);

  
 *accessUnitBoundaryFlag = HANTRO_FALSE;

 if ( ( (nuNext->nalUnitType > 5) && (nuNext->nalUnitType < 12) ) ||
 ( (nuNext->nalUnitType > 12) && (nuNext->nalUnitType <= 18) ) )
 {
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 return(HANTRO_OK);
 }
 else if ( nuNext->nalUnitType != NAL_CODED_SLICE &&
              nuNext->nalUnitType != NAL_CODED_SLICE_IDR )
 {
 return(HANTRO_OK);
 }

  
 if (storage->aub->firstCallFlag)
 {
 *accessUnitBoundaryFlag = HANTRO_TRUE;
        storage->aub->firstCallFlag = HANTRO_FALSE;
 }

  
    tmp = h264bsdCheckPpsId(strm, &ppsId);
 if (tmp != HANTRO_OK)
 return(tmp);

  
    pps = storage->pps[ppsId];
 if ( pps == NULL || storage->sps[pps->seqParameterSetId] == NULL  ||
 (storage->activeSpsId != MAX_NUM_SEQ_PARAM_SETS &&
          pps->seqParameterSetId != storage->activeSpsId &&
          nuNext->nalUnitType != NAL_CODED_SLICE_IDR) )
 return(PARAM_SET_ERROR);
    sps = storage->sps[pps->seqParameterSetId];

 if (storage->aub->nuPrev->nalRefIdc != nuNext->nalRefIdc &&
 (storage->aub->nuPrev->nalRefIdc == 0 || nuNext->nalRefIdc == 0))
 *accessUnitBoundaryFlag = HANTRO_TRUE;

 if ((storage->aub->nuPrev->nalUnitType == NAL_CODED_SLICE_IDR &&
          nuNext->nalUnitType != NAL_CODED_SLICE_IDR) ||
 (storage->aub->nuPrev->nalUnitType != NAL_CODED_SLICE_IDR &&
       nuNext->nalUnitType == NAL_CODED_SLICE_IDR))
 *accessUnitBoundaryFlag = HANTRO_TRUE;

    tmp = h264bsdCheckFrameNum(strm, sps->maxFrameNum, &frameNum);
 if (tmp != HANTRO_OK)
 return(HANTRO_NOK);

 if (storage->aub->prevFrameNum != frameNum)
 {
        storage->aub->prevFrameNum = frameNum;
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 }

 if (nuNext->nalUnitType == NAL_CODED_SLICE_IDR)
 {
        tmp = h264bsdCheckIdrPicId(strm, sps->maxFrameNum, nuNext->nalUnitType,
 &idrPicId);
 if (tmp != HANTRO_OK)
 return(HANTRO_NOK);

 if (storage->aub->nuPrev->nalUnitType == NAL_CODED_SLICE_IDR &&
          storage->aub->prevIdrPicId != idrPicId)
 *accessUnitBoundaryFlag = HANTRO_TRUE;

        storage->aub->prevIdrPicId = idrPicId;
 }

 if (sps->picOrderCntType == 0)
 {
        tmp = h264bsdCheckPicOrderCntLsb(strm, sps, nuNext->nalUnitType,
 &picOrderCntLsb);
 if (tmp != HANTRO_OK)
 return(HANTRO_NOK);

 if (storage->aub->prevPicOrderCntLsb != picOrderCntLsb)
 {
            storage->aub->prevPicOrderCntLsb = picOrderCntLsb;
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 }

 if (pps->picOrderPresentFlag)
 {
            tmp = h264bsdCheckDeltaPicOrderCntBottom(strm, sps,
                nuNext->nalUnitType, &deltaPicOrderCntBottom);
 if (tmp != HANTRO_OK)
 return(tmp);

 if (storage->aub->prevDeltaPicOrderCntBottom !=
                deltaPicOrderCntBottom)
 {
                storage->aub->prevDeltaPicOrderCntBottom =
                    deltaPicOrderCntBottom;
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 }
 }
 }
 else if (sps->picOrderCntType == 1 && !sps->deltaPicOrderAlwaysZeroFlag)
 {
        tmp = h264bsdCheckDeltaPicOrderCnt(strm, sps, nuNext->nalUnitType,
          pps->picOrderPresentFlag, deltaPicOrderCnt);
 if (tmp != HANTRO_OK)
 return(tmp);

 if (storage->aub->prevDeltaPicOrderCnt[0] != deltaPicOrderCnt[0])
 {
            storage->aub->prevDeltaPicOrderCnt[0] = deltaPicOrderCnt[0];
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 }

 if (pps->picOrderPresentFlag)
 if (storage->aub->prevDeltaPicOrderCnt[1] != deltaPicOrderCnt[1])
 {
                storage->aub->prevDeltaPicOrderCnt[1] = deltaPicOrderCnt[1];
 *accessUnitBoundaryFlag = HANTRO_TRUE;
 }
 }

 *storage->aub->nuPrev = *nuNext;

 return(HANTRO_OK);

}
