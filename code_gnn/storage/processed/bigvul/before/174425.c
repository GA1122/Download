void aacDecoder_drcInitChannelData (
 CDrcChannelData *pDrcChData )
{
 if (pDrcChData != NULL) {
    pDrcChData->expiryCount = 0;
    pDrcChData->numBands    = 1;
    pDrcChData->bandTop[0] = (1024 >> 2) - 1;
    pDrcChData->drcValue[0] = 0;
    pDrcChData->drcInterpolationScheme = 0;
    pDrcChData->drcDataType = UNKNOWN_PAYLOAD;
 }
}
