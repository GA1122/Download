int AecGetParameter(preproc_effect_t *effect,
 void *pParam,
 uint32_t *pValueSize,
 void *pValue)
{
 int status = 0;
 uint32_t param = *(uint32_t *)pParam;

 if (*pValueSize < sizeof(uint32_t)) {
 return -EINVAL;
 }
 switch (param) {
 case AEC_PARAM_ECHO_DELAY:
 case AEC_PARAM_PROPERTIES:
 *(uint32_t *)pValue = 1000 * effect->session->apm->stream_delay_ms();
        ALOGV("AecGetParameter() echo delay %d us", *(uint32_t *)pValue);
 break;
 default:
        ALOGW("AecGetParameter() unknown param %08x value %08x", param, *(uint32_t *)pValue);
        status = -EINVAL;
 break;
 }
 return status;
}
