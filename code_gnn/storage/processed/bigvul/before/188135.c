 void lppTransposer (HANDLE_SBR_LPP_TRANS hLppTrans,  
                     QMF_SCALE_FACTOR  *sbrScaleFactor,  
                     FIXP_DBL **qmfBufferReal,  
 
                     FIXP_DBL *degreeAlias,  
                     FIXP_DBL **qmfBufferImag,  
  const int useLP,
  const int timeStep,  
  const int firstSlotOffs,  
  const int lastSlotOffs,  
  const int nInvfBands,  
                     INVF_MODE *sbr_invf_mode,  
                     INVF_MODE *sbr_invf_mode_prev       
  )
 {
   INT    bwIndex[MAX_NUM_PATCHES];
   FIXP_DBL  bwVector[MAX_NUM_PATCHES];  
 
  int    i;
  int    loBand, start, stop;
   TRANSPOSER_SETTINGS *pSettings = hLppTrans->pSettings;
   PATCH_PARAM *patchParam = pSettings->patchParam;
  int    patch;
 
   FIXP_SGL  alphar[LPC_ORDER], a0r, a1r;
   FIXP_SGL  alphai[LPC_ORDER], a0i=0, a1i=0;
   FIXP_SGL  bw = FL2FXCONST_SGL(0.0f);
 
  int    autoCorrLength;
 
   FIXP_DBL k1, k1_below=0, k1_below2=0;
 
   ACORR_COEFS ac;
  int    startSample;
  int    stopSample;
  int    stopSampleClear;
 
  int comLowBandScale;
  int ovLowBandShift;
  int lowBandShift;
  
  int targetStopBand;
 
 
   alphai[0] = FL2FXCONST_SGL(0.0f);
   alphai[1] = FL2FXCONST_SGL(0.0f);
 
 
   startSample = firstSlotOffs * timeStep;
   stopSample  = pSettings->nCols + lastSlotOffs * timeStep;
 
 
   inverseFilteringLevelEmphasis(hLppTrans, nInvfBands, sbr_invf_mode, sbr_invf_mode_prev, bwVector);
 
   stopSampleClear = stopSample;
 
   autoCorrLength = pSettings->nCols + pSettings->overlap;
 
   
   targetStopBand = patchParam[pSettings->noOfPatches-1].targetStartBand
  + patchParam[pSettings->noOfPatches-1].numBandsInPatch;
 
  int memSize = ((64) - targetStopBand) * sizeof(FIXP_DBL);
 
  if (!useLP) {
  for (i = startSample; i < stopSampleClear; i++) {
  FDKmemclear(&qmfBufferReal[i][targetStopBand], memSize);
  FDKmemclear(&qmfBufferImag[i][targetStopBand], memSize);
  }
  } else
  for (i = startSample; i < stopSampleClear; i++) {
  FDKmemclear(&qmfBufferReal[i][targetStopBand], memSize);
 
    }
  
     
  FDKmemclear(bwIndex, pSettings->noOfPatches*sizeof(INT));
//   FDKmemclear(bwIndex, MAX_NUM_PATCHES*sizeof(INT));
  
     
   comLowBandScale = fixMin(sbrScaleFactor->ov_lb_scale,sbrScaleFactor->lb_scale);
 
   ovLowBandShift =  sbrScaleFactor->ov_lb_scale - comLowBandScale;
   lowBandShift   =  sbrScaleFactor->lb_scale - comLowBandScale;
   
 
   
 
  if (!useLP) {
     start = pSettings->lbStartPatching;
     stop = pSettings->lbStopPatching;
  } else
  {
     start = fixMax(1, pSettings->lbStartPatching - 2);
     stop = patchParam[0].targetStartBand;
  }
 
 
  for ( loBand = start; loBand <  stop; loBand++ ) {
 
     FIXP_DBL  lowBandReal[(((1024)/(32))+(6))+LPC_ORDER];
     FIXP_DBL *plowBandReal = lowBandReal;
     FIXP_DBL **pqmfBufferReal = qmfBufferReal;
     FIXP_DBL  lowBandImag[(((1024)/(32))+(6))+LPC_ORDER];
     FIXP_DBL *plowBandImag = lowBandImag;
     FIXP_DBL **pqmfBufferImag = qmfBufferImag;
  int resetLPCCoeffs=0;
  int dynamicScale = DFRACT_BITS-1-LPC_SCALE_FACTOR;
  int acDetScale = 0;  
 
  for(i=0;i<LPC_ORDER;i++){
  *plowBandReal++ = hLppTrans->lpcFilterStatesReal[i][loBand];
  if (!useLP)
  *plowBandImag++ = hLppTrans->lpcFilterStatesImag[i][loBand];
  }
 
   
  if (!useLP) {
  for(i=0;i<pSettings->nCols+pSettings->overlap;i++){
  *plowBandReal++ = (*pqmfBufferReal++)[loBand];
  *plowBandImag++ = (*pqmfBufferImag++)[loBand];
  }
  } else
  {
   
       FDK_ASSERT((pSettings->overlap & 1) == 0);
 
  for(i=0;i<((pSettings->overlap+pSettings->nCols)>>1);i++) {
  *plowBandReal++ = (*pqmfBufferReal++)[loBand];
  *plowBandReal++ = (*pqmfBufferReal++)[loBand];
  }
  if (pSettings->nCols & 1) {
  *plowBandReal++ = (*pqmfBufferReal++)[loBand];
  }
  }
 
   
     dynamicScale = fixMin(dynamicScale, getScalefactor(lowBandReal, LPC_ORDER+pSettings->overlap) + ovLowBandShift);
     dynamicScale = fixMin(dynamicScale, getScalefactor(&lowBandReal[LPC_ORDER+pSettings->overlap], pSettings->nCols) + lowBandShift);
  if (!useLP) {
       dynamicScale = fixMin(dynamicScale, getScalefactor(lowBandImag, LPC_ORDER+pSettings->overlap) + ovLowBandShift);
       dynamicScale = fixMin(dynamicScale, getScalefactor(&lowBandImag[LPC_ORDER+pSettings->overlap], pSettings->nCols) + lowBandShift);
  }
     dynamicScale = fixMax(0, dynamicScale-1);  
 
   
     scaleValues(&lowBandReal[0], LPC_ORDER+pSettings->overlap, dynamicScale-ovLowBandShift);
     scaleValues(&lowBandReal[LPC_ORDER+pSettings->overlap], pSettings->nCols, dynamicScale-lowBandShift);
 
  if (!useLP) {
       scaleValues(&lowBandImag[0], LPC_ORDER+pSettings->overlap, dynamicScale-ovLowBandShift);
       scaleValues(&lowBandImag[LPC_ORDER+pSettings->overlap], pSettings->nCols, dynamicScale-lowBandShift);
  }
 
 
  if (!useLP) {
         acDetScale += autoCorr2nd_cplx(&ac, lowBandReal+LPC_ORDER, lowBandImag+LPC_ORDER, autoCorrLength);
  }
  else
  {
         acDetScale += autoCorr2nd_real(&ac, lowBandReal+LPC_ORDER, autoCorrLength);
  }
 
   
       acDetScale += 2*(comLowBandScale + dynamicScale);
       acDetScale *= 2;  
       acDetScale += ac.det_scale;  
 
   
  if (acDetScale>126 ) {
         resetLPCCoeffs = 1;
  }
 
 
     alphar[1] = FL2FXCONST_SGL(0.0f);
  if (!useLP)
       alphai[1] = FL2FXCONST_SGL(0.0f);
 
  if (ac.det != FL2FXCONST_DBL(0.0f)) {
       FIXP_DBL tmp,absTmp,absDet;
 
       absDet = fixp_abs(ac.det);
 
  if (!useLP) {
         tmp = ( fMultDiv2(ac.r01r,ac.r12r) >> (LPC_SCALE_FACTOR-1) ) -
  ( (fMultDiv2(ac.r01i,ac.r12i) + fMultDiv2(ac.r02r,ac.r11r)) >> (LPC_SCALE_FACTOR-1) );
  } else
  {
         tmp = ( fMultDiv2(ac.r01r,ac.r12r) >> (LPC_SCALE_FACTOR-1) ) -
  ( fMultDiv2(ac.r02r,ac.r11r) >> (LPC_SCALE_FACTOR-1) );
  }
       absTmp = fixp_abs(tmp);
 
   
  {
         INT scale;
         FIXP_DBL result = fDivNorm(absTmp, absDet, &scale);
         scale = scale+ac.det_scale;
 
  if ( (scale > 0) && (result >= (FIXP_DBL)MAXVAL_DBL>>scale) ) {
           resetLPCCoeffs = 1;
  }
  else {
           alphar[1] = FX_DBL2FX_SGL(scaleValue(result,scale));
  if((tmp<FL2FX_DBL(0.0f)) ^ (ac.det<FL2FX_DBL(0.0f))) {
             alphar[1] = -alphar[1];
  }
  }
  }
 
  if (!useLP)
  {
         tmp = ( fMultDiv2(ac.r01i,ac.r12r) >> (LPC_SCALE_FACTOR-1) ) +
  ( (fMultDiv2(ac.r01r,ac.r12i) - (FIXP_DBL)fMultDiv2(ac.r02i,ac.r11r)) >> (LPC_SCALE_FACTOR-1) ) ;
 
         absTmp = fixp_abs(tmp);
 
   
  {
           INT scale;
           FIXP_DBL result = fDivNorm(absTmp, absDet, &scale);
           scale = scale+ac.det_scale;
 
  if ( (scale > 0) && (result >=   (FIXP_DBL)MAXVAL_DBL>>scale) ) {
             resetLPCCoeffs = 1;
  }
  else {
             alphai[1] = FX_DBL2FX_SGL(scaleValue(result,scale));
  if((tmp<FL2FX_DBL(0.0f)) ^ (ac.det<FL2FX_DBL(0.0f))) {
               alphai[1] = -alphai[1];
  }
  }
  }
  }
  }
 
     alphar[0] =  FL2FXCONST_SGL(0.0f);
  if (!useLP)
       alphai[0] = FL2FXCONST_SGL(0.0f);
 
  if ( ac.r11r != FL2FXCONST_DBL(0.0f) ) {
 
   
       FIXP_DBL tmp,absTmp;
 
  if (!useLP) {
         tmp = (ac.r01r>>(LPC_SCALE_FACTOR+1)) +
  (fMultDiv2(alphar[1],ac.r12r) + fMultDiv2(alphai[1],ac.r12i));
  } else
  {
  if(ac.r01r>=FL2FXCONST_DBL(0.0f))
           tmp = (ac.r01r>>(LPC_SCALE_FACTOR+1)) + fMultDiv2(alphar[1],ac.r12r);
  else
           tmp = -((-ac.r01r)>>(LPC_SCALE_FACTOR+1)) + fMultDiv2(alphar[1],ac.r12r);
  }
 
       absTmp = fixp_abs(tmp);
 
   
 
  if (absTmp >= (ac.r11r>>1)) {
         resetLPCCoeffs=1;
  }
  else {
         INT scale;
         FIXP_DBL result = fDivNorm(absTmp, fixp_abs(ac.r11r), &scale);
         alphar[0] =  FX_DBL2FX_SGL(scaleValue(result,scale+1));
 
  if((tmp>FL2FX_DBL(0.0f)) ^ (ac.r11r<FL2FX_DBL(0.0f)))
           alphar[0] = -alphar[0];
  }
 
  if (!useLP)
  {
         tmp = (ac.r01i>>(LPC_SCALE_FACTOR+1)) +
  (fMultDiv2(alphai[1],ac.r12r) - fMultDiv2(alphar[1],ac.r12i));
 
         absTmp = fixp_abs(tmp);
 
   
  if (absTmp >= (ac.r11r>>1)) {
           resetLPCCoeffs=1;
  }
  else {
           INT scale;
           FIXP_DBL result = fDivNorm(absTmp, fixp_abs(ac.r11r), &scale);
           alphai[0] = FX_DBL2FX_SGL(scaleValue(result,scale+1));
  if((tmp>FL2FX_DBL(0.0f)) ^ (ac.r11r<FL2FX_DBL(0.0f)))
             alphai[0] = -alphai[0];
  }
  }
  }
 
 
  if (!useLP)
  {
   
  if( (fMultDiv2(alphar[0],alphar[0]) + fMultDiv2(alphai[0],alphai[0])) >= FL2FXCONST_DBL(0.5f) )
         resetLPCCoeffs=1;
  if( (fMultDiv2(alphar[1],alphar[1]) + fMultDiv2(alphai[1],alphai[1])) >= FL2FXCONST_DBL(0.5f) )
         resetLPCCoeffs=1;
  }
 
  if(resetLPCCoeffs){
       alphar[0] = FL2FXCONST_SGL(0.0f);
       alphar[1] = FL2FXCONST_SGL(0.0f);
  if (!useLP)
  {
         alphai[0] = FL2FXCONST_SGL(0.0f);
         alphai[1] = FL2FXCONST_SGL(0.0f);
  }
  }
 
  if (useLP)
  {
 
   
  if(ac.r11r==FL2FXCONST_DBL(0.0f)) {
         k1 = FL2FXCONST_DBL(0.0f);
  }
  else {
  if ( fixp_abs(ac.r01r) >= fixp_abs(ac.r11r) ) {
  if ( fMultDiv2(ac.r01r,ac.r11r) < FL2FX_DBL(0.0f)) {
             k1 = (FIXP_DBL)MAXVAL_DBL  ;
  }else {
   
             k1 = (FIXP_DBL)(MINVAL_DBL+1)  ;
  }
  }
  else {
           INT scale;
           FIXP_DBL result = fDivNorm(fixp_abs(ac.r01r), fixp_abs(ac.r11r), &scale);
           k1 = scaleValue(result,scale);
 
  if(!((ac.r01r<FL2FX_DBL(0.0f)) ^ (ac.r11r<FL2FX_DBL(0.0f)))) {
             k1 = -k1;
  }
  }
  }
  if(loBand > 1){
   
         FIXP_DBL deg =   (FIXP_DBL)MAXVAL_DBL - fPow2(k1_below);
         degreeAlias[loBand] = FL2FXCONST_DBL(0.0f);
  if (((loBand & 1) == 0) && (k1 < FL2FXCONST_DBL(0.0f))){
  if (k1_below < FL2FXCONST_DBL(0.0f)) {  
             degreeAlias[loBand] = (FIXP_DBL)MAXVAL_DBL  ;
  if ( k1_below2 > FL2FXCONST_DBL(0.0f) ) {  
               degreeAlias[loBand-1] = deg;
  }
  }
  else if ( k1_below2 > FL2FXCONST_DBL(0.0f) ) {  
             degreeAlias[loBand] = deg;
  }
  }
  if (((loBand & 1) == 1) && (k1 > FL2FXCONST_DBL(0.0f))){
  if (k1_below > FL2FXCONST_DBL(0.0f)) {  
             degreeAlias[loBand] = (FIXP_DBL)MAXVAL_DBL  ;
  if ( k1_below2 < FL2FXCONST_DBL(0.0f) ) {  
               degreeAlias[loBand-1] = deg;
  }
  }
  else if ( k1_below2 < FL2FXCONST_DBL(0.0f) ) {  
             degreeAlias[loBand] = deg;
  }
  }
  }
   
       k1_below2 = k1_below;
       k1_below = k1;
  }
 
     patch = 0;
 
  while ( patch < pSettings->noOfPatches ) {  
 
  int hiBand = loBand + patchParam[patch].targetBandOffs;
 
  if ( loBand < patchParam[patch].sourceStartBand
  || loBand >= patchParam[patch].sourceStopBand
  ) {
   
         patch++;
  continue;
  }
 
 
        FDK_ASSERT( hiBand < (64) );
  
         
      while (hiBand >= pSettings->bwBorders[bwIndex[patch]])
//       while (hiBand >= pSettings->bwBorders[bwIndex[patch]] && bwIndex[patch] < MAX_NUM_PATCHES-1) {
          bwIndex[patch]++;
//       }
  
         
       bw = FX_DBL2FX_SGL(bwVector[bwIndex[patch]]);
 
       a0r = FX_DBL2FX_SGL(fMult(bw,alphar[0]));  
 
 
  if (!useLP)
         a0i = FX_DBL2FX_SGL(fMult(bw,alphai[0]));
       bw =  FX_DBL2FX_SGL(fPow2(bw));
       a1r = FX_DBL2FX_SGL(fMult(bw,alphar[1]));
  if (!useLP)
         a1i = FX_DBL2FX_SGL(fMult(bw,alphai[1]));
 
 
 
   
 
  if ( bw <= FL2FXCONST_SGL(0.0f) ) {
  if (!useLP) {
  int descale = fixMin(DFRACT_BITS-1, (LPC_SCALE_FACTOR+dynamicScale));
  for(i = startSample; i < stopSample; i++ ) {
             qmfBufferReal[i][hiBand] = lowBandReal[LPC_ORDER+i]>>descale;
             qmfBufferImag[i][hiBand] = lowBandImag[LPC_ORDER+i]>>descale;
  }
  } else
  {
  int descale = fixMin(DFRACT_BITS-1, (LPC_SCALE_FACTOR+dynamicScale));
  for(i = startSample; i < stopSample; i++ ) {
             qmfBufferReal[i][hiBand] = lowBandReal[LPC_ORDER+i]>>descale;
  }
  }
  }
  else {  
 
  if (!useLP) {
  int descale = fixMin(DFRACT_BITS-1, (LPC_SCALE_FACTOR+dynamicScale));
 #ifdef FUNCTION_LPPTRANSPOSER_func1
           lppTransposer_func1(lowBandReal+LPC_ORDER+startSample,lowBandImag+LPC_ORDER+startSample,
                               qmfBufferReal+startSample,qmfBufferImag+startSample,
                               stopSample-startSample, (int) hiBand,
                               dynamicScale,descale,
                               a0r, a0i, a1r, a1i);
 #else
  for(i = startSample; i < stopSample; i++ ) {
             FIXP_DBL accu1, accu2;
 
             accu1 = (fMultDiv2(a0r,lowBandReal[LPC_ORDER+i-1]) - fMultDiv2(a0i,lowBandImag[LPC_ORDER+i-1]) +
                      fMultDiv2(a1r,lowBandReal[LPC_ORDER+i-2]) - fMultDiv2(a1i,lowBandImag[LPC_ORDER+i-2]))>>dynamicScale;
             accu2 = (fMultDiv2(a0i,lowBandReal[LPC_ORDER+i-1]) + fMultDiv2(a0r,lowBandImag[LPC_ORDER+i-1]) +
                      fMultDiv2(a1i,lowBandReal[LPC_ORDER+i-2]) + fMultDiv2(a1r,lowBandImag[LPC_ORDER+i-2]))>>dynamicScale;
 
             qmfBufferReal[i][hiBand] = (lowBandReal[LPC_ORDER+i]>>descale) + (accu1<<1);
             qmfBufferImag[i][hiBand] = (lowBandImag[LPC_ORDER+i]>>descale) + (accu2<<1);
  }
 #endif
  } else
  {
  int descale = fixMin(DFRACT_BITS-1, (LPC_SCALE_FACTOR+dynamicScale));
 
           FDK_ASSERT(dynamicScale >= 0);
  for(i = startSample; i < stopSample; i++ ) {
             FIXP_DBL accu1;
 
             accu1 = (fMultDiv2(a0r,lowBandReal[LPC_ORDER+i-1]) + fMultDiv2(a1r,lowBandReal[LPC_ORDER+i-2]))>>dynamicScale;
 
             qmfBufferReal[i][hiBand] = (lowBandReal[LPC_ORDER+i]>>descale) + (accu1<<1);
  }
  }
  }  
 
       patch++;
 
  }  
 
   
 
 
  }  
 
  if (useLP)
  {
  for ( loBand = pSettings->lbStartPatching; loBand <  pSettings->lbStopPatching; loBand++ ) {
       patch = 0;
  while ( patch < pSettings->noOfPatches ) {
 
         UCHAR hiBand = loBand + patchParam[patch].targetBandOffs;
 
  if ( loBand < patchParam[patch].sourceStartBand
  || loBand >= patchParam[patch].sourceStopBand
  || hiBand >= (64)  
  ) {
   
           patch++;
  continue;
  }
 
  if(hiBand != patchParam[patch].targetStartBand)
           degreeAlias[hiBand] = degreeAlias[loBand];
 
         patch++;
  }
  } 
  }
 
  for (i = 0; i < nInvfBands; i++ ) {
    hLppTrans->bwVectorOld[i] = bwVector[i];
  }
 
   
   sbrScaleFactor->hb_scale = comLowBandScale-(LPC_SCALE_FACTOR);
 
 }