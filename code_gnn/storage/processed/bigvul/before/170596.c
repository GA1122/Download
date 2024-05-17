int Volume_getParameter(EffectContext *pContext,
 void *pParam,
 uint32_t *pValueSize,
 void *pValue){
 int status = 0;
 int bMute = 0;
 int32_t *pParamTemp = (int32_t *)pParam;
 int32_t param = *pParamTemp++;;
 char *name;


 switch (param){
 case VOLUME_PARAM_LEVEL:
 case VOLUME_PARAM_MAXLEVEL:
 case VOLUME_PARAM_STEREOPOSITION:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Volume_getParameter() invalid pValueSize 1  %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;

 case VOLUME_PARAM_MUTE:
 case VOLUME_PARAM_ENABLESTEREOPOSITION:
 if (*pValueSize < sizeof(int32_t)){
                ALOGV("\tLVM_ERROR : Volume_getParameter() invalid pValueSize 2  %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int32_t);
 break;

 default:
            ALOGV("\tLVM_ERROR : Volume_getParameter unknown param %d", param);
 return -EINVAL;
 }

 switch (param){
 case VOLUME_PARAM_LEVEL:
            status = VolumeGetVolumeLevel(pContext, (int16_t *)(pValue));
 break;

 case VOLUME_PARAM_MAXLEVEL:
 *(int16_t *)pValue = 0;
 break;

 case VOLUME_PARAM_STEREOPOSITION:
 VolumeGetStereoPosition(pContext, (int16_t *)pValue);
 break;

 case VOLUME_PARAM_MUTE:
            status = VolumeGetMute(pContext, (uint32_t *)pValue);
            ALOGV("\tVolume_getParameter() VOLUME_PARAM_MUTE Value is %d",
 *(uint32_t *)pValue);
 break;

 case VOLUME_PARAM_ENABLESTEREOPOSITION:
 *(int32_t *)pValue = pContext->pBundledContext->bStereoPositionEnabled;
 break;

 default:
            ALOGV("\tLVM_ERROR : Volume_getParameter() invalid param %d", param);
            status = -EINVAL;
 break;
 }

 return status;
}  
