static EAS_RESULT Parse_insh (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_U32 *pRgnCount, EAS_U32 *pLocale)
{
    EAS_RESULT result;
    EAS_U32 bank;
    EAS_U32 program;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, pRgnCount, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &bank, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &program, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if (bank & 0x7fff8080)
 {
 {   }
        bank &= 0xff7f;
 }
 if (program > 127)
 {
 {   }
        program &= 0x7f;
 }

  
 *pLocale = (bank << 8) | program;
 return EAS_SUCCESS;
}
