static EAS_RESULT PushcdlStack (EAS_U32 *pStack, EAS_INT *pStackPtr, EAS_U32 value)
{

  
 if (*pStackPtr >= CDL_STACK_SIZE)
 return EAS_ERROR_FILE_FORMAT;

  
 *pStackPtr = *pStackPtr + 1;
    pStack[*pStackPtr] = value;
 return EAS_SUCCESS;
}
