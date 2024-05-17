void Session_SetProcEnabled(preproc_session_t *session, uint32_t procId, bool enabled)
{
 if (enabled) {
 if(session->enabledMsk == 0) {
            session->framesIn = 0;
 if (session->inResampler != NULL) {
                speex_resampler_reset_mem(session->inResampler);
 }
            session->framesOut = 0;
 if (session->outResampler != NULL) {
                speex_resampler_reset_mem(session->outResampler);
 }
 }
        session->enabledMsk |= (1 << procId);
 if (HasReverseStream(procId)) {
            session->framesRev = 0;
 if (session->revResampler != NULL) {
                speex_resampler_reset_mem(session->revResampler);
 }
            session->revEnabledMsk |= (1 << procId);
 }
 } else {
        session->enabledMsk &= ~(1 << procId);
 if (HasReverseStream(procId)) {
            session->revEnabledMsk &= ~(1 << procId);
 }
 }
    ALOGV("Session_SetProcEnabled proc %d, enabled %d enabledMsk %08x revEnabledMsk %08x",
         procId, enabled, session->enabledMsk, session->revEnabledMsk);
    session->processedMsk = 0;
 if (HasReverseStream(procId)) {
        session->revProcessedMsk = 0;
 }
}
