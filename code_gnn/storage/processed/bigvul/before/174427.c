static int aacDecoder_drcParse (
    HANDLE_FDK_BITSTREAM  bs,
 CDrcPayload *pDrcBs,
    UINT                  payloadPosition )
{
 int i, numBands, bitCnt = 4;

  
 FDKpushBiDirectional(bs, FDKgetValidBits(bs)-payloadPosition);

  
 if (FDKreadBits(bs,1))
 {
    pDrcBs->pceInstanceTag = FDKreadBits(bs, 4);  
  
 FDKreadBits(bs, 4);  
    bitCnt += 8;
 } else {
    pDrcBs->pceInstanceTag = -1;  
 }

 if (FDKreadBits(bs,1)) {  
  
    bitCnt += parseExcludedChannels(&pDrcBs->excludedChnsMask, bs);
 } else {
    pDrcBs->excludedChnsMask = 0;
 }

  numBands = 1;
 if (FDKreadBits(bs,1))  
 {
  
    numBands += FDKreadBits(bs, 4);  
    pDrcBs->channelData.drcInterpolationScheme = FDKreadBits(bs, 4);  
    bitCnt += 8;
  
 for (i = 0; i < numBands; i++)
 {
      pDrcBs->channelData.bandTop[i] = FDKreadBits(bs, 8);  
      bitCnt += 8;
 }
 }
 else {
    pDrcBs->channelData.bandTop[0] = (1024 >> 2) - 1;  ;
 }

  pDrcBs->channelData.numBands = numBands;

 if (FDKreadBits(bs,1))  
 {
    pDrcBs->progRefLevel = FDKreadBits(bs, 7);  
 FDKreadBits(bs, 1);  
    bitCnt += 8;
 } else {
    pDrcBs->progRefLevel = -1;
 }

 for (i = 0; i < numBands; i++)
 {
    pDrcBs->channelData.drcValue[i] = FDKreadBits(bs, 1) << 7;  
    pDrcBs->channelData.drcValue[i] |= FDKreadBits(bs, 7) & 0x7F;  
    bitCnt += 8;
 }

  
  pDrcBs->channelData.drcDataType = MPEG_DRC_EXT_DATA;

 return (bitCnt);
}
