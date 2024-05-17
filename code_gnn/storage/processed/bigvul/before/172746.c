static u32 Mmcop4(dpbStorage_t *dpb, u32 maxLongTermFrameIdx)
{

 

    u32 i;

 

    dpb->maxLongTermFrameIdx = maxLongTermFrameIdx;

 for (i = 0; i < dpb->maxRefFrames; i++)
 if (IS_LONG_TERM(dpb->buffer[i]) &&
 ( ((u32)dpb->buffer[i].picNum > maxLongTermFrameIdx) ||
 (dpb->maxLongTermFrameIdx == NO_LONG_TERM_FRAME_INDICES) ) )
 {
            SET_UNUSED(dpb->buffer[i]);
            dpb->numRefFrames--;
 if (!dpb->buffer[i].toBeDisplayed)
                dpb->fullness--;
 }

 return(HANTRO_OK);

}
