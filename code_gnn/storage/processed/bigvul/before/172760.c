u32 h264bsdReorderRefPicList(
 dpbStorage_t *dpb,
 refPicListReordering_t *order,
  u32 currFrameNum,
  u32 numRefIdxActive)
{

 

    u32 i, j, k, picNumPred, refIdx;
    i32 picNum, picNumNoWrap, index;
    u32 isShortTerm;

 

    ASSERT(order);
    ASSERT(currFrameNum <= dpb->maxFrameNum);
    ASSERT(numRefIdxActive <= MAX_NUM_REF_IDX_L0_ACTIVE);

  
 SetPicNums(dpb, currFrameNum);

 if (!order->refPicListReorderingFlagL0)
 return(HANTRO_OK);

    refIdx     = 0;
    picNumPred = currFrameNum;

    i = 0;
 while (order->command[i].reorderingOfPicNumsIdc < 3)
 {
  
 if (order->command[i].reorderingOfPicNumsIdc < 2)
 {
 if (order->command[i].reorderingOfPicNumsIdc == 0)
 {
                picNumNoWrap =
 (i32)picNumPred - (i32)order->command[i].absDiffPicNum;
 if (picNumNoWrap < 0)
                    picNumNoWrap += (i32)dpb->maxFrameNum;
 }
 else
 {
                picNumNoWrap =
 (i32)(picNumPred + order->command[i].absDiffPicNum);
 if (picNumNoWrap >= (i32)dpb->maxFrameNum)
                    picNumNoWrap -= (i32)dpb->maxFrameNum;
 }
            picNumPred = (u32)picNumNoWrap;
            picNum = picNumNoWrap;
 if ((u32)picNumNoWrap > currFrameNum)
                picNum -= (i32)dpb->maxFrameNum;
            isShortTerm = HANTRO_TRUE;
 }
  
 else
 {
            picNum = (i32)order->command[i].longTermPicNum;
            isShortTerm = HANTRO_FALSE;

 }
  
        index = FindDpbPic(dpb, picNum, isShortTerm);
 if (index < 0 || !IS_EXISTING(dpb->buffer[index]))
 return(HANTRO_NOK);

  
 for (j = numRefIdxActive; j > refIdx; j--)
            dpb->list[j] = dpb->list[j-1];
  
        dpb->list[refIdx++] = &dpb->buffer[index];
  
 for (j = k = refIdx; j <= numRefIdxActive; j++)
 if(dpb->list[j] != &dpb->buffer[index])
                dpb->list[k++] = dpb->list[j];

        i++;
 }

 return(HANTRO_OK);

}
