static int aacDecoder_drcReadCompression (
    HANDLE_FDK_BITSTREAM  bs,
 CDrcPayload *pDrcBs,
    UINT                  payloadPosition )
{
 int  bitCnt = 0;
 int  dmxLevelsPresent, extensionPresent, compressionPresent;
 int  coarseGrainTcPresent, fineGrainTcPresent;

  
 FDKpushBiDirectional(bs, FDKgetValidBits(bs)-payloadPosition);

  
 if ( FDKgetValidBits(bs) < 24 ) {
 return 0;
 }

  
 if (FDKreadBits(bs, 8) != DVB_ANC_DATA_SYNC_BYTE) {
 return 0;
 }

   
 if (FDKreadBits(bs, 2) != 3) {  
  
 return 0;
 }
 FDKreadBits(bs, 2);  
  pDrcBs->presMode = FDKreadBits(bs, 2);  
 FDKreadBits(bs, 1);  
 if (FDKreadBits(bs, 1) != 0) {  
 return 0;
 }

  
 if (FDKreadBits(bs, 3) != 0) {  
 return 0;
 }
  dmxLevelsPresent = FDKreadBits(bs, 1);  
  extensionPresent = FDKreadBits(bs, 1);  
  compressionPresent   = FDKreadBits(bs, 1);  
  coarseGrainTcPresent = FDKreadBits(bs, 1);  
  fineGrainTcPresent   = FDKreadBits(bs, 1);  
  bitCnt += 24;

 if (dmxLevelsPresent) {
 FDKreadBits(bs, 8);  
    bitCnt += 8;
 }

  
 if (compressionPresent)
 {
    UCHAR compressionOn, compressionValue;

  
 if ( FDKreadBits(bs, 7) != 0 ) {  
 return 0;
 }
    compressionOn    = (UCHAR)FDKreadBits(bs, 1);  
    compressionValue = (UCHAR)FDKreadBits(bs, 8);  
    bitCnt += 16;

 if ( compressionOn ) {
  
      pDrcBs->channelData.numBands    = 1;  
      pDrcBs->channelData.drcValue[0] =  compressionValue;  
      pDrcBs->channelData.bandTop[0] = (1024 >> 2) - 1;  
      pDrcBs->pceInstanceTag          = -1;  
      pDrcBs->progRefLevel            = -1;  
      pDrcBs->channelData.drcDataType =  DVB_DRC_ANC_DATA;  
 } else {
  
  
  
      pDrcBs->channelData.numBands    = 1;  
      pDrcBs->channelData.drcValue[0] = 0x80;  
      pDrcBs->channelData.bandTop[0] = (1024 >> 2) - 1;  
      pDrcBs->channelData.drcDataType =  DVB_DRC_ANC_DATA;  

  
 if (compressionValue != 0) {
 return 0;
 }
 }
 }

  
 if (coarseGrainTcPresent) {
 FDKreadBits(bs, 16);  
    bitCnt += 16;
 }
 if (fineGrainTcPresent) {
 FDKreadBits(bs, 16);  
    bitCnt += 16;
 }

  
 if (extensionPresent) {
 int  extBits = 8;

 FDKreadBits(bs, 1);  
 if (FDKreadBits(bs, 1)) extBits += 8;  
 if (FDKreadBits(bs, 1)) extBits += 16;  
 if (FDKreadBits(bs, 1)) extBits += 8;  

 FDKpushFor(bs, extBits - 4);  
    bitCnt += extBits;
 }

 return (bitCnt);
}
