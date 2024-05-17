int Reverb_getParameter(ReverbContext *pContext,
 void *pParam,
 size_t *pValueSize,
 void *pValue){
 int status = 0;
 int32_t *pParamTemp = (int32_t *)pParam;
 int32_t param = *pParamTemp++;
 char *name;
    t_reverb_settings *pProperties;

 if (pContext->preset) {
 if (param != REVERB_PARAM_PRESET || *pValueSize < sizeof(uint16_t)) {
 return -EINVAL;
 }

 *(uint16_t *)pValue = pContext->nextPreset;
        ALOGV("get REVERB_PARAM_PRESET, preset %d", pContext->nextPreset);
 return 0;
 }

 switch (param){
 case REVERB_PARAM_ROOM_LEVEL:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize1 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_ROOM_HF_LEVEL:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize12 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_DECAY_TIME:
 if (*pValueSize != sizeof(uint32_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize3 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(uint32_t);
 break;
 case REVERB_PARAM_DECAY_HF_RATIO:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize4 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_REFLECTIONS_LEVEL:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize5 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_REFLECTIONS_DELAY:
 if (*pValueSize != sizeof(uint32_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize6 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(uint32_t);
 break;
 case REVERB_PARAM_REVERB_LEVEL:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize7 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_REVERB_DELAY:
 if (*pValueSize != sizeof(uint32_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize8 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(uint32_t);
 break;
 case REVERB_PARAM_DIFFUSION:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize9 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_DENSITY:
 if (*pValueSize != sizeof(int16_t)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize10 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(int16_t);
 break;
 case REVERB_PARAM_PROPERTIES:
 if (*pValueSize != sizeof(t_reverb_settings)){
                ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid pValueSize11 %d", *pValueSize);
 return -EINVAL;
 }
 *pValueSize = sizeof(t_reverb_settings);
 break;

 default:
            ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid param %d", param);
 return -EINVAL;
 }

    pProperties = (t_reverb_settings *) pValue;

 switch (param){
 case REVERB_PARAM_PROPERTIES:
            pProperties->roomLevel = ReverbGetRoomLevel(pContext);
            pProperties->roomHFLevel = ReverbGetRoomHfLevel(pContext);
            pProperties->decayTime = ReverbGetDecayTime(pContext);
            pProperties->decayHFRatio = ReverbGetDecayHfRatio(pContext);
            pProperties->reflectionsLevel = 0;
            pProperties->reflectionsDelay = 0;
            pProperties->reverbDelay = 0;
            pProperties->reverbLevel = ReverbGetReverbLevel(pContext);
            pProperties->diffusion = ReverbGetDiffusion(pContext);
            pProperties->density = ReverbGetDensity(pContext);

            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is roomLevel        %d",
                pProperties->roomLevel);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is roomHFLevel      %d",
                pProperties->roomHFLevel);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is decayTime        %d",
                pProperties->decayTime);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is decayHFRatio     %d",
                pProperties->decayHFRatio);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is reflectionsLevel %d",
                pProperties->reflectionsLevel);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is reflectionsDelay %d",
                pProperties->reflectionsDelay);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is reverbDelay      %d",
                pProperties->reverbDelay);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is reverbLevel      %d",
                pProperties->reverbLevel);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is diffusion        %d",
                pProperties->diffusion);
            ALOGV("\tReverb_getParameter() REVERB_PARAM_PROPERTIES Value is density          %d",
                pProperties->density);
 break;

 case REVERB_PARAM_ROOM_LEVEL:
 *(int16_t *)pValue = ReverbGetRoomLevel(pContext);

 break;
 case REVERB_PARAM_ROOM_HF_LEVEL:
 *(int16_t *)pValue = ReverbGetRoomHfLevel(pContext);

 break;
 case REVERB_PARAM_DECAY_TIME:
 *(uint32_t *)pValue = ReverbGetDecayTime(pContext);

 break;
 case REVERB_PARAM_DECAY_HF_RATIO:
 *(int16_t *)pValue = ReverbGetDecayHfRatio(pContext);

 break;
 case REVERB_PARAM_REVERB_LEVEL:
 *(int16_t *)pValue = ReverbGetReverbLevel(pContext);

 break;
 case REVERB_PARAM_DIFFUSION:
 *(int16_t *)pValue = ReverbGetDiffusion(pContext);

 break;
 case REVERB_PARAM_DENSITY:
 *(uint16_t *)pValue = 0;
 *(int16_t *)pValue = ReverbGetDensity(pContext);
 break;
 case REVERB_PARAM_REFLECTIONS_LEVEL:
 *(uint16_t *)pValue = 0;
 case REVERB_PARAM_REFLECTIONS_DELAY:
 *(uint32_t *)pValue = 0;
 case REVERB_PARAM_REVERB_DELAY:
 *(uint32_t *)pValue = 0;
 break;

 default:
            ALOGV("\tLVM_ERROR : Reverb_getParameter() invalid param %d", param);
            status = -EINVAL;
 break;
 }

 return status;
}  
