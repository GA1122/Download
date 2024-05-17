int Virtualizer_getParameter(EffectContext *pContext,
 void *pParam,
 uint32_t *pValueSize,
 void *pValue){
 int status = 0;
 int32_t *pParamTemp = (int32_t *)pParam;
 int32_t param = *pParamTemp++;
 char *name;


 switch (param){
 case VIRTUALIZER_PARAM_STRENGTH_SUPPORTED:
 if (*pValueSize != sizeof(uint32_t)){
                ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid pValueSize %d",*pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(uint32_t);
 break;
 case VIRTUALIZER_PARAM_STRENGTH:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid pValueSize2 %d",*pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case VIRTUALIZER_PARAM_VIRTUAL_SPEAKER_ANGLES:
 break;
 case VIRTUALIZER_PARAM_VIRTUALIZATION_MODE:
 if (*pValueSize != sizeof(uint32_t)){
                ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid pValueSize %d",*pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(uint32_t);
 break;
 default:
            ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid param %d", param);
 return -EINVAL;
 }

 switch (param){
 case VIRTUALIZER_PARAM_STRENGTH_SUPPORTED:
 *(uint32_t *)pValue = 1;

 break;

 case VIRTUALIZER_PARAM_STRENGTH:
 *(int16_t *)pValue = VirtualizerGetStrength(pContext);

 break;

 case VIRTUALIZER_PARAM_VIRTUAL_SPEAKER_ANGLES: {
 const audio_channel_mask_t channelMask = (audio_channel_mask_t) *pParamTemp++;
 const audio_devices_t deviceType = (audio_devices_t) *pParamTemp;
 uint32_t nbChannels = audio_channel_count_from_out_mask(channelMask);
 if (*pValueSize < 3 * nbChannels * sizeof(int32_t)){
                ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid pValueSize %d",*pValueSize);
 return -EINVAL;
 }
            status = VirtualizerIsConfigurationSupported(channelMask, deviceType);
 if (status == 0) {
                ALOGV("VIRTUALIZER_PARAM_VIRTUAL_SPEAKER_ANGLES supports mask=0x%x device=0x%x",
                        channelMask, deviceType);
 VirtualizerGetSpeakerAngles(channelMask, deviceType, (int32_t *)pValue);
 }
 }
 break;

 case VIRTUALIZER_PARAM_VIRTUALIZATION_MODE:
 *(uint32_t *)pValue  = (uint32_t) VirtualizerGetVirtualizationMode(pContext);
 break;

 default:
            ALOGV("\tLVM_ERROR : Virtualizer_getParameter() invalid param %d", param);
            status = -EINVAL;
 break;
 }

    ALOGV("\tVirtualizer_getParameter end returning status=%d", status);
 return status;
}  
