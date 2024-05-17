void NsEnable(preproc_effect_t *effect)
{
    webrtc::NoiseSuppression *ns = static_cast<webrtc::NoiseSuppression *>(effect->engine);
    ALOGV("NsEnable ns %p", ns);
    ns->Enable(true);
}
