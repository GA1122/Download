static EAS_RESULT Parse_lins (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_I32 size)
{
    EAS_RESULT result;
    EAS_U32 temp;
    EAS_I32 endChunk;
    EAS_I32 chunkPos;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
    endChunk = pos + size;
 while (pos < endChunk)
 {
        chunkPos = pos;

  
 if ((result = NextChunk(pDLSData, &pos, &temp, &size)) != EAS_SUCCESS)
 return result;

  
 if (temp != CHUNK_INS)
 continue;

 if ((result = Parse_ins(pDLSData, chunkPos + 12, size)) != EAS_SUCCESS)
 return result;
 }

 return EAS_SUCCESS;
}
