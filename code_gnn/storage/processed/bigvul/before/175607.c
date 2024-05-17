Bool PVAllocVideoData(VideoDecControls *decCtrl, int width, int height, int nLayers)
{
 VideoDecData *video = (VideoDecData *) decCtrl->videoDecoderData;
 Bool status = PV_TRUE;
 int nTotalMB;
 int nMBPerRow;
 int32 size;

 if (video->shortVideoHeader == PV_TRUE)
 {
        video->displayWidth = width;
        video->displayHeight = height;
        video->width = (width + 15) & -16;
        video->height = (height + 15) & -16;

        video->nMBPerRow =
            video->nMBinGOB  = video->width / MB_SIZE;
        video->nMBPerCol =
            video->nGOBinVop = video->height / MB_SIZE;
        video->nTotalMB =
            video->nMBPerRow * video->nMBPerCol;
 }

 if (((uint64_t)video->width * video->height) > (uint64_t)INT32_MAX / sizeof(PIXEL)) {
 return PV_FALSE;
 }

    size = (int32)sizeof(PIXEL) * video->width * video->height;
#ifdef PV_MEMORY_POOL
    decCtrl->size = size;
#else
#ifdef DEC_INTERNAL_MEMORY_OPT
    video->currVop->yChan = IMEM_currVop_yChan;  
 if (video->currVop->yChan == NULL) status = PV_FALSE;
    video->currVop->uChan = video->currVop->yChan + size;
    video->currVop->vChan = video->currVop->uChan + (size >> 2);

    video->prevVop->yChan = IMEM_prevVop_yChan;  
 if (video->prevVop->yChan == NULL) status = PV_FALSE;
    video->prevVop->uChan = video->prevVop->yChan + size;
    video->prevVop->vChan = video->prevVop->uChan + (size >> 2);
#else
 if (size > INT32_MAX / 3) {
 return PV_FALSE;
 }
    video->currVop->yChan = (PIXEL *) oscl_malloc(size * 3 / 2);  
 if (video->currVop->yChan == NULL) status = PV_FALSE;

    video->currVop->uChan = video->currVop->yChan + size;
    video->currVop->vChan = video->currVop->uChan + (size >> 2);
    video->prevVop->yChan = (PIXEL *) oscl_malloc(size * 3 / 2);  
 if (video->prevVop->yChan == NULL) status = PV_FALSE;

    video->prevVop->uChan = video->prevVop->yChan + size;
    video->prevVop->vChan = video->prevVop->uChan + (size >> 2);
#endif
    video->memoryUsage += (size * 3);
#endif  
  
  
 if (nLayers > 1)
 {
        video->prevEnhcVop = (Vop *) oscl_malloc(sizeof(Vop));
        video->memoryUsage += (sizeof(Vop));
 if (video->prevEnhcVop == NULL)
 {
            status = PV_FALSE;
 }
 else
 {
            oscl_memset(video->prevEnhcVop, 0, sizeof(Vop));
#ifndef PV_MEMORY_POOL
 if (size > INT32_MAX / 3) {
 return PV_FALSE;
 }

            video->prevEnhcVop->yChan = (PIXEL *) oscl_malloc(size * 3 / 2);  
 if (video->prevEnhcVop->yChan == NULL) status = PV_FALSE;
            video->prevEnhcVop->uChan = video->prevEnhcVop->yChan + size;
            video->prevEnhcVop->vChan = video->prevEnhcVop->uChan + (size >> 2);
            video->memoryUsage += (3 * size / 2);
#endif
 }
 }

  
  
    nTotalMB = video->nTotalMB;
    nMBPerRow = video->nMBPerRow;

#ifdef DEC_INTERNAL_MEMORY_OPT
    video->sliceNo = (uint8 *)(IMEM_sliceNo);
 if (video->sliceNo == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;
    video->acPredFlag = (uint8 *)(IMEM_acPredFlag);
 if (video->acPredFlag == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB);
    video->predDC = (typeDCStore *)(IMEM_predDC);
 if (video->predDC == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB * sizeof(typeDCStore));
    video->predDCAC_col = (typeDCACStore *)(IMEM_predDCAC_col);
 if (video->predDCAC_col == NULL) status = PV_FALSE;
    video->memoryUsage += ((nMBPerRow + 1) * sizeof(typeDCACStore));
    video->predDCAC_row = video->predDCAC_col + 1;
    video->headerInfo.Mode = (uint8 *)(IMEM_headerInfo_Mode);
 if (video->headerInfo.Mode == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;
    video->headerInfo.CBP = (uint8 *)(IMEM_headerInfo_CBP);
 if (video->headerInfo.CBP == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;
    video->QPMB = (int *)(IMEM_headerInfo_QPMB);
 if (video->QPMB == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB * sizeof(int));
    video->mblock = &IMEM_mblock;
 if (video->mblock == NULL) status = PV_FALSE;
    oscl_memset(video->mblock->block, 0, sizeof(int16)*6*NCOEFF_BLOCK);  

    video->memoryUsage += sizeof(MacroBlock);
    video->motX = (MOT *)(IMEM_motX);
 if (video->motX == NULL) status = PV_FALSE;
    video->motY = (MOT *)(IMEM_motY);
 if (video->motY == NULL) status = PV_FALSE;
    video->memoryUsage += (sizeof(MOT) * 8 * nTotalMB);
#else
    video->sliceNo = (uint8 *) oscl_malloc(nTotalMB);
 if (video->sliceNo == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;

    video->acPredFlag = (uint8 *) oscl_malloc(nTotalMB * sizeof(uint8));
 if (video->acPredFlag == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB);

 if ((size_t)nTotalMB > SIZE_MAX / sizeof(typeDCStore)) {
 return PV_FALSE;
 }
    video->predDC = (typeDCStore *) oscl_malloc(nTotalMB * sizeof(typeDCStore));
 if (video->predDC == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB * sizeof(typeDCStore));

 if (nMBPerRow > INT32_MAX - 1
 || (size_t)(nMBPerRow + 1) > SIZE_MAX / sizeof(typeDCACStore)) {
 return PV_FALSE;
 }
    video->predDCAC_col = (typeDCACStore *) oscl_malloc((nMBPerRow + 1) * sizeof(typeDCACStore));
 if (video->predDCAC_col == NULL) status = PV_FALSE;
    video->memoryUsage += ((nMBPerRow + 1) * sizeof(typeDCACStore));

  
  
    video->predDCAC_row = video->predDCAC_col + 1;  

  
    video->headerInfo.Mode = (uint8 *) oscl_malloc(nTotalMB);
 if (video->headerInfo.Mode == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;
    video->headerInfo.CBP = (uint8 *) oscl_malloc(nTotalMB);
 if (video->headerInfo.CBP == NULL) status = PV_FALSE;
    video->memoryUsage += nTotalMB;

 if ((size_t)nTotalMB > SIZE_MAX / sizeof(int16)) {
 return PV_FALSE;
 }
    video->QPMB = (int16 *) oscl_malloc(nTotalMB * sizeof(int16));
 if (video->QPMB == NULL) status = PV_FALSE;
    video->memoryUsage += (nTotalMB * sizeof(int));

  
    video->mblock = (MacroBlock *) oscl_malloc(sizeof(MacroBlock));
 if (video->mblock == NULL)
 {
        status = PV_FALSE;
 }
 else
 {
        oscl_memset(video->mblock->block, 0, sizeof(int16)*6*NCOEFF_BLOCK);  

        video->memoryUsage += sizeof(MacroBlock);
 }
  
 if ((size_t)nTotalMB > SIZE_MAX / (sizeof(MOT) * 4)) {
 return PV_FALSE;
 }
    video->motX = (MOT *) oscl_malloc(sizeof(MOT) * 4 * nTotalMB);
 if (video->motX == NULL) status = PV_FALSE;
    video->motY = (MOT *) oscl_malloc(sizeof(MOT) * 4 * nTotalMB);
 if (video->motY == NULL) status = PV_FALSE;
    video->memoryUsage += (sizeof(MOT) * 8 * nTotalMB);
#endif

#ifdef PV_POSTPROC_ON
  
#ifdef DEC_INTERNAL_MEMORY_OPT
    video->pstprcTypCur = IMEM_pstprcTypCur;
    video->memoryUsage += (nTotalMB * 6);
 if (video->pstprcTypCur == NULL)
 {
        status = PV_FALSE;
 }
 else
 {
        oscl_memset(video->pstprcTypCur, 0, 4*nTotalMB + 2*nTotalMB);
 }

    video->pstprcTypPrv = IMEM_pstprcTypPrv;
    video->memoryUsage += (nTotalMB * 6);
 if (video->pstprcTypPrv == NULL)
 {
        status = PV_FALSE;
 }
 else
 {
        oscl_memset(video->pstprcTypPrv, 0, nTotalMB*6);
 }

#else
 if (nTotalMB > INT32_MAX / 6) {
 return PV_FALSE;
 }
    video->pstprcTypCur = (uint8 *) oscl_malloc(nTotalMB * 6);
    video->memoryUsage += (nTotalMB * 6);
 if (video->pstprcTypCur == NULL)
 {
        status = PV_FALSE;
 }
 else
 {
        oscl_memset(video->pstprcTypCur, 0, 4*nTotalMB + 2*nTotalMB);
 }

    video->pstprcTypPrv = (uint8 *) oscl_malloc(nTotalMB * 6);
    video->memoryUsage += (nTotalMB * 6);
 if (video->pstprcTypPrv == NULL)
 {
        status = PV_FALSE;
 }
 else
 {
        oscl_memset(video->pstprcTypPrv, 0, nTotalMB*6);
 }

#endif

#endif

  
    video->prevVop->predictionType = I_VOP;
    video->prevVop->timeStamp = 0;
#ifndef PV_MEMORY_POOL
    oscl_memset(video->prevVop->yChan, 16, sizeof(uint8)*size);  
    oscl_memset(video->prevVop->uChan, 128, sizeof(uint8)*size / 2);

    oscl_memset(video->currVop->yChan, 0, sizeof(uint8)*size*3 / 2);
 if (nLayers > 1)
 {
        oscl_memset(video->prevEnhcVop->yChan, 0, sizeof(uint8)*size*3 / 2);
        video->prevEnhcVop->timeStamp = 0;
 }
    video->concealFrame = video->prevVop->yChan;  
    decCtrl->outputFrame = video->prevVop->yChan;  
#endif

  
    video->currLayer = 0;
 return status;
}
