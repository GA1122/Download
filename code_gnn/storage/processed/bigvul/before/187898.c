 u32 h264bsdInit(storage_t *pStorage, u32 noOutputReordering)
 {
 
  
     u32 size;
  
 
     ASSERT(pStorage);
 
     h264bsdInitStorage(pStorage);
 
   
      size = (sizeof(macroblockLayer_t) + 63) & ~0x3F;
  
    pStorage->mbLayer = (macroblockLayer_t*)H264SwDecMalloc(size);
//     pStorage->mbLayer = (macroblockLayer_t*)H264SwDecMalloc(size, 1);
      if (!pStorage->mbLayer)
          return HANTRO_NOK;
  
  if (noOutputReordering)
         pStorage->noReordering = HANTRO_TRUE;
 
  return HANTRO_OK;
 }