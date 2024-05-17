int PreProcessingLib_Release(effect_handle_t interface)
{
 int status;
    ALOGV("EffectRelease start %p", interface);
 if (PreProc_Init() != 0) {
 return sInitStatus;
 }

 preproc_effect_t *fx = (preproc_effect_t *)interface;

 if (fx->session->io == 0) {
 return -EINVAL;
 }
 return Session_ReleaseEffect(fx->session, fx);
}
