static EAS_RESULT Parse_fmt (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, S_WSMP_DATA *p)
{
    EAS_RESULT result;
    EAS_U16 wtemp;
    EAS_U32 ltemp;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &wtemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if (wtemp != WAVE_FORMAT_PCM)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &wtemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;
 if (wtemp != 1)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->sampleRate, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle, &ltemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &wtemp, EAS_FALSE)) != EAS_SUCCESS)
 return result;

  
 if ((result = EAS_HWGetWord(pDLSData->hwInstData, pDLSData->fileHandle, &p->bitsPerSample, EAS_FALSE)) != EAS_SUCCESS)
 return result;

 if ((p->bitsPerSample != 8) && (p->bitsPerSample != 16))
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

 return EAS_SUCCESS;
}
