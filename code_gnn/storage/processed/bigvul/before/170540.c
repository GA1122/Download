int Downmix_setParameter(downmix_object_t *pDownmixer, int32_t param, uint32_t size, void *pValue) {

 int16_t value16;
    ALOGV("Downmix_setParameter, context %p, param %" PRId32 ", value16 %" PRId16 ", value32 %" PRId32,
            pDownmixer, param, *(int16_t *)pValue, *(int32_t *)pValue);

 switch (param) {

 case DOWNMIX_PARAM_TYPE:
 if (size != sizeof(downmix_type_t)) {
            ALOGE("Downmix_setParameter(DOWNMIX_PARAM_TYPE) invalid size %" PRIu32 ", should be %zu",
                    size, sizeof(downmix_type_t));
 return -EINVAL;
 }
        value16 = *(int16_t *)pValue;
        ALOGV("set DOWNMIX_PARAM_TYPE, type %" PRId16, value16);
 if (!((value16 > DOWNMIX_TYPE_INVALID) && (value16 <= DOWNMIX_TYPE_LAST))) {
            ALOGE("Downmix_setParameter invalid DOWNMIX_PARAM_TYPE value %" PRId16, value16);
 return -EINVAL;
 } else {
            pDownmixer->type = (downmix_type_t) value16;
 break;

 default:
        ALOGE("Downmix_setParameter unknown parameter %" PRId32, param);
 return -EINVAL;
 }
}

 return 0;
}  
