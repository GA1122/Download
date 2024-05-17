extern "C" int Session_CreateEffect(preproc_session_t *session,
 int32_t procId,
 effect_handle_t *interface)
{
 int status = -ENOMEM;

    ALOGV("Session_CreateEffect procId %d, createdMsk %08x", procId, session->createdMsk);

 if (session->createdMsk == 0) {
        session->apm = webrtc::AudioProcessing::Create(session->io);
 if (session->apm == NULL) {
            ALOGW("Session_CreateEffect could not get apm engine");
 goto error;
 }
        session->apm->set_sample_rate_hz(kPreprocDefaultSr);
        session->apm->set_num_channels(kPreProcDefaultCnl, kPreProcDefaultCnl);
        session->apm->set_num_reverse_channels(kPreProcDefaultCnl);
        session->procFrame = new webrtc::AudioFrame();
 if (session->procFrame == NULL) {
            ALOGW("Session_CreateEffect could not allocate audio frame");
 goto error;
 }
        session->revFrame = new webrtc::AudioFrame();
 if (session->revFrame == NULL) {
            ALOGW("Session_CreateEffect could not allocate reverse audio frame");
 goto error;
 }
        session->apmSamplingRate = kPreprocDefaultSr;
        session->apmFrameCount = (kPreprocDefaultSr) / 100;
        session->frameCount = session->apmFrameCount;
        session->samplingRate = kPreprocDefaultSr;
        session->inChannelCount = kPreProcDefaultCnl;
        session->outChannelCount = kPreProcDefaultCnl;
        session->procFrame->_frequencyInHz = kPreprocDefaultSr;
        session->procFrame->_audioChannel = kPreProcDefaultCnl;
        session->revChannelCount = kPreProcDefaultCnl;
        session->revFrame->_frequencyInHz = kPreprocDefaultSr;
        session->revFrame->_audioChannel = kPreProcDefaultCnl;
        session->enabledMsk = 0;
        session->processedMsk = 0;
        session->revEnabledMsk = 0;
        session->revProcessedMsk = 0;
        session->inResampler = NULL;
        session->inBuf = NULL;
        session->inBufSize = 0;
        session->outResampler = NULL;
        session->outBuf = NULL;
        session->outBufSize = 0;
        session->revResampler = NULL;
        session->revBuf = NULL;
        session->revBufSize = 0;
 }
    status = Effect_Create(&session->effects[procId], session, interface);
 if (status < 0) {
 goto error;
 }
    ALOGV("Session_CreateEffect OK");
    session->createdMsk |= (1<<procId);
 return status;

error:
 if (session->createdMsk == 0) {
 delete session->revFrame;
        session->revFrame = NULL;
 delete session->procFrame;
        session->procFrame = NULL;
        webrtc::AudioProcessing::Destroy(session->apm);
        session->apm = NULL;
 }
 return status;
}
