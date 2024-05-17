static EAS_RESULT Parse_wlnk (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_U32 *pWaveIndex)
{
    EAS_RESULT result;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos + 8)) != EAS_SUCCESS)
 return result;

  
 return EAS_HWGetDWord(pDLSData->hwInstData, pDLSData->fileHandle,pWaveIndex, EAS_FALSE);
}
