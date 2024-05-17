void AgcEnable(preproc_effect_t *effect)
{
    webrtc::GainControl *agc = static_cast<webrtc::GainControl *>(effect->engine);
    ALOGV("AgcEnable agc %p", agc);
    agc->Enable(true);
}
