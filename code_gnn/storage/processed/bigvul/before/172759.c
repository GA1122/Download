u32 h264bsdMarkDecRefPic(
 dpbStorage_t *dpb,
 decRefPicMarking_t *mark,
 image_t *image,
  u32 frameNum,
  i32 picOrderCnt,
  u32 isIdr,
  u32 currentPicId,
  u32 numErrMbs)
{

 

    u32 i, status;
    u32 markedAsLongTerm;
    u32 toBeDisplayed;

 

    ASSERT(dpb);
    ASSERT(mark || !isIdr);
    ASSERT(!isIdr || (frameNum == 0 && picOrderCnt == 0));
    ASSERT(frameNum < dpb->maxFrameNum);

 if (image->data != dpb->currentOut->data)
 {
        EPRINT("TRYING TO MARK NON-ALLOCATED IMAGE");
 return(HANTRO_NOK);
 }

    dpb->lastContainsMmco5 = HANTRO_FALSE;
    status = HANTRO_OK;

    toBeDisplayed = dpb->noReordering ? HANTRO_FALSE : HANTRO_TRUE;

  
 if (mark == NULL)
 {
        dpb->currentOut->status = UNUSED;
        dpb->currentOut->frameNum = frameNum;
        dpb->currentOut->picNum = (i32)frameNum;
        dpb->currentOut->picOrderCnt = picOrderCnt;
        dpb->currentOut->toBeDisplayed = toBeDisplayed;
 if (!dpb->noReordering)
            dpb->fullness++;
 }
  
 else if (isIdr)
 {

  
        dpb->numOut = dpb->outIndex = 0;

  
 Mmcop5(dpb);
  
 if (mark->noOutputOfPriorPicsFlag || dpb->noReordering)
 {
            dpb->numOut = 0;
            dpb->outIndex = 0;
 }

 if (mark->longTermReferenceFlag)
 {
            dpb->currentOut->status = LONG_TERM;
            dpb->maxLongTermFrameIdx = 0;
 }
 else
 {
            dpb->currentOut->status = SHORT_TERM;
            dpb->maxLongTermFrameIdx = NO_LONG_TERM_FRAME_INDICES;
 }
        dpb->currentOut->frameNum  = 0;
        dpb->currentOut->picNum    = 0;
        dpb->currentOut->picOrderCnt = 0;
        dpb->currentOut->toBeDisplayed = toBeDisplayed;
        dpb->fullness = 1;
        dpb->numRefFrames = 1;
 }
  
 else
 {
        markedAsLongTerm = HANTRO_FALSE;
 if (mark->adaptiveRefPicMarkingModeFlag)
 {
            i = 0;
 while (mark->operation[i].memoryManagementControlOperation)
 {
 switch (mark->operation[i].memoryManagementControlOperation)
 {
 case 1:
                        status = Mmcop1(
                          dpb,
                          frameNum,
                          mark->operation[i].differenceOfPicNums);
 break;

 case 2:
                        status = Mmcop2(dpb, mark->operation[i].longTermPicNum);
 break;

 case 3:
                        status = Mmcop3(
                          dpb,
                          frameNum,
                          mark->operation[i].differenceOfPicNums,
                          mark->operation[i].longTermFrameIdx);
 break;

 case 4:
                        status = Mmcop4(
                          dpb,
                          mark->operation[i].maxLongTermFrameIdx);
 break;

 case 5:
                        status = Mmcop5(dpb);
                        dpb->lastContainsMmco5 = HANTRO_TRUE;
                        frameNum = 0;
 break;

 case 6:
                        status = Mmcop6(
                          dpb,
                          frameNum,
                          picOrderCnt,
                          mark->operation[i].longTermFrameIdx);
 if (status == HANTRO_OK)
                            markedAsLongTerm = HANTRO_TRUE;
 break;

 default:  
                        status = HANTRO_NOK;
 break;
 }
 if (status != HANTRO_OK)
 {
 break;
 }
                i++;
 }
 }
 else
 {
            status = SlidingWindowRefPicMarking(dpb);
 }
  
 if (!markedAsLongTerm)
 {
 if (dpb->numRefFrames < dpb->maxRefFrames)
 {
                dpb->currentOut->frameNum = frameNum;
                dpb->currentOut->picNum   = (i32)frameNum;
                dpb->currentOut->picOrderCnt = picOrderCnt;
                dpb->currentOut->status   = SHORT_TERM;
                dpb->currentOut->toBeDisplayed = toBeDisplayed;
                dpb->fullness++;
                dpb->numRefFrames++;
 }
  
 else
 {
                status = HANTRO_NOK;
 }
 }
 }

    dpb->currentOut->isIdr = isIdr;
    dpb->currentOut->picId = currentPicId;
    dpb->currentOut->numErrMbs = numErrMbs;

  
 if (dpb->noReordering)
 {
        ASSERT(dpb->numOut == 0);
        ASSERT(dpb->outIndex == 0);
        dpb->outBuf[dpb->numOut].data  = dpb->currentOut->data;
        dpb->outBuf[dpb->numOut].isIdr = dpb->currentOut->isIdr;
        dpb->outBuf[dpb->numOut].picId = dpb->currentOut->picId;
        dpb->outBuf[dpb->numOut].numErrMbs = dpb->currentOut->numErrMbs;
        dpb->numOut++;
 }
 else
 {
  
 while (dpb->fullness > dpb->dpbSize)
 {
            i = OutputPicture(dpb);
            ASSERT(i == HANTRO_OK);
 }
 }

  
 ShellSort(dpb->buffer, dpb->dpbSize+1);

 return(status);

}
