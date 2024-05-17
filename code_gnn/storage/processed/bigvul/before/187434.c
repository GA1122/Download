 EAS_RESULT DLSParser (EAS_HW_DATA_HANDLE hwInstData, EAS_FILE_HANDLE fileHandle, EAS_I32 offset, EAS_DLSLIB_HANDLE *ppDLS)
 {
     EAS_RESULT result;
     SDLS_SYNTHESIZER_DATA dls;
     EAS_U32 temp;
     EAS_I32 pos;
     EAS_I32 chunkPos;
     EAS_I32 size;
     EAS_I32 instSize;
     EAS_I32 rgnPoolSize;
     EAS_I32 artPoolSize;
     EAS_I32 waveLenSize;
     EAS_I32 endDLS;
     EAS_I32 wvplPos;
     EAS_I32 wvplSize;
     EAS_I32 linsPos;
     EAS_I32 linsSize;
     EAS_I32 ptblPos;
     EAS_I32 ptblSize;
  void *p;
 
   
     EAS_HWMemSet(&dls, 0, sizeof(dls));
 
   
     dls.hwInstData = hwInstData;
     dls.fileHandle = fileHandle;
 
   
  *ppDLS = NULL;
 
   
  if ((result = EAS_HWFileSeek(dls.hwInstData, dls.fileHandle, offset)) != EAS_SUCCESS)
  return result;
  if ((result = EAS_HWReadFile(dls.hwInstData, dls.fileHandle, &temp, sizeof(temp), &size)) != EAS_SUCCESS)
  return result;
 
   
     dls.bigEndian = (temp == CHUNK_RIFF);
 
   
     pos = offset;
  if ((result = NextChunk(&dls, &pos, &temp, &size)) != EAS_SUCCESS)
  return result;
  if (temp != CHUNK_DLS)
  {
  {   }
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
     linsSize = wvplSize = ptblSize = linsPos = wvplPos = ptblPos = 0;
 
   
     endDLS = offset + size;
     pos = offset + 12;
  while (pos < endDLS)
  {
         chunkPos = pos;
 
   
  if ((result = NextChunk(&dls, &pos, &temp, &size)) != EAS_SUCCESS)
  return result;
 
   
  switch (temp)
  {
  case CHUNK_CDL:
  if ((result = Parse_cdl(&dls, size, &temp)) != EAS_SUCCESS)
  return result;
  if (!temp)
  return EAS_ERROR_UNRECOGNIZED_FORMAT;
  break;
 
  case CHUNK_LINS:
                 linsPos = chunkPos + 12;
                 linsSize = size - 4;
  break;
 
  case CHUNK_WVPL:
                 wvplPos = chunkPos + 12;
                 wvplSize = size - 4;
  break;
 
  case CHUNK_PTBL:
                 ptblPos = chunkPos + 8;
                 ptblSize = size - 4;
  break;
 
  default:
  break;
  }
  }
 
   
  if (linsSize == 0)
  {
  {   }
  return EAS_ERROR_UNRECOGNIZED_FORMAT;
  }
 
   
  if (wvplSize == 0)
  {
  {   }
  return EAS_ERROR_UNRECOGNIZED_FORMAT;
  }
 
   
  if ((ptblSize == 0) || (ptblSize > DLS_MAX_WAVE_COUNT * sizeof(POOLCUE) + sizeof(POOLTABLE)))
  {
  {   }
  return EAS_ERROR_UNRECOGNIZED_FORMAT;
  }
 
   
  if ((result = Parse_ptbl(&dls, ptblPos, wvplPos, wvplSize)) != EAS_SUCCESS)
  return result;
 
   
  if ((dls.waveCount == 0) || (dls.waveCount > DLS_MAX_WAVE_COUNT))
  {
  {   }
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
     dls.wsmpData = EAS_HWMalloc(dls.hwInstData, (EAS_I32) (sizeof(S_WSMP_DATA) * dls.waveCount));
  if (dls.wsmpData == NULL)
  {
  {   }
  return EAS_ERROR_MALLOC_FAILED;
  }
     EAS_HWMemSet(dls.wsmpData, 0, (EAS_I32) (sizeof(S_WSMP_DATA) * dls.waveCount));
 
   
     result = Parse_lins(&dls, linsPos, linsSize);
  if (result == EAS_SUCCESS)
  {
 
   
  if ((dls.regionCount == 0) || (dls.regionCount > DLS_MAX_REGION_COUNT))
  {
  {   }
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
  if ((dls.artCount == 0) || (dls.artCount > DLS_MAX_ART_COUNT))
  {
  {   }
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
  if ((dls.instCount == 0) || (dls.instCount > DLS_MAX_INST_COUNT))
  {
  {   }
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
   
         instSize = (EAS_I32) (sizeof(S_PROGRAM) * dls.instCount);
 
   
         rgnPoolSize = (EAS_I32) (sizeof(S_DLS_REGION) * dls.regionCount);
 
   
         dls.artCount++;
         artPoolSize = (EAS_I32) (sizeof(S_DLS_ARTICULATION) * dls.artCount);
 
   
         waveLenSize = (EAS_I32) (dls.waveCount * sizeof(EAS_U32));
 
   
         size = (EAS_I32) sizeof(S_EAS) + instSize + rgnPoolSize + artPoolSize + (2 * waveLenSize) + (EAS_I32) dls.wavePoolSize;
  if (size <= 0) {
  return EAS_ERROR_FILE_FORMAT;
  }
 
   
         dls.pDLS = EAS_HWMalloc(dls.hwInstData, size);
  if (dls.pDLS == NULL)
  {
  {   }
  return EAS_ERROR_MALLOC_FAILED;
  }
         EAS_HWMemSet(dls.pDLS, 0, size);
         dls.pDLS->refCount = 1;
         p = PtrOfs(dls.pDLS, sizeof(S_EAS));
 
   
         dls.pDLS->numDLSPrograms = (EAS_U16) dls.instCount;
         dls.pDLS->pDLSPrograms = p;
         p = PtrOfs(p, instSize);
 
   
         dls.pDLS->pDLSRegions = p;
         dls.pDLS->numDLSRegions = (EAS_U16) dls.regionCount;
         p = PtrOfs(p, rgnPoolSize);
 
   
         dls.pDLS->numDLSArticulations = (EAS_U16) dls.artCount;
         dls.pDLS->pDLSArticulations = p;
         p = PtrOfs(p, artPoolSize);
 
   
         dls.pDLS->numDLSSamples = (EAS_U16) dls.waveCount;
         dls.pDLS->pDLSSampleLen = p;
         p = PtrOfs(p, waveLenSize);
 
   
         dls.pDLS->pDLSSampleOffsets = p;
         p = PtrOfs(p, waveLenSize);
 
   
         dls.pDLS->pDLSSamples = p;
 
   
         dls.filterUsed = EAS_FALSE;
 
   
         result = Parse_ptbl(&dls, ptblPos, wvplPos, wvplSize);
 
      }
  
       
    Convert_art(&dls, &defaultArt, 0);
    dls.artCount = 1;
//     if (dls.pDLS) {
//         Convert_art(&dls, &defaultArt, 0);
//         dls.artCount = 1;
//     }
  
       
      dls.regionCount = dls.instCount = 0;
  if (result == EAS_SUCCESS)
         result = Parse_lins(&dls, linsPos, linsSize);
 
   
  if (dls.wsmpData)
         EAS_HWFree(dls.hwInstData, dls.wsmpData);
 
   
  if (result == EAS_SUCCESS)
  {
  *ppDLS = dls.pDLS;
 #ifdef _DEBUG_DLS
  DumpDLS(dls.pDLS);
 #endif
  }
 
   
  else
  DLSCleanup(dls.hwInstData, dls.pDLS);
 
  return result;
 }