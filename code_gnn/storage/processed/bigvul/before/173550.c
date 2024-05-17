static int session_create_effect(struct session_s *session,
 int32_t id,
 effect_handle_t *interface)
{
 int status = -ENOMEM;

    ALOGV("session_create_effect() %s, created_msk %08x",
          id == AEC_ID ? "AEC" : id == NS_ID ? "NS" : "?", session->created_msk);

 if (session->created_msk == 0) {
        session->config.inputCfg.samplingRate = 16000;
        session->config.inputCfg.channels = AUDIO_CHANNEL_IN_MONO;
        session->config.inputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
        session->config.outputCfg.samplingRate = 16000;
        session->config.outputCfg.channels = AUDIO_CHANNEL_IN_MONO;
        session->config.outputCfg.format = AUDIO_FORMAT_PCM_16_BIT;
        session->enabled_msk = 0;
        session->processed_msk = 0;
 }
    status = effect_create(&session->effects[id], session, interface);
 if (status < 0)
 goto error;

    ALOGV("session_create_effect() OK");
    session->created_msk |= (1<<id);
 return status;

error:
 return status;
}
