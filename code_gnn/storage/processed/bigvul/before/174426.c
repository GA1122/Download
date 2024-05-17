int aacDecoder_drcMarkPayload (
    HANDLE_AAC_DRC self,
    HANDLE_FDK_BITSTREAM bs,
    AACDEC_DRC_PAYLOAD_TYPE type )
{
  UINT bsStartPos;
 int  i, numBands = 1, bitCnt = 0;

 if (self == NULL) {
 return 0;
 }

  bsStartPos = FDKgetValidBits(bs);

 switch (type) {
 case MPEG_DRC_EXT_DATA:
 {
      bitCnt = 4;

 if (FDKreadBits(bs,1)) {  
 FDKreadBits(bs,8);  
        bitCnt+=8;
 }

 if (FDKreadBits(bs,1)) {  
 FDKreadBits(bs,7);  
        bitCnt+=8;
 while (FDKreadBits(bs,1)) {  
 FDKreadBits(bs,7);  
          bitCnt+=8;
 }
 }

 if (FDKreadBits(bs,1)) {  
        numBands += FDKreadBits(bs, 4);  
 FDKreadBits(bs,4);  
        bitCnt+=8;
 for (i = 0; i < numBands; i++) {
 FDKreadBits(bs,8);  
          bitCnt+=8;
 }
 }

 if (FDKreadBits(bs,1)) {  
 FDKreadBits(bs,8);  
        bitCnt+=8;
 }

 for (i = 0; i < numBands; i++) {
 FDKreadBits(bs,8);  
        bitCnt+=8;
 }

 if ( (self->numPayloads < MAX_DRC_THREADS)
 && ((INT)FDKgetValidBits(bs) >= 0) )
 {
        self->drcPayloadPosition[self->numPayloads++] = bsStartPos;
 }
 }
 break;

 case DVB_DRC_ANC_DATA:
      bitCnt += 8;
  
 if (FDKreadBits(bs, 8) == DVB_ANC_DATA_SYNC_BYTE)
 {
 int dmxLevelsPresent, compressionPresent;
 int coarseGrainTcPresent, fineGrainTcPresent;

   
 FDKreadBits(bs, 8);  
        bitCnt+=8;

  
 FDKreadBits(bs, 3);  
        dmxLevelsPresent = FDKreadBits(bs, 1);  
 FDKreadBits(bs, 1);  
        compressionPresent   = FDKreadBits(bs, 1);  
        coarseGrainTcPresent = FDKreadBits(bs, 1);  
        fineGrainTcPresent   = FDKreadBits(bs, 1);  
        bitCnt+=8;

  
 if (dmxLevelsPresent) {
 FDKreadBits(bs, 8);  
          bitCnt+=8;
 }
  
 if (compressionPresent) {
 FDKreadBits(bs, 16);  
          bitCnt+=16;
 }
  
 if (coarseGrainTcPresent) {
 FDKreadBits(bs, 16);  
          bitCnt+=16;
 }
  
 if (fineGrainTcPresent) {
 FDKreadBits(bs, 16);  
          bitCnt+=16;
 }
 if ( !self->dvbAncDataAvailable
 && ((INT)FDKgetValidBits(bs) >= 0) )
 {
          self->dvbAncDataPosition  = bsStartPos;
          self->dvbAncDataAvailable = 1;
 }
 }
 break;

 default:
 break;
 }

 return (bitCnt);
}
