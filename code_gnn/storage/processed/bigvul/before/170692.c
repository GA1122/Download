static EAS_RESULT Parse_ins (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_I32 size)
{
    EAS_RESULT result;
    EAS_U32 temp;
    EAS_I32 chunkPos;
    EAS_I32 endChunk;
    EAS_I32 lrgnPos;
    EAS_I32 lrgnSize;
    EAS_I32 lartPos;
    EAS_I32 lartSize;
    EAS_I32 lar2Pos;
    EAS_I32 lar2Size;
    EAS_I32 inshPos;
    EAS_U32 regionCount;
    EAS_U32 locale;
    S_DLS_ART_VALUES art;
    S_PROGRAM *pProgram;
    EAS_U16 artIndex;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
    lrgnPos = lrgnSize = lartPos = lartSize = lar2Pos = lar2Size = inshPos = artIndex = 0;

  
    endChunk = pos + size;
 while (pos < endChunk)
 {
        chunkPos = pos;

  
 if ((result = NextChunk(pDLSData, &pos, &temp, &size)) != EAS_SUCCESS)
 return result;

  
 switch (temp)
 {
 case CHUNK_INSH:
                inshPos = chunkPos + 8;
 break;

 case CHUNK_LART:
                lartPos = chunkPos + 12;
                lartSize = size;
 break;

 case CHUNK_LAR2:
                lar2Pos = chunkPos + 12;
                lar2Size = size;
 break;

 case CHUNK_LRGN:
                lrgnPos = chunkPos + 12;
                lrgnSize = size;
 break;

 default:
 break;
 }
 }

  
 if (!lrgnPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if (!inshPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if ((result = Parse_insh(pDLSData, inshPos, &regionCount, &locale)) != EAS_SUCCESS)
 return result;

  
    EAS_HWMemCpy(&art, &defaultArt, sizeof(S_DLS_ART_VALUES));
 if (lartPos)
 if ((result = Parse_lart(pDLSData, lartPos, lartSize, &art)) != EAS_SUCCESS)
 return result;
 if (lar2Pos)
 if ((result = Parse_lart(pDLSData, lar2Pos, lar2Size, &art)) != EAS_SUCCESS)
 return result;

 if (art.values[PARAM_MODIFIED])
 {
        artIndex = (EAS_U16) pDLSData->artCount;
        pDLSData->artCount++;
 }

  
 if (pDLSData->pDLS)
 {

 if (art.values[PARAM_MODIFIED])
 Convert_art(pDLSData, &art, artIndex);

  
        pProgram = &pDLSData->pDLS->pDLSPrograms[pDLSData->instCount];

  
        pProgram->locale = locale;
        pProgram->regionIndex = (EAS_U16) pDLSData->regionCount | FLAG_RGN_IDX_DLS_SYNTH;

 }

  
 if ((result = Parse_lrgn(pDLSData, lrgnPos, lrgnSize, artIndex, regionCount)) != EAS_SUCCESS)
 return result;

  
    pDLSData->instCount++;
 return EAS_SUCCESS;
}
