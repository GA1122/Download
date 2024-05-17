static EAS_RESULT Parse_ptbl (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_I32 wtblPos, EAS_I32 wtblSize)
{
    EAS_RESULT result;
    EAS_U32 temp;
    EAS_FILE_HANDLE tempFile;
    EAS_U16 waveIndex;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &temp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &pDLSData->waveCount, EAS_FALSE)) != EAS_SUCCESS)
 return result;

#if 0
  
 if (!pDLSData->pDLS)
 return EAS_SUCCESS;
#endif

  
 if ((result = EAS_HWDupHandle(pDLSData->hwInstData, pDLSData->fileHandle, &tempFile)) != EAS_SUCCESS)
 return result;

  
 for (waveIndex = 0; waveIndex < pDLSData->waveCount; waveIndex++)
 {

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, tempFile, &temp, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if (temp > (EAS_U32) wtblSize)
 {
 {   }
            EAS_HWCloseFile(pDLSData->hwInstData, tempFile);
 return EAS_ERROR_FILE_FORMAT;
 }

  
 if ((result = Parse_wave(pDLSData, wtblPos +(EAS_I32)  temp, waveIndex)) != EAS_SUCCESS)
 return result;
 }

  
    EAS_HWCloseFile(pDLSData->hwInstData, tempFile);
 return EAS_SUCCESS;
}
