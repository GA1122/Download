 size_t jsuGetFreeStack() {
 #ifdef ARM
   void *frame = __builtin_frame_address(0);
   size_t stackPos = (size_t)((char*)frame);
   size_t stackEnd = (size_t)((char*)&LINKER_END_VAR);
   if (stackPos < stackEnd) return 0;  
   return  stackPos - stackEnd;
 #elif defined(LINUX)
    char ptr;  
    extern void *STACK_BASE;
    uint32_t count =  (uint32_t)((size_t)STACK_BASE - (size_t)&ptr);
  return 1000000 - count;  
//   const uint32_t max_stack = 1000000;  
//   if (count>max_stack) return 0;
//   return max_stack - count;
  #else
    return 1000000;  
 #endif
 }