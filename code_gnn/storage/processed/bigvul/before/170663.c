int Session_SetConfig(preproc_session_t *session, effect_config_t *config)
{
 uint32_t sr;
 uint32_t inCnl = audio_channel_count_from_out_mask(config->inputCfg.channels);
 uint32_t outCnl = audio_channel_count_from_out_mask(config->outputCfg.channels);

 if (config->inputCfg.samplingRate != config->outputCfg.samplingRate ||
        config->inputCfg.format != config->outputCfg.format ||
        config->inputCfg.format != AUDIO_FORMAT_PCM_16_BIT) {
 return -EINVAL;
 }

    ALOGV("Session_SetConfig sr %d cnl %08x",
         config->inputCfg.samplingRate, config->inputCfg.channels);
 int status;

 if (session->enabledMsk) {
 if (session->samplingRate != config->inputCfg.samplingRate ||
                session->inChannelCount != inCnl ||
                session->outChannelCount != outCnl) {
 return -ENOSYS;
 } else {
 return 0;
 }
 }

 if (config->inputCfg.samplingRate >= 32000 && !(session->createdMsk & (1 << PREPROC_AEC))) {
        session->apmSamplingRate = 32000;
 } else
 if (config->inputCfg.samplingRate >= 16000) {
        session->apmSamplingRate = 16000;
 } else if (config->inputCfg.samplingRate >= 8000) {
        session->apmSamplingRate = 8000;
 }
    status = session->apm->set_sample_rate_hz(session->apmSamplingRate);
 if (status < 0) {
 return -EINVAL;
 }
    status = session->apm->set_num_channels(inCnl, outCnl);
 if (status < 0) {
 return -EINVAL;
 }
    status = session->apm->set_num_reverse_channels(inCnl);
 if (status < 0) {
 return -EINVAL;
 }

    session->samplingRate = config->inputCfg.samplingRate;
    session->apmFrameCount = session->apmSamplingRate / 100;
 if (session->samplingRate == session->apmSamplingRate) {
        session->frameCount = session->apmFrameCount;
 } else {
        session->frameCount = (session->apmFrameCount * session->samplingRate) /
                session->apmSamplingRate  + 1;
 }
    session->inChannelCount = inCnl;
    session->outChannelCount = outCnl;
    session->procFrame->_audioChannel = inCnl;
    session->procFrame->_frequencyInHz = session->apmSamplingRate;

    session->revChannelCount = inCnl;
    session->revFrame->_audioChannel = inCnl;
    session->revFrame->_frequencyInHz = session->apmSamplingRate;

    session->inBufSize = 0;
    session->outBufSize = 0;
    session->framesIn = 0;
    session->framesOut = 0;


 if (session->inResampler != NULL) {
        speex_resampler_destroy(session->inResampler);
        session->inResampler = NULL;
 }
 if (session->outResampler != NULL) {
        speex_resampler_destroy(session->outResampler);
        session->outResampler = NULL;
 }
 if (session->revResampler != NULL) {
        speex_resampler_destroy(session->revResampler);
        session->revResampler = NULL;
 }
 if (session->samplingRate != session->apmSamplingRate) {
 int error;
        session->inResampler = speex_resampler_init(session->inChannelCount,
                                                    session->samplingRate,
                                                    session->apmSamplingRate,
                                                    RESAMPLER_QUALITY,
 &error);
 if (session->inResampler == NULL) {
            ALOGW("Session_SetConfig Cannot create speex resampler: %s",
                 speex_resampler_strerror(error));
 return -EINVAL;
 }
        session->outResampler = speex_resampler_init(session->outChannelCount,
                                                    session->apmSamplingRate,
                                                    session->samplingRate,
                                                    RESAMPLER_QUALITY,
 &error);
 if (session->outResampler == NULL) {
            ALOGW("Session_SetConfig Cannot create speex resampler: %s",
                 speex_resampler_strerror(error));
            speex_resampler_destroy(session->inResampler);
            session->inResampler = NULL;
 return -EINVAL;
 }
        session->revResampler = speex_resampler_init(session->inChannelCount,
                                                    session->samplingRate,
                                                    session->apmSamplingRate,
                                                    RESAMPLER_QUALITY,
 &error);
 if (session->revResampler == NULL) {
            ALOGW("Session_SetConfig Cannot create speex resampler: %s",
                 speex_resampler_strerror(error));
            speex_resampler_destroy(session->inResampler);
            session->inResampler = NULL;
            speex_resampler_destroy(session->outResampler);
            session->outResampler = NULL;
 return -EINVAL;
 }
 }

    session->state = PREPROC_SESSION_STATE_CONFIG;
 return 0;
}
