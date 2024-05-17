int AgcCreate(preproc_effect_t *effect)
{
    webrtc::GainControl *agc = effect->session->apm->gain_control();
    ALOGV("AgcCreate got agc %p", agc);
 if (agc == NULL) {
        ALOGW("AgcCreate Error");
 return -ENOMEM;
 }
    effect->engine = static_cast<preproc_fx_handle_t>(agc);
 AgcInit(effect);
 return 0;
}
