static EAS_RESULT PopcdlStack (EAS_U32 *pStack, EAS_INT *pStackPtr, EAS_U32 *pValue)
{

  
 if (*pStackPtr < 0)
 return EAS_ERROR_FILE_FORMAT;

  
 *pValue = pStack[*pStackPtr];
 *pStackPtr = *pStackPtr - 1;
 return EAS_SUCCESS;
}
