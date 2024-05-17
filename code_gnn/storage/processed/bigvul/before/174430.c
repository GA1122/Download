AAC_DECODER_ERROR aacDecoder_drcSetParam (
    HANDLE_AAC_DRC    self,
    AACDEC_DRC_PARAM  param,
    INT               value )
{
  AAC_DECODER_ERROR ErrorStatus = AAC_DEC_OK;

 switch (param)
 {
 case DRC_CUT_SCALE:
  
 if ( (value < 0)
 || (value > DRC_MAX_QUANT_FACTOR) ) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
    self->params.usrCut = (FIXP_DBL)((INT)(DRC_PARAM_QUANT_STEP>>DRC_PARAM_SCALE) * (INT)value);
 if (self->params.applyHeavyCompression == 0)
      self->params.cut = self->params.usrCut;
 break;
 case DRC_BOOST_SCALE:
  
 if ( (value < 0)
 || (value > DRC_MAX_QUANT_FACTOR) ) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
    self->params.usrBoost = (FIXP_DBL)((INT)(DRC_PARAM_QUANT_STEP>>DRC_PARAM_SCALE) * (INT)value);
 if (self->params.applyHeavyCompression == 0)
      self->params.boost = self->params.usrBoost;
 break;
 case TARGET_REF_LEVEL:
 if ( value >  MAX_REFERENCE_LEVEL
 || value < -MAX_REFERENCE_LEVEL ) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
 if (value < 0) {
      self->params.applyDigitalNorm = 0;
      self->params.targetRefLevel = -1;
 }
 else {
  
      self->params.applyDigitalNorm = 1;
 if (self->params.targetRefLevel != (SCHAR)value) {
        self->params.targetRefLevel = (SCHAR)value;
        self->progRefLevel = (SCHAR)value;  
 }
 }
 break;
 case APPLY_NORMALIZATION:
 if (value < 0 || value > 1) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
  
    self->params.applyDigitalNorm = (UCHAR)value;
 break;
 case APPLY_HEAVY_COMPRESSION:
 if (value < 0 || value > 1) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
 if (self->params.applyHeavyCompression != (UCHAR)value) {
 if (value == 1) {
  
        self->params.boost = FL2FXCONST_DBL(1.0f/(float)(1<<DRC_PARAM_SCALE));
        self->params.cut   = FL2FXCONST_DBL(1.0f/(float)(1<<DRC_PARAM_SCALE));
 } else {
  
        self->params.boost = self->params.usrBoost;
        self->params.cut   = self->params.usrCut;
 }
  
      self->params.applyHeavyCompression = (UCHAR)value;
 }
 break;
 case DRC_BS_DELAY:
 if (value < 0 || value > 1) {
 return AAC_DEC_SET_PARAM_FAIL;
 }
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
    self->params.bsDelayEnable = value;
 break;
 case DRC_DATA_EXPIRY_FRAME:
 if (self == NULL) {
 return AAC_DEC_INVALID_HANDLE;
 }
    self->params.expiryFrame = (UINT)value;
 break;
 default:
 return AAC_DEC_SET_PARAM_FAIL;
 }  

  
  self->enable = ( (self->params.boost > (FIXP_DBL)0)
 || (self->params.cut   > (FIXP_DBL)0)
 || (self->params.applyHeavyCompression != 0)
 || (self->params.targetRefLevel >= 0) );


 return ErrorStatus;
}
