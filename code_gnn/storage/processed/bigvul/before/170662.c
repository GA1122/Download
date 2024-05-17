int Session_ReleaseEffect(preproc_session_t *session,
 preproc_effect_t *fx)
{
    ALOGW_IF(Effect_Release(fx) != 0, " Effect_Release() failed for proc ID %d", fx->procId);
    session->createdMsk &= ~(1<<fx->procId);
 if (session->createdMsk == 0) {
        webrtc::AudioProcessing::Destroy(session->apm);
        session->apm = NULL;
 delete session->procFrame;
        session->procFrame = NULL;
 delete session->revFrame;
        session->revFrame = NULL;
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
 delete session->inBuf;
        session->inBuf = NULL;
 delete session->outBuf;
        session->outBuf = NULL;
 delete session->revBuf;
        session->revBuf = NULL;

        session->io = 0;
 }

 return 0;
}
