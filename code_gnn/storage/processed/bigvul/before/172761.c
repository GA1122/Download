u32 h264bsdResetDpb(
 dpbStorage_t *dpb,
  u32 picSizeInMbs,
  u32 dpbSize,
  u32 maxRefFrames,
  u32 maxFrameNum,
  u32 noReordering)
{

 

    ASSERT(picSizeInMbs);
    ASSERT(maxRefFrames <= MAX_NUM_REF_PICS);
    ASSERT(maxRefFrames <= dpbSize);
    ASSERT(maxFrameNum);
    ASSERT(dpbSize);

    h264bsdFreeDpb(dpb);

 return h264bsdInitDpb(dpb, picSizeInMbs, dpbSize, maxRefFrames,
                          maxFrameNum, noReordering);
}
