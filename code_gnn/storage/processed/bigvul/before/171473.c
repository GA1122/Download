static EAS_RESULT Parse_data (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_I32 size, S_WSMP_DATA *pWsmp, EAS_SAMPLE *pSample, EAS_U32 sampleLen)
{
    EAS_RESULT result;
    EAS_U8 convBuf[SAMPLE_CONVERT_CHUNK_SIZE];
    EAS_I32 count;
    EAS_I32 i;
    EAS_I8 *p;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
    p = pSample;
 if (pWsmp->bitsPerSample == 8)
 {
 if ((result = EAS_HWReadFile(pDLSData->hwInstData, pDLSData->fileHandle, pSample, size, &count)) != EAS_SUCCESS)
 return result;
 for (i = 0; i < size; i++)
  
 *p++ ^= 0x80;
 }

  
 else
 {

 while (size)
 {
            EAS_I8 *pInput;

  
 if (pDLSData->bigEndian)
                pInput = (EAS_I8*) convBuf;
 else
                pInput = (EAS_I8*) convBuf + 1;

  
            count = (size < SAMPLE_CONVERT_CHUNK_SIZE ? size : SAMPLE_CONVERT_CHUNK_SIZE);
 if ((result = EAS_HWReadFile(pDLSData->hwInstData, pDLSData->fileHandle, convBuf, count, &count)) != EAS_SUCCESS)
 return result;
            size -= count;
  
            count = count >> 1;

 while (count--)
 {
 *p++ = *pInput;
                pInput += 2;
 }
 }
 }

  
 if (pWsmp->loopLength)
 {
 if (sampleLen < sizeof(EAS_SAMPLE)
 || (pWsmp->loopStart + pWsmp->loopLength) * sizeof(EAS_SAMPLE) > sampleLen - sizeof(EAS_SAMPLE))
 {
 return EAS_FAILURE;
 }

        pSample[pWsmp->loopStart + pWsmp->loopLength] = pSample[pWsmp->loopStart];
 }

 return EAS_SUCCESS;
}
