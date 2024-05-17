int AecSetParameter (preproc_effect_t *effect, void *pParam, void *pValue)
{
 int status = 0;
 uint32_t param = *(uint32_t *)pParam;
 uint32_t value = *(uint32_t *)pValue;

 switch (param) {
 case AEC_PARAM_ECHO_DELAY:
 case AEC_PARAM_PROPERTIES:
        status = effect->session->apm->set_stream_delay_ms(value/1000);
        ALOGV("AecSetParameter() echo delay %d us, status %d", value, status);
 break;
 default:
        ALOGW("AecSetParameter() unknown param %08x value %08x", param, *(uint32_t *)pValue);
        status = -EINVAL;
 break;
 }
 return status;
}
