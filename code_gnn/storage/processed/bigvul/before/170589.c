int Virtualizer_setParameter (EffectContext *pContext, void *pParam, void *pValue){
 int status = 0;
 int16_t strength;
 int32_t *pParamTemp = (int32_t *)pParam;
 int32_t param = *pParamTemp++;


 switch (param){
 case VIRTUALIZER_PARAM_STRENGTH:
            strength = *(int16_t *)pValue;
 VirtualizerSetStrength(pContext, (int32_t)strength);
 break;

 case VIRTUALIZER_PARAM_FORCE_VIRTUALIZATION_MODE: {
 const audio_devices_t deviceType = *(audio_devices_t *) pValue;
            status = VirtualizerForceVirtualizationMode(pContext, deviceType);
 }
 break;

 default:
            ALOGV("\tLVM_ERROR : Virtualizer_setParameter() invalid param %d", param);
 break;
 }

 return status;
}  
