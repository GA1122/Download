 static EAS_RESULT PushcdlStack (EAS_U32 *pStack, EAS_INT *pStackPtr, EAS_U32 value)
 
  {
  
       
    if (*pStackPtr >= CDL_STACK_SIZE)
//     if (*pStackPtr >= (CDL_STACK_SIZE - 1)) {
//         ALOGE("b/34031018, stackPtr(%d)", *pStackPtr);
//         android_errorWriteLog(0x534e4554, "34031018");
          return EAS_ERROR_FILE_FORMAT;
//     }
  
       
      *pStackPtr = *pStackPtr + 1;
     pStack[*pStackPtr] = value;
  return EAS_SUCCESS;
 }