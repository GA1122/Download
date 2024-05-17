 u32 h264bsdActivateParamSets(storage_t *pStorage, u32 ppsId, u32 isIdr)
 {
 
  
 
     u32 tmp;
     u32 flag;
 
  
 
     ASSERT(pStorage);
     ASSERT(ppsId < MAX_NUM_PIC_PARAM_SETS);
 
   
  if ( (pStorage->pps[ppsId] == NULL) ||
  (pStorage->sps[pStorage->pps[ppsId]->seqParameterSetId] == NULL) )
  {
  return(HANTRO_NOK);
  }
 
   
     tmp = CheckPps(pStorage->pps[ppsId],
                    pStorage->sps[pStorage->pps[ppsId]->seqParameterSetId]);
  if (tmp != HANTRO_OK)
  return(tmp);
 
   
  if (pStorage->activePpsId == MAX_NUM_PIC_PARAM_SETS)
  {
         pStorage->activePpsId = ppsId;
 
          pStorage->activePps = pStorage->pps[ppsId];
          pStorage->activeSpsId = pStorage->activePps->seqParameterSetId;
          pStorage->activeSps = pStorage->sps[pStorage->activeSpsId];
        pStorage->picSizeInMbs =
            pStorage->activeSps->picWidthInMbs *
            pStorage->activeSps->picHeightInMbs;
// 
//          
//         if (pStorage->activeSps->picWidthInMbs == 0)
//         {
//             pStorage->picSizeInMbs = 0;
//         }
//         else if (pStorage->activeSps->picHeightInMbs >
//                  UINT32_MAX / pStorage->activeSps->picWidthInMbs)
//         {
//             return(MEMORY_ALLOCATION_ERROR);
//         }
//         else
//         {
//             pStorage->picSizeInMbs =
//                 pStorage->activeSps->picWidthInMbs *
//                 pStorage->activeSps->picHeightInMbs;
//         }
  
          pStorage->currImage->width = pStorage->activeSps->picWidthInMbs;
          pStorage->currImage->height = pStorage->activeSps->picHeightInMbs;
 
         pStorage->pendingActivation = HANTRO_TRUE;
  }
   
  else if (pStorage->pendingActivation)
  {
         pStorage->pendingActivation = HANTRO_FALSE;
 
         FREE(pStorage->mb);
         FREE(pStorage->sliceGroupMap);
 
         ALLOCATE(pStorage->mb, pStorage->picSizeInMbs, mbStorage_t);
         ALLOCATE(pStorage->sliceGroupMap, pStorage->picSizeInMbs, u32);
  if (pStorage->mb == NULL || pStorage->sliceGroupMap == NULL)
  return(MEMORY_ALLOCATION_ERROR);
 
         H264SwDecMemset(pStorage->mb, 0,
             pStorage->picSizeInMbs * sizeof(mbStorage_t));
 
         h264bsdInitMbNeighbours(pStorage->mb,
             pStorage->activeSps->picWidthInMbs,
             pStorage->picSizeInMbs);
 
   
  if ( pStorage->noReordering ||
              pStorage->activeSps->picOrderCntType == 2 ||
  (pStorage->activeSps->vuiParametersPresentFlag &&
               pStorage->activeSps->vuiParameters->bitstreamRestrictionFlag &&
  !pStorage->activeSps->vuiParameters->numReorderFrames) )
             flag = HANTRO_TRUE;
  else
             flag = HANTRO_FALSE;
 
         tmp = h264bsdResetDpb(pStorage->dpb,
             pStorage->activeSps->picWidthInMbs *
             pStorage->activeSps->picHeightInMbs,
             pStorage->activeSps->maxDpbSize,
             pStorage->activeSps->numRefFrames,
             pStorage->activeSps->maxFrameNum,
             flag);
  if (tmp != HANTRO_OK)
  return(tmp);
  }
  else if (ppsId != pStorage->activePpsId)
  {
   
  if (pStorage->pps[ppsId]->seqParameterSetId != pStorage->activeSpsId)
  {
             DEBUG(("SEQ PARAM SET CHANGING...\n"));
  if (isIdr)
  {
                 pStorage->activePpsId = ppsId;
                 pStorage->activePps = pStorage->pps[ppsId];
                 pStorage->activeSpsId = pStorage->activePps->seqParameterSetId;
                 pStorage->activeSps = pStorage->sps[pStorage->activeSpsId];
                 pStorage->picSizeInMbs =
                     pStorage->activeSps->picWidthInMbs *
                     pStorage->activeSps->picHeightInMbs;
 
                 pStorage->currImage->width = pStorage->activeSps->picWidthInMbs;
                 pStorage->currImage->height =
                     pStorage->activeSps->picHeightInMbs;
 
                 pStorage->pendingActivation = HANTRO_TRUE;
  }
  else
  {
                 DEBUG(("TRYING TO CHANGE SPS IN NON-IDR SLICE\n"));
  return(HANTRO_NOK);
  }
  }
  else
  {
             pStorage->activePpsId = ppsId;
             pStorage->activePps = pStorage->pps[ppsId];
  }
  }
 
  return(HANTRO_OK);
 
 }