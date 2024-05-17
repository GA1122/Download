static EAS_RESULT Parse_wsmp (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, S_WSMP_DATA *p)
{
    EAS_RESULT result;
    EAS_U16 wtemp;
    EAS_U32 ltemp;
    EAS_U32 cbSize;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &cbSize, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &wtemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if (wtemp <= 127)
        p->unityNote = (EAS_U8) wtemp;
 else
 {
        p->unityNote = 60;
 {   }
 }

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->fineTune, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->gain, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if (p->gain > 0)
 {
 {   }
        p->gain = 0;
 }

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &ltemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &ltemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if (ltemp)
 {

 if (ltemp > 1)
 {   }

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos + (EAS_I32) cbSize)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &ltemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &ltemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->loopStart, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->loopLength, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if (p->loopLength
 && ((p->loopStart > EAS_U32_MAX - p->loopLength)
 || (p->loopStart + p->loopLength > EAS_U32_MAX / sizeof(EAS_SAMPLE))))
 {
 return EAS_FAILURE;
 }
 }

 return EAS_SUCCESS;
}
