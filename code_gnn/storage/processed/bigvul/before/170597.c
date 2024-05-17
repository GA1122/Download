int Volume_setParameter (EffectContext *pContext, void *pParam, void *pValue){
 int      status = 0;
 int16_t  level;
 int16_t  position;
 uint32_t mute;
 uint32_t positionEnabled;
 int32_t *pParamTemp = (int32_t *)pParam;
 int32_t param = *pParamTemp++;


 switch (param){
 case VOLUME_PARAM_LEVEL:
            level = *(int16_t *)pValue;
            status = VolumeSetVolumeLevel(pContext, (int16_t)level);
 break;

 case VOLUME_PARAM_MUTE:
            mute = *(uint32_t *)pValue;
            status = VolumeSetMute(pContext, mute);
 break;

 case VOLUME_PARAM_ENABLESTEREOPOSITION:
            positionEnabled = *(uint32_t *)pValue;
            status = VolumeEnableStereoPosition(pContext, positionEnabled);
            status = VolumeSetStereoPosition(pContext, pContext->pBundledContext->positionSaved);
 break;

 case VOLUME_PARAM_STEREOPOSITION:
            position = *(int16_t *)pValue;
            status = VolumeSetStereoPosition(pContext, (int16_t)position);
 break;

 default:
            ALOGV("\tLVM_ERROR : Volume_setParameter() invalid param %d", param);
 break;
 }

 return status;
}  
