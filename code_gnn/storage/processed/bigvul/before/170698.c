static EAS_RESULT Parse_rgn (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_I32 pos, EAS_I32 size, EAS_U16 artIndex)
{
    EAS_RESULT result;
    EAS_U32 temp;
    EAS_I32 chunkPos;
    EAS_I32 endChunk;
    EAS_I32 rgnhPos;
    EAS_I32 lartPos;
    EAS_I32 lartSize;
    EAS_I32 lar2Pos;
    EAS_I32 lar2Size;
    EAS_I32 wlnkPos;
    EAS_I32 wsmpPos;
    EAS_U32 waveIndex;
    S_DLS_ART_VALUES art;
    S_WSMP_DATA wsmp;
    S_WSMP_DATA *pWsmp;
    EAS_U16 regionIndex;

  
 if ((result = EAS_HWFileSeek(pDLSData->hwInstData, pDLSData->fileHandle, pos)) != EAS_SUCCESS)
 return result;

  
    rgnhPos = lartPos = lartSize = lar2Pos = lar2Size = wsmpPos = wlnkPos = 0;
    regionIndex = (EAS_U16) pDLSData->regionCount;

  
    endChunk = pos + size;
 while (pos < endChunk)
 {
        chunkPos = pos;

  
 if ((result = NextChunk(pDLSData, &pos, &temp, &size)) != EAS_SUCCESS)
 return result;

  
 switch (temp)
 {
 case CHUNK_CDL:
 if ((result = Parse_cdl(pDLSData, size, &temp)) != EAS_SUCCESS)
 return result;

  
 if (!temp)
 return EAS_SUCCESS;
 break;

 case CHUNK_RGNH:
                rgnhPos = chunkPos + 8;
 break;

 case CHUNK_WLNK:
                wlnkPos = chunkPos + 8;
 break;

 case CHUNK_WSMP:
                wsmpPos = chunkPos + 8;
 break;

 case CHUNK_LART:
                lartPos = chunkPos + 12;
                lartSize = size;
 break;

 case CHUNK_LAR2:
                lar2Pos = chunkPos + 12;
                lar2Size = size;
 break;

 default:
 break;
 }
 }

  
 if (!rgnhPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if (!wlnkPos)
 {
 {   }
 return EAS_ERROR_UNRECOGNIZED_FORMAT;
 }

  
 if ((result = Parse_wlnk(pDLSData, wlnkPos, &waveIndex)) != EAS_SUCCESS)
 return result;
    pWsmp = &pDLSData->wsmpData[waveIndex];

  
    EAS_HWMemCpy(&art, &defaultArt, sizeof(S_DLS_ART_VALUES));
 if (lartPos)
 {
 if ((result = Parse_lart(pDLSData, lartPos, lartSize, &art)) != EAS_SUCCESS)
 return result;
 }

 if (lar2Pos)
 {
 if ((result = Parse_lart(pDLSData, lar2Pos, lar2Size, &art)) != EAS_SUCCESS)
 return result;
 }

  
 if (pDLSData->pDLS)
 {

  
 if (art.values[PARAM_MODIFIED] == EAS_TRUE)
 {
 Convert_art(pDLSData, &art, (EAS_U16) pDLSData->artCount);
            artIndex = (EAS_U16) pDLSData->artCount;
 }

  
 if ((result = Parse_rgnh(pDLSData, rgnhPos, &pDLSData->pDLS->pDLSRegions[regionIndex & REGION_INDEX_MASK])) != EAS_SUCCESS)
 return result;

  
 if (wsmpPos)
 {
            EAS_HWMemCpy(&wsmp, pWsmp, sizeof(wsmp));
 if ((result = Parse_wsmp(pDLSData, wsmpPos, &wsmp)) != EAS_SUCCESS)
 return result;

            pWsmp = &wsmp;
 }

 Convert_rgn(pDLSData, regionIndex, artIndex, (EAS_U16) waveIndex, pWsmp);
 }

  
 if (art.values[PARAM_MODIFIED])
        pDLSData->artCount++;

  
    pDLSData->regionCount++;
 return EAS_SUCCESS;
}
