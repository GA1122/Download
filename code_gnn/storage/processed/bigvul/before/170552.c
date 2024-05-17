int BassBoost_setParameter (EffectContext *pContext, void *pParam, void *pValue){
 int status = 0;
 int16_t strength;
 int32_t *pParamTemp = (int32_t *)pParam;


 switch (*pParamTemp){
 case BASSBOOST_PARAM_STRENGTH:
            strength = *(int16_t *)pValue;
 BassSetStrength(pContext, (int32_t)strength);
 break;
 default:
            ALOGV("\tLVM_ERROR : BassBoost_setParameter() invalid param %d", *pParamTemp);
 break;
 }

 return status;
}  
