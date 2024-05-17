 resetLppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,  
                     UCHAR  highBandStartSb,  
                     UCHAR *v_k_master,  
                     UCHAR  numMaster,  
                     UCHAR *noiseBandTable,  
                     UCHAR  noNoiseBands,  
                     UCHAR  usb,  
                     UINT   fs                         
  )
 {
   TRANSPOSER_SETTINGS *pSettings = hLppTrans->pSettings;
   PATCH_PARAM  *patchParam = pSettings->patchParam;
 
  int i, patch;
  int targetStopBand;
  int sourceStartBand;
  int patchDistance;
  int numBandsInPatch;
 
  int lsb = v_k_master[0];  
  int xoverOffset = highBandStartSb - lsb;  
  int startFreqHz;
 
  int desiredBorder;
 
   usb = fixMin(usb, v_k_master[numMaster]);  
 
   
 
  if ( lsb - SHIFT_START_SB < 4 ) {
  return SBRDEC_UNSUPPORTED_CONFIG;
  }
 
 
   
   
   desiredBorder    = (((2048000*2) / fs) + 1) >> 1;
 
   desiredBorder = findClosestEntry(desiredBorder, v_k_master, numMaster, 1);  
 
   
   sourceStartBand = SHIFT_START_SB + xoverOffset;
   targetStopBand = lsb + xoverOffset;  
 
   
   patch = 0;
  while(targetStopBand < usb) {
 
   
  if (patch > MAX_NUM_PATCHES) {
  return SBRDEC_UNSUPPORTED_CONFIG;
  }
 
     patchParam[patch].guardStartBand = targetStopBand;
     patchParam[patch].targetStartBand = targetStopBand;
 
     numBandsInPatch = desiredBorder - targetStopBand;  
 
  if ( numBandsInPatch >= lsb - sourceStartBand ) {
   
       patchDistance   = targetStopBand - sourceStartBand;  
       patchDistance   = patchDistance & ~1;  
       numBandsInPatch = lsb - (targetStopBand - patchDistance);  
       numBandsInPatch = findClosestEntry(targetStopBand + numBandsInPatch, v_k_master, numMaster, 0) -
                         targetStopBand;  
  }
 
   
     patchDistance   = numBandsInPatch + targetStopBand - lsb;  
     patchDistance   = (patchDistance + 1) & ~1;  
 
  if (numBandsInPatch > 0) {
       patchParam[patch].sourceStartBand = targetStopBand - patchDistance;
       patchParam[patch].targetBandOffs  = patchDistance;
       patchParam[patch].numBandsInPatch = numBandsInPatch;
       patchParam[patch].sourceStopBand  = patchParam[patch].sourceStartBand + numBandsInPatch;
 
       targetStopBand += patchParam[patch].numBandsInPatch;
       patch++;
  }
 
   
     sourceStartBand = SHIFT_START_SB;
 
   
  if( desiredBorder - targetStopBand < 3)  
  {
       desiredBorder = usb;
  }
 
  }
 
   patch--;
 
   
  if ( (patch>0) && (patchParam[patch].numBandsInPatch < 3) ) {
     patch--;
     targetStopBand = patchParam[patch].targetStartBand + patchParam[patch].numBandsInPatch;
  }
 
   
  if (patch >= MAX_NUM_PATCHES) {
  return SBRDEC_UNSUPPORTED_CONFIG;
  }
 
   pSettings->noOfPatches = patch + 1;
 
   
   pSettings->lbStartPatching = targetStopBand;
   pSettings->lbStopPatching  = 0;
  for ( patch = 0; patch < pSettings->noOfPatches; patch++ ) {
     pSettings->lbStartPatching = fixMin( pSettings->lbStartPatching, patchParam[patch].sourceStartBand );
     pSettings->lbStopPatching  = fixMax( pSettings->lbStopPatching, patchParam[patch].sourceStopBand );
  }
 
 
    for(i = 0 ; i < noNoiseBands; i++){
      pSettings->bwBorders[i] = noiseBandTable[i+1];
    }
//   for (;i < MAX_NUM_NOISE_VALUES; i++) {
//     pSettings->bwBorders[i] = 255;
//   }
// 
  
     
 
   startFreqHz = ( (lsb + xoverOffset)*fs ) >> 7;  
 
  for( i = 1; i < NUM_WHFACTOR_TABLE_ENTRIES; i++ )
  {
  if( startFreqHz < FDK_sbrDecoder_sbr_whFactorsIndex[i])
  break;
  }
   i--;
 
   pSettings->whFactors.off = FDK_sbrDecoder_sbr_whFactorsTable[i][0];
   pSettings->whFactors.transitionLevel = FDK_sbrDecoder_sbr_whFactorsTable[i][1];
   pSettings->whFactors.lowLevel = FDK_sbrDecoder_sbr_whFactorsTable[i][2];
   pSettings->whFactors.midLevel = FDK_sbrDecoder_sbr_whFactorsTable[i][3];
   pSettings->whFactors.highLevel = FDK_sbrDecoder_sbr_whFactorsTable[i][4];
 
  return SBRDEC_OK;
 }