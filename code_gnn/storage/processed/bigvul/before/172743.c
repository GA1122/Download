static u32 Mmcop1(dpbStorage_t *dpb, u32 currPicNum, u32 differenceOfPicNums)
{

 

    i32 index, picNum;

 

    ASSERT(currPicNum < dpb->maxFrameNum);

    picNum = (i32)currPicNum - (i32)differenceOfPicNums;

    index = FindDpbPic(dpb, picNum, HANTRO_TRUE);
 if (index < 0)
 return(HANTRO_NOK);

    SET_UNUSED(dpb->buffer[index]);
    dpb->numRefFrames--;
 if (!dpb->buffer[index].toBeDisplayed)
        dpb->fullness--;

 return(HANTRO_OK);

}
