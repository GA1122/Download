void AecDisable(preproc_effect_t *effect)
{
    ALOGV("AecDisable");
    webrtc::EchoControlMobile *aec = static_cast<webrtc::EchoControlMobile *>(effect->engine);
    aec->Enable(false);
}
