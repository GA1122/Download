static EAS_RESULT Parse_wave (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_U16 waveIndex)
{
    EAS_RESULT result;
    EAS_U32 temp;
    EAS_I32 size;
    EAS_I32 endChunk;
    EAS_I32 chunkPos;
    EAS_I32 wsmpPos = 0;
    EAS_I32 fmtPos = 0;
    EAS_I32 dataPos = 0;
    EAS_I32 dataSize = 0;
    S_WSMP_DATA *p;
 void *pSample;
    S_WSMP_DATA wsmp;

  
    chunkPos = pos + 12;
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
 if ((result = NextChunk(pDLSData, &pos, &temp, &size)) != EAS_SUCCESS)
 return result;

  
 if (temp != CHUNK_WAVE)
 {
 {   }
 return EAS_ERROR_FILE_FORMAT;
 }

  
    pos = chunkPos;
    endChunk = pos + size;
 while (pos < endChunk)
 {
        chunkPos = pos;

  
 if ((result = NextChunk(pDLSData, &pos, &temp, &size)) != EAS_SUCCESS)
 return result;

  
 switch (temp)
 {
 case CHUNK_WSMP:
                wsmpPos = chunkPos + 8;
 break;

 case CHUNK_FMT:
                fmtPos = chunkPos + 8;
 break;

 case CHUNK_DATA:
                dataPos = chunkPos + 8;
                dataSize = size;
 break;

 default:
 break;
 }
 }

 if (dataSize < 0 || dataSize > MAX_DLS_WAVE_SIZE)
 {
 return EAS_ERROR_SOUND_LIBRARY;
 }

  
 if (pDLSData->pDLS == NULL)
        p = &wsmp;
 else
        p = &pDLSData->wsmpData[waveIndex];

  
    p->fineTune = 0;
    p->unityNote = 60;
    p->gain = 0;
    p->loopStart = 0;
    p->loopLength = 0;

  
 if (!fmtPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if (!dataPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if (wsmpPos)
 {
 if ((result = Parse_wsmp(pDLSData, wsmpPos, p)) != EAS_SUCCESS)
 return result;
 }

  
 if ((result = Parse_fmt(pDLSData, fmtPos, p)) != EAS_SUCCESS)
 return result;

  

  
  
 if (bitDepth == 8)
 {
 if (p->bitsPerSample == 8)
            size = dataSize;
 else
  
            size = dataSize >> 1;
 if (p->loopLength)
            size++;
 }

 else
 {
 if (p->bitsPerSample == 16)
            size = dataSize;
 else
  
            size = dataSize << 1;
 if (p->loopLength)
            size += 2;
 }

  
 if (pDLSData->pDLS == NULL)
 {
        pDLSData->wavePoolSize += (EAS_U32) size;
 return EAS_SUCCESS;
 }

  
    pSample = (EAS_U8*)pDLSData->pDLS->pDLSSamples + pDLSData->wavePoolOffset;
    pDLSData->pDLS->pDLSSampleOffsets[waveIndex] = pDLSData->wavePoolOffset;
    pDLSData->pDLS->pDLSSampleLen[waveIndex] = (EAS_U32) size;
    pDLSData->wavePoolOffset += (EAS_U32) size;
 if (pDLSData->wavePoolOffset > pDLSData->wavePoolSize)
 {
 {   }
 return EAS_ERROR_SOUND_LIBRARY;
 }

 if ((result = Parse_data(pDLSData, dataPos, dataSize, p, pSample, (EAS_U32)size)) != EAS_SUCCESS)
 return result;

 return EAS_SUCCESS;
}
