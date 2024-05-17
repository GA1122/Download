int32_t DownmixLib_Release(effect_handle_t handle) {
 downmix_module_t *pDwmModule = (downmix_module_t *)handle;

    ALOGV("DownmixLib_Release() %p", handle);
 if (handle == NULL) {
 return -EINVAL;
 }

    pDwmModule->context.state = DOWNMIX_STATE_UNINITIALIZED;

    free(pDwmModule);
 return 0;
}
