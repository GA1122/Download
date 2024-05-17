 void* H264SwDecMalloc(u32 size)
// void* H264SwDecMalloc(u32 size, u32 num)
  {
//     if (size > UINT32_MAX / num) {
//         return NULL;
//     }
  
  #if defined(CHECK_MEMORY_USAGE)
       
      static u32 numBytes = 0;
    numBytes += size;
//     numBytes += size * num;
      DEBUG(("Allocated %d bytes, total %d\n", size, numBytes));
  #endif
  
    return malloc(size);
//     return malloc(size * num);
  }