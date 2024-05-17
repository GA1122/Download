int AecCreate(preproc_effect_t *effect)
{
    webrtc::EchoControlMobile *aec = effect->session->apm->echo_control_mobile();
    ALOGV("AecCreate got aec %p", aec);
 if (aec == NULL) {
        ALOGW("AgcCreate Error");
 return -ENOMEM;
 }
    effect->engine = static_cast<preproc_fx_handle_t>(aec);
 AecInit (effect);
 return 0;
}
