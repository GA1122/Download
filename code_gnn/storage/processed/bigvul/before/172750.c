static void SetPicNums(dpbStorage_t *dpb, u32 currFrameNum)
{

 

    u32 i;
    i32 frameNumWrap;

 

    ASSERT(dpb);
    ASSERT(currFrameNum < dpb->maxFrameNum);

 for (i = 0; i < dpb->numRefFrames; i++)
 if (IS_SHORT_TERM(dpb->buffer[i]))
 {
 if (dpb->buffer[i].frameNum > currFrameNum)
                frameNumWrap =
 (i32)dpb->buffer[i].frameNum - (i32)dpb->maxFrameNum;
 else
                frameNumWrap = (i32)dpb->buffer[i].frameNum;
            dpb->buffer[i].picNum = frameNumWrap;
 }

}
