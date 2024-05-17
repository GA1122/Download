 void SoftVPX::onQueueFilled(OMX_U32  ) {
     if (mOutputPortSettingsChange != NONE || mEOSStatus == OUTPUT_FRAMES_FLUSHED) {
         return;
 }

 List<BufferInfo *> &inQueue = getPortQueue(0);
 List<BufferInfo *> &outQueue = getPortQueue(1);
 bool EOSseen = false;
 bool portWillReset = false;

 while ((mEOSStatus == INPUT_EOS_SEEN || !inQueue.empty())
 && !outQueue.empty()) {
 if (mEOSStatus == INPUT_EOS_SEEN || mImg != NULL) {
 if (!outputBuffers(
                     mEOSStatus == INPUT_EOS_SEEN, true  ,
                     mEOSStatus == INPUT_EOS_SEEN, &portWillReset)) {
                ALOGE("on2 decoder failed to output frame.");
                notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
 return;
 }
 if (portWillReset || mEOSStatus == OUTPUT_FRAMES_FLUSHED ||
                    mEOSStatus == INPUT_EOS_SEEN) {
 return;
 }
 continue;
 }

 BufferInfo *inInfo = *inQueue.begin();
        OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;

 if (inHeader->nFlags & OMX_BUFFERFLAG_CODECCONFIG) {
 if (mMode == MODE_VP9) {
                inQueue.erase(inQueue.begin());
                inInfo->mOwnedByUs = false;
                notifyEmptyBufferDone(inHeader);
 continue;
 } else {
                ALOGW("WARNING: Got CSD buffer for VP8.");
 }
 }

        mTimeStamps[mTimeStampIdx] = inHeader->nTimeStamp;

 if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
            mEOSStatus = INPUT_EOS_SEEN;
 EOSseen = true;
 }

 if (inHeader->nFilledLen > 0) {
 vpx_codec_err_t err = vpx_codec_decode(
 (vpx_codec_ctx_t *)mCtx, inHeader->pBuffer + inHeader->nOffset,
                    inHeader->nFilledLen, &mTimeStamps[mTimeStampIdx], 0);
 if (err == VPX_CODEC_OK) {
                inInfo->mOwnedByUs = false;
                inQueue.erase(inQueue.begin());
                inInfo = NULL;
                notifyEmptyBufferDone(inHeader);
                inHeader = NULL;
 } else {
                ALOGE("on2 decoder failed to decode frame. err: %d", err);
                notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
 return;
 }
 }

        mTimeStampIdx = (mTimeStampIdx + 1) % kNumBuffers;

 if (!outputBuffers(
 EOSseen  , true  , EOSseen, &portWillReset)) {
            ALOGE("on2 decoder failed to output frame.");
            notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
 return;
 }
 if (portWillReset) {
 return;
 }
 }
}
