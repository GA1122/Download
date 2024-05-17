int AgcInit (preproc_effect_t *effect)
{
    ALOGV("AgcInit");
    webrtc::GainControl *agc = static_cast<webrtc::GainControl *>(effect->engine);
    agc->set_mode(webrtc::GainControl::kFixedDigital);
    agc->set_target_level_dbfs(kAgcDefaultTargetLevel);
    agc->set_compression_gain_db(kAgcDefaultCompGain);
    agc->enable_limiter(kAgcDefaultLimiter);
 return 0;
}
