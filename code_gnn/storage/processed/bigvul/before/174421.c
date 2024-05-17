void aacDecoder_drcApply (
        HANDLE_AAC_DRC          self,
 void *pSbrDec,
 CAacDecoderChannelInfo *pAacDecoderChannelInfo,
 CDrcChannelData *pDrcChData,
        FIXP_DBL               *extGain,
 int  ch,  
 int  aacFrameSize,
 int  bSbrPresent )
{
 int band, top, bin, numBands;
 int bottom = 0;
 int modifyBins = 0;

  FIXP_DBL max_mantissa;
  INT max_exponent;

  FIXP_DBL norm_mantissa = FL2FXCONST_DBL(0.5f);
  INT  norm_exponent = 1;

  FIXP_DBL fact_mantissa[MAX_DRC_BANDS];
  INT  fact_exponent[MAX_DRC_BANDS];

 CDrcParams *pParams = &self->params;

  FIXP_DBL    *pSpectralCoefficient  =  SPEC_LONG(pAacDecoderChannelInfo->pSpectralCoefficient);
 CIcsInfo *pIcsInfo              = &pAacDecoderChannelInfo->icsInfo;
  SHORT       *pSpecScale            =  pAacDecoderChannelInfo->specScale;

 int winSeq = pIcsInfo->WindowSequence;

  
 if ( (pParams->expiryFrame > 0)
 && (++pDrcChData->expiryCount > pParams->expiryFrame) )
 {  
    aacDecoder_drcInitChannelData( pDrcChData );
 }

 if (!self->enable) {
    sbrDecoder_drcDisable( (HANDLE_SBRDECODER)pSbrDec, ch );
 if (extGain != NULL) {
      INT gainScale = (INT)*extGain;
  
 if (gainScale >= 0 && gainScale <= DFRACT_BITS) {
 *extGain = scaleValue(norm_mantissa, norm_exponent-gainScale);
 } else {
        FDK_ASSERT(0);
 }
 }
 return;
 }

  numBands = pDrcChData->numBands;
  top = FDKmax(0, numBands-1);

  pDrcChData->bandTop[0] = fixMin(pDrcChData->bandTop[0], (aacFrameSize >> 2) - 1);

  

 if (pParams->targetRefLevel >= 0)
 {
  
    norm_mantissa = fLdPow(
            FL2FXCONST_DBL(-1.0),  
 0,
 (FIXP_DBL)((INT)(FL2FXCONST_DBL(1.0f/24.0)>>3) * (INT)(pParams->targetRefLevel-self->progRefLevel)),
 3,
 &norm_exponent );
 }
  
 if (extGain != NULL) {
    INT gainScale = (INT)*extGain;
  
 if (gainScale >= 0 && gainScale <= DFRACT_BITS) {
 *extGain = scaleValue(norm_mantissa, norm_exponent-gainScale);
 } else {
      FDK_ASSERT(0);
 }
 }
 if (self->params.applyDigitalNorm == 0) {
  
    norm_mantissa = FL2FXCONST_DBL(0.5f);
    norm_exponent = 1;
 }


  
 for (band = 0; band < numBands; band++)
 {
    UCHAR drcVal = pDrcChData->drcValue[band];
    top = fixMin((int)( (pDrcChData->bandTop[band]+1)<<2 ), aacFrameSize);

    fact_mantissa[band] = FL2FXCONST_DBL(0.5f);
    fact_exponent[band] = 1;

 if (  pParams->applyHeavyCompression
 && ((AACDEC_DRC_PAYLOAD_TYPE)pDrcChData->drcDataType == DVB_DRC_ANC_DATA) )
 {
      INT compressionFactorVal_e;
 int valX, valY;

      valX = drcVal >> 4;
      valY = drcVal & 0x0F;

  
 if ( drcVal != 0x7F ) {
        fact_mantissa[band] =
          fPowInt( FL2FXCONST_DBL(0.95483867181),  
 0,
                   valY,
 &compressionFactorVal_e );

  
        fact_mantissa[band] = fMult(FL2FXCONST_DBL(0.99990790084), fact_mantissa[band]);

        fact_exponent[band] = DVB_COMPRESSION_SCALE - valX + compressionFactorVal_e;
 }
 } else
 if ((AACDEC_DRC_PAYLOAD_TYPE)pDrcChData->drcDataType == MPEG_DRC_EXT_DATA)
 {
  

 if ((drcVal&0x7F) > 0) {
      FIXP_DBL tParamVal = (drcVal & 0x80) ? -pParams->cut : pParams->boost;

      fact_mantissa[band] =
        f2Pow( (FIXP_DBL)((INT)fMult(FL2FXCONST_DBL(1.0f/192.0f), tParamVal) * (drcVal&0x7F)),
 3+DRC_PARAM_SCALE,
 &fact_exponent[band] );
 }
 }

    fact_mantissa[band] = fMult(fact_mantissa[band], norm_mantissa);
    fact_exponent[band] += norm_exponent;


    bottom = top;

 }  


  
 {
 int res;

    max_mantissa = FL2FXCONST_DBL(0.0f);
    max_exponent = 0;
 for (band = 0; band < numBands; band++) {
      max_mantissa = fixMax(max_mantissa, fact_mantissa[band]);
      max_exponent = fixMax(max_exponent, fact_exponent[band]);
 }

  
    res = CntLeadingZeros(max_mantissa) - 1;

  
 if (((pDrcChData->bandTop[numBands-1]+1)<<2) < aacFrameSize) res = 0;

 if (res > 0) {
      res = fixMin(res, max_exponent);
      max_exponent -= res;

 for (band = 0; band < numBands; band++) {
        fact_mantissa[band] <<= res;
        fact_exponent[band] -= res;
 }
 }

  
 for (band = 0; band < numBands; band++) {
 if (fact_exponent[band] < max_exponent) {
        fact_mantissa[band] >>= max_exponent - fact_exponent[band];
 }
 if (fact_mantissa[band] != FL2FXCONST_DBL(0.5f)) {
        modifyBins = 1;
 }
 }
 if (max_exponent != 1) {
      modifyBins = 1;
 }
 }

  
 if (!bSbrPresent)
 {
    bottom = 0;

 if (!modifyBins) {
  
      max_exponent -= 1;
 } else
 {
 for (band = 0; band < numBands; band++)
 {
        top = fixMin((int)( (pDrcChData->bandTop[band]+1)<<2 ), aacFrameSize);  

 for (bin = bottom; bin < top; bin++) {
          pSpectralCoefficient[bin] = fMult(pSpectralCoefficient[bin], fact_mantissa[band]);
 }

        bottom = top;
 }
 }

  
 if (max_exponent > 0) {
 for (bin = bottom; bin < aacFrameSize; bin+=1) {
        pSpectralCoefficient[bin] >>= max_exponent;
 }
 }

  
    pSpecScale[0] += max_exponent;

 if (winSeq == EightShortSequence) {
 int win;
 for (win = 1; win < 8; win++) {
        pSpecScale[win] += max_exponent;
 }
 }
 }
 else {
    HANDLE_SBRDECODER hSbrDecoder = (HANDLE_SBRDECODER)pSbrDec;
    UINT numBands = pDrcChData->numBands;

  
    sbrDecoder_drcFeedChannel (
            hSbrDecoder,
            ch,
            numBands,
            fact_mantissa,
            max_exponent,
            pDrcChData->drcInterpolationScheme,
            winSeq,
            pDrcChData->bandTop
 );
 }

 return;
}
