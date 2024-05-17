int AgcGetParameter(preproc_effect_t *effect,
 void *pParam,
 uint32_t *pValueSize,
 void *pValue)
{
 int status = 0;
 uint32_t param = *(uint32_t *)pParam;
    t_agc_settings *pProperties = (t_agc_settings *)pValue;
    webrtc::GainControl *agc = static_cast<webrtc::GainControl *>(effect->engine);

 switch (param) {
 case AGC_PARAM_TARGET_LEVEL:
 case AGC_PARAM_COMP_GAIN:
 if (*pValueSize < sizeof(int16_t)) {
 *pValueSize = 0;
 return -EINVAL;
 }
 break;
 case AGC_PARAM_LIMITER_ENA:
 if (*pValueSize < sizeof(bool)) {
 *pValueSize = 0;
 return -EINVAL;
 }
 break;
 case AGC_PARAM_PROPERTIES:
 if (*pValueSize < sizeof(t_agc_settings)) {
 *pValueSize = 0;
 return -EINVAL;
 }
 break;

 default:
        ALOGW("AgcGetParameter() unknown param %08x", param);
        status = -EINVAL;
 break;
 }

 switch (param) {
 case AGC_PARAM_TARGET_LEVEL:
 *(int16_t *) pValue = (int16_t)(agc->target_level_dbfs() * -100);
        ALOGV("AgcGetParameter() target level %d milliBels", *(int16_t *) pValue);
 break;
 case AGC_PARAM_COMP_GAIN:
 *(int16_t *) pValue = (int16_t)(agc->compression_gain_db() * 100);
        ALOGV("AgcGetParameter() comp gain %d milliBels", *(int16_t *) pValue);
 break;
 case AGC_PARAM_LIMITER_ENA:
 *(bool *) pValue = (bool)agc->is_limiter_enabled();
        ALOGV("AgcGetParameter() limiter enabled %s",
 (*(int16_t *) pValue != 0) ? "true" : "false");
 break;
 case AGC_PARAM_PROPERTIES:
        pProperties->targetLevel = (int16_t)(agc->target_level_dbfs() * -100);
        pProperties->compGain = (int16_t)(agc->compression_gain_db() * 100);
        pProperties->limiterEnabled = (bool)agc->is_limiter_enabled();
 break;
 default:
        ALOGW("AgcGetParameter() unknown param %d", param);
        status = -EINVAL;
 break;
 }
 return status;
}
