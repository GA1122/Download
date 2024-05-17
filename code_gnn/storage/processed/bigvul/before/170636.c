void AgcDisable(preproc_effect_t *effect)
{
    ALOGV("AgcDisable");
    webrtc::GainControl *agc = static_cast<webrtc::GainControl *>(effect->engine);
    agc->Enable(false);
}
