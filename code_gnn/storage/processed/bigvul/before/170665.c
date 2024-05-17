int Session_SetReverseConfig(preproc_session_t *session, effect_config_t *config)
{
 if (config->inputCfg.samplingRate != config->outputCfg.samplingRate ||
            config->inputCfg.format != config->outputCfg.format ||
            config->inputCfg.format != AUDIO_FORMAT_PCM_16_BIT) {
 return -EINVAL;
 }

    ALOGV("Session_SetReverseConfig sr %d cnl %08x",
         config->inputCfg.samplingRate, config->inputCfg.channels);

 if (session->state < PREPROC_SESSION_STATE_CONFIG) {
 return -ENOSYS;
 }
 if (config->inputCfg.samplingRate != session->samplingRate ||
            config->inputCfg.format != AUDIO_FORMAT_PCM_16_BIT) {
 return -EINVAL;
 }
 uint32_t inCnl = audio_channel_count_from_out_mask(config->inputCfg.channels);
 int status = session->apm->set_num_reverse_channels(inCnl);
 if (status < 0) {
 return -EINVAL;
 }
    session->revChannelCount = inCnl;
    session->revFrame->_audioChannel = inCnl;
    session->revFrame->_frequencyInHz = session->apmSamplingRate;
    session->revBufSize = 0;
    session->framesRev = 0;

 return 0;
}
