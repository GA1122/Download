void SoftFlacEncoder::onQueueFilled(OMX_U32 portIndex) {
    UNUSED_UNLESS_VERBOSE(portIndex);
    ALOGV("SoftFlacEncoder::onQueueFilled(portIndex=%d)", portIndex);

 if (mSignalledError) {
 return;
 }

 List<BufferInfo *> &inQueue = getPortQueue(0);
 List<BufferInfo *> &outQueue = getPortQueue(1);

 while (!inQueue.empty() && !outQueue.empty()) {
 BufferInfo *inInfo = *inQueue.begin();
        OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;

 BufferInfo *outInfo = *outQueue.begin();
        OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;

 if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
            inQueue.erase(inQueue.begin());
            inInfo->mOwnedByUs = false;
            notifyEmptyBufferDone(inHeader);

            outHeader->nFilledLen = 0;
            outHeader->nFlags = OMX_BUFFERFLAG_EOS;

            outQueue.erase(outQueue.begin());
            outInfo->mOwnedByUs = false;
            notifyFillBufferDone(outHeader);

 return;
 }

 if (inHeader->nFilledLen > kMaxInputBufferSize) {
            ALOGE("input buffer too large (%d).", inHeader->nFilledLen);
            mSignalledError = true;
            notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
 return;
 }

        assert(mNumChannels != 0);
        mEncoderWriteData = true;
        mEncoderReturnedEncodedData = false;
        mEncoderReturnedNbBytes = 0;
        mCurrentInputTimeStamp = inHeader->nTimeStamp;

 const unsigned nbInputFrames = inHeader->nFilledLen / (2 * mNumChannels);
 const unsigned nbInputSamples = inHeader->nFilledLen / 2;
 const OMX_S16 * const pcm16 = reinterpret_cast<OMX_S16 *>(inHeader->pBuffer);

        CHECK_LE(nbInputSamples, 2 * kMaxNumSamplesPerFrame);
 for (unsigned i=0 ; i < nbInputSamples ; i++) {
            mInputBufferPcm32[i] = (FLAC__int32) pcm16[i];
 }
        ALOGV(" about to encode %u samples per channel", nbInputFrames);
        FLAC__bool ok = FLAC__stream_encoder_process_interleaved(
                        mFlacStreamEncoder,
                        mInputBufferPcm32,
                        nbInputFrames   );

 if (ok) {
 if (mEncoderReturnedEncodedData && (mEncoderReturnedNbBytes != 0)) {
                ALOGV(" dequeueing buffer on output port after writing data");
                outInfo->mOwnedByUs = false;
                outQueue.erase(outQueue.begin());
                outInfo = NULL;
                notifyFillBufferDone(outHeader);
                outHeader = NULL;
                mEncoderReturnedEncodedData = false;
 } else {
                ALOGV(" encoder process_interleaved returned without data to write");
 }
 } else {
            ALOGE(" error encountered during encoding");
            mSignalledError = true;
            notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
 return;
 }

        inInfo->mOwnedByUs = false;
        inQueue.erase(inQueue.begin());
        inInfo = NULL;
        notifyEmptyBufferDone(inHeader);
        inHeader = NULL;
 }
}
