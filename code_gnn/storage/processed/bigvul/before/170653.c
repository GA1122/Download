int PreProcessingFx_Process(effect_handle_t     self,
 audio_buffer_t *inBuffer,
 audio_buffer_t *outBuffer)
{
 preproc_effect_t * effect = (preproc_effect_t *)self;
 int    status = 0;

 if (effect == NULL){
        ALOGV("PreProcessingFx_Process() ERROR effect == NULL");
 return -EINVAL;
 }
 preproc_session_t * session = (preproc_session_t *)effect->session;

 if (inBuffer == NULL  || inBuffer->raw == NULL  ||
            outBuffer == NULL || outBuffer->raw == NULL){
        ALOGW("PreProcessingFx_Process() ERROR bad pointer");
 return -EINVAL;
 }

    session->processedMsk |= (1<<effect->procId);


 if ((session->processedMsk & session->enabledMsk) == session->enabledMsk) {
        effect->session->processedMsk = 0;
 size_t framesRq = outBuffer->frameCount;
 size_t framesWr = 0;
 if (session->framesOut) {
 size_t fr = session->framesOut;
 if (outBuffer->frameCount < fr) {
                fr = outBuffer->frameCount;
 }
            memcpy(outBuffer->s16,
                  session->outBuf,
                  fr * session->outChannelCount * sizeof(int16_t));
            memcpy(session->outBuf,
                  session->outBuf + fr * session->outChannelCount,
 (session->framesOut - fr) * session->outChannelCount * sizeof(int16_t));
            session->framesOut -= fr;
            framesWr += fr;
 }
        outBuffer->frameCount = framesWr;
 if (framesWr == framesRq) {
            inBuffer->frameCount = 0;
 return 0;
 }

 if (session->inResampler != NULL) {
 size_t fr = session->frameCount - session->framesIn;
 if (inBuffer->frameCount < fr) {
                fr = inBuffer->frameCount;
 }
 if (session->inBufSize < session->framesIn + fr) {
                session->inBufSize = session->framesIn + fr;
                session->inBuf = (int16_t *)realloc(session->inBuf,
                                 session->inBufSize * session->inChannelCount * sizeof(int16_t));
 }
            memcpy(session->inBuf + session->framesIn * session->inChannelCount,
                   inBuffer->s16,
                   fr * session->inChannelCount * sizeof(int16_t));
#ifdef DUAL_MIC_TEST
            pthread_mutex_lock(&gPcmDumpLock);
 if (gPcmDumpFh != NULL) {
                fwrite(inBuffer->raw,
                       fr * session->inChannelCount * sizeof(int16_t), 1, gPcmDumpFh);
 }
            pthread_mutex_unlock(&gPcmDumpLock);
#endif

            session->framesIn += fr;
            inBuffer->frameCount = fr;
 if (session->framesIn < session->frameCount) {
 return 0;
 }
 spx_uint32_t frIn = session->framesIn;
 spx_uint32_t frOut = session->apmFrameCount;
 if (session->inChannelCount == 1) {
                speex_resampler_process_int(session->inResampler,
 0,
                                            session->inBuf,
 &frIn,
                                            session->procFrame->_payloadData,
 &frOut);
 } else {
                speex_resampler_process_interleaved_int(session->inResampler,
                                                        session->inBuf,
 &frIn,
                                                        session->procFrame->_payloadData,
 &frOut);
 }
            memcpy(session->inBuf,
                   session->inBuf + frIn * session->inChannelCount,
 (session->framesIn - frIn) * session->inChannelCount * sizeof(int16_t));
            session->framesIn -= frIn;
 } else {
 size_t fr = session->frameCount - session->framesIn;
 if (inBuffer->frameCount < fr) {
                fr = inBuffer->frameCount;
 }
            memcpy(session->procFrame->_payloadData + session->framesIn * session->inChannelCount,
                   inBuffer->s16,
                   fr * session->inChannelCount * sizeof(int16_t));

#ifdef DUAL_MIC_TEST
            pthread_mutex_lock(&gPcmDumpLock);
 if (gPcmDumpFh != NULL) {
                fwrite(inBuffer->raw,
                       fr * session->inChannelCount * sizeof(int16_t), 1, gPcmDumpFh);
 }
            pthread_mutex_unlock(&gPcmDumpLock);
#endif

            session->framesIn += fr;
            inBuffer->frameCount = fr;
 if (session->framesIn < session->frameCount) {
 return 0;
 }
            session->framesIn = 0;
 }
        session->procFrame->_payloadDataLengthInSamples =
                session->apmFrameCount * session->inChannelCount;

        effect->session->apm->ProcessStream(session->procFrame);

 if (session->outBufSize < session->framesOut + session->frameCount) {
            session->outBufSize = session->framesOut + session->frameCount;
            session->outBuf = (int16_t *)realloc(session->outBuf,
                              session->outBufSize * session->outChannelCount * sizeof(int16_t));
 }

 if (session->outResampler != NULL) {
 spx_uint32_t frIn = session->apmFrameCount;
 spx_uint32_t frOut = session->frameCount;
 if (session->inChannelCount == 1) {
                speex_resampler_process_int(session->outResampler,
 0,
                                    session->procFrame->_payloadData,
 &frIn,
                                    session->outBuf + session->framesOut * session->outChannelCount,
 &frOut);
 } else {
                speex_resampler_process_interleaved_int(session->outResampler,
                                    session->procFrame->_payloadData,
 &frIn,
                                    session->outBuf + session->framesOut * session->outChannelCount,
 &frOut);
 }
            session->framesOut += frOut;
 } else {
            memcpy(session->outBuf + session->framesOut * session->outChannelCount,
                   session->procFrame->_payloadData,
                   session->frameCount * session->outChannelCount * sizeof(int16_t));
            session->framesOut += session->frameCount;
 }
 size_t fr = session->framesOut;
 if (framesRq - framesWr < fr) {
            fr = framesRq - framesWr;
 }
        memcpy(outBuffer->s16 + framesWr * session->outChannelCount,
              session->outBuf,
              fr * session->outChannelCount * sizeof(int16_t));
        memcpy(session->outBuf,
              session->outBuf + fr * session->outChannelCount,
 (session->framesOut - fr) * session->outChannelCount * sizeof(int16_t));
        session->framesOut -= fr;
        outBuffer->frameCount += fr;

 return 0;
 } else {
 return -ENODATA;
 }
}
