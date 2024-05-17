static u32 Mmcop6(dpbStorage_t *dpb, u32 frameNum, i32 picOrderCnt,
    u32 longTermFrameIdx)
{

 

    u32 i;

 

    ASSERT(frameNum < dpb->maxFrameNum);

 if ( (dpb->maxLongTermFrameIdx == NO_LONG_TERM_FRAME_INDICES) ||
 (longTermFrameIdx > dpb->maxLongTermFrameIdx) )
 return(HANTRO_NOK);

  
 for (i = 0; i < dpb->maxRefFrames; i++)
 if (IS_LONG_TERM(dpb->buffer[i]) &&
 (u32)dpb->buffer[i].picNum == longTermFrameIdx)
 {
            SET_UNUSED(dpb->buffer[i]);
            dpb->numRefFrames--;
 if (!dpb->buffer[i].toBeDisplayed)
                dpb->fullness--;
 break;
 }

 if (dpb->numRefFrames < dpb->maxRefFrames)
 {
        dpb->currentOut->frameNum = frameNum;
        dpb->currentOut->picNum   = (i32)longTermFrameIdx;
        dpb->currentOut->picOrderCnt = picOrderCnt;
        dpb->currentOut->status   = LONG_TERM;
 if (dpb->noReordering)
            dpb->currentOut->toBeDisplayed = HANTRO_FALSE;
 else
            dpb->currentOut->toBeDisplayed = HANTRO_TRUE;
        dpb->numRefFrames++;
        dpb->fullness++;
 return(HANTRO_OK);
 }
  
 else
 return(HANTRO_NOK);

}
