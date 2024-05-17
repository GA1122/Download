static i32 FindDpbPic(dpbStorage_t *dpb, i32 picNum, u32 isShortTerm)
{

 

    u32 i = 0;
    u32 found = HANTRO_FALSE;

 

 if (isShortTerm)
 {
 while (i < dpb->maxRefFrames && !found)
 {
 if (IS_SHORT_TERM(dpb->buffer[i]) &&
              dpb->buffer[i].picNum == picNum)
                found = HANTRO_TRUE;
 else
                i++;
 }
 }
 else
 {
        ASSERT(picNum >= 0);
 while (i < dpb->maxRefFrames && !found)
 {
 if (IS_LONG_TERM(dpb->buffer[i]) &&
              dpb->buffer[i].picNum == picNum)
                found = HANTRO_TRUE;
 else
                i++;
 }
 }

 if (found)
 return((i32)i);
 else
 return(-1);

}
