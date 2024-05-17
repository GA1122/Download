 static int aacDecoder_drcExtractAndMap (
         HANDLE_AAC_DRC  self,
         HANDLE_FDK_BITSTREAM hBs,
  CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo[],
         UCHAR  pceInstanceTag,
         UCHAR  channelMapping[],  
  int    validChannels )
 {
  CDrcPayload  threadBs[MAX_DRC_THREADS];
  CDrcPayload *validThreadBs[MAX_DRC_THREADS];
  CDrcParams *pParams;
   UINT backupBsPosition;
  int  i, thread, validThreads = 0;
  int  numExcludedChns[MAX_DRC_THREADS];
 
   FDK_ASSERT(self != NULL);
   FDK_ASSERT(hBs != NULL);
   FDK_ASSERT(pAacDecoderStaticChannelInfo != NULL);
 
   pParams = &self->params;
 
   self->numThreads = 0;
   backupBsPosition = FDKgetValidBits(hBs);
 
  for (i = 0; i < self->numPayloads && self->numThreads < MAX_DRC_THREADS; i++) {
  int bitsParsed;
 
   
  FDKmemclear(&threadBs[self->numThreads], sizeof(CDrcPayload));
     threadBs[self->numThreads].channelData.bandTop[0] = (1024 >> 2) - 1;
 
   
     bitsParsed = aacDecoder_drcParse( hBs,
  &threadBs[self->numThreads],
                                       self->drcPayloadPosition[i] );
  if (bitsParsed > 0) {
       self->numThreads++;
  }
 
    }
    self->numPayloads = 0;
  
//   if (self->numThreads >= MAX_DRC_THREADS) {
//       self->numThreads = MAX_DRC_THREADS - 1;
//   }
// 
    if (self->dvbAncDataAvailable)
    {  
      int bitsParsed;
 
   
  FDKmemclear(&threadBs[self->numThreads], sizeof(CDrcPayload));
     threadBs[self->numThreads].channelData.bandTop[0] = (1024 >> 2) - 1;
 
   
     bitsParsed = aacDecoder_drcReadCompression( hBs,
  &threadBs[self->numThreads],
                                                 self->dvbAncDataPosition );
  if (bitsParsed > 0) {
       self->numThreads++;
  }
  }
   self->dvbAncDataAvailable = 0;
 
   
  FDKpushBiDirectional(hBs, FDKgetValidBits(hBs) - backupBsPosition);
 
   
 
  
     
  
//   if (self->numThreads >= MAX_DRC_THREADS) {
//       self->numThreads = MAX_DRC_THREADS - 1;
//   }
// 
     
    for (thread = 0; thread < self->numThreads; thread++) {
      CDrcPayload *pThreadBs = &threadBs[thread];
  int numExclChns = 0;
 
  switch ((AACDEC_DRC_PAYLOAD_TYPE)pThreadBs->channelData.drcDataType) {
  default:
  continue;
  case MPEG_DRC_EXT_DATA:
  case DVB_DRC_ANC_DATA:
  break;
  }
 
  if (pThreadBs->pceInstanceTag >= 0) {  
  if (pThreadBs->pceInstanceTag != pceInstanceTag) {
  continue;  
  }
  }
 
   
  if (pThreadBs->excludedChnsMask > 0) {
       INT exclMask = pThreadBs->excludedChnsMask;
  int ch;
  for (ch = 0; ch < validChannels; ch++) {
         numExclChns += exclMask & 0x1;
         exclMask >>= 1;
  }
  }
  if (numExclChns < validChannels) {
       validThreadBs[validThreads] = pThreadBs;
       numExcludedChns[validThreads] = numExclChns;
       validThreads++;
  }
  }
 
  if (validThreads > 1) {
  int ch;
 
   
  for (ch = 0; ch < validChannels; ch++) {
  int present = 0;
 
  for (thread = 0; thread < validThreads; thread++) {
  CDrcPayload *pThreadBs = validThreadBs[thread];
 
 
   
  if ( (pThreadBs->channelData.drcDataType == MPEG_DRC_EXT_DATA)
  && ( (numExcludedChns[thread] == 0)
  || (!(pThreadBs->excludedChnsMask & (1<<ch))) ) ) {
           present++;
  }
  }
 
 
  if (present > 1) {
  return -1;
  }
  }
  }
 
   
  for (thread = 0; thread < validThreads; thread++)
  {
  CDrcPayload *pThreadBs = validThreadBs[thread];
     INT exclMask = pThreadBs->excludedChnsMask;
     AACDEC_DRC_PAYLOAD_TYPE drcPayloadType = (AACDEC_DRC_PAYLOAD_TYPE)pThreadBs->channelData.drcDataType;
  int ch;
 
   
  if (pThreadBs->progRefLevel >= 0) {
       self->progRefLevel = pThreadBs->progRefLevel;
       self->progRefLevelPresent = 1;
       self->prlExpiryCount = 0;  
  }
 
  if (drcPayloadType == DVB_DRC_ANC_DATA) {
   
       self->presMode = pThreadBs->presMode;
  }
 
   
  for (ch = 0; ch < validChannels; ch++) {
  int mapedChannel = channelMapping[ch];
 
  if ( ((exclMask & (1<<mapedChannel)) == 0)
  && ( (drcPayloadType == MPEG_DRC_EXT_DATA)
  || ((drcPayloadType == DVB_DRC_ANC_DATA) && self->params.applyHeavyCompression)
  ) ) {
   
         pAacDecoderStaticChannelInfo[ch]->drcData = pThreadBs->channelData;
  }
  }
   
  }
 
   
  if ( (pParams->expiryFrame > 0)
  && (self->prlExpiryCount++ > pParams->expiryFrame) )
  {  
     self->progRefLevelPresent = 0;
     self->progRefLevel = pParams->targetRefLevel;
     self->prlExpiryCount = 0;
  }
 
  return 0;
 }