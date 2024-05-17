int NsInit (preproc_effect_t *effect)
{
    ALOGV("NsInit");
    webrtc::NoiseSuppression *ns = static_cast<webrtc::NoiseSuppression *>(effect->engine);
    ns->set_level(kNsDefaultLevel);
 return 0;
}
