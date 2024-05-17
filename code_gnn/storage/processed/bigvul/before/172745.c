static u32 Mmcop3(dpbStorage_t *dpb, u32 currPicNum, u32 differenceOfPicNums,
    u32 longTermFrameIdx)
{

 

    i32 index, picNum;
    u32 i;

 

    ASSERT(dpb);
    ASSERT(currPicNum < dpb->maxFrameNum);

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

    picNum = (i32)currPicNum - (i32)differenceOfPicNums;

    index = FindDpbPic(dpb, picNum, HANTRO_TRUE);
 if (index < 0)
 return(HANTRO_NOK);
 if (!IS_EXISTING(dpb->buffer[index]))
 return(HANTRO_NOK);

    dpb->buffer[index].status = LONG_TERM;
    dpb->buffer[index].picNum = (i32)longTermFrameIdx;

 return(HANTRO_OK);

}
