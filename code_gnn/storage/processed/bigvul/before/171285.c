void SoftMPEG4Encoder::onQueueFilled(OMX_U32  ) {
 if (mSignalledError || mSawInputEOS) {
 return;
 }

 if (!mStarted) {
 if (OMX_ErrorNone != initEncoder()) {
 return;
 }
 }

 List<BufferInfo *> &inQueue = getPortQueue(0);
 List<BufferInfo *> &outQueue = getPortQueue(1);

 while (!mSawInputEOS && !inQueue.empty() && !outQueue.empty()) {
 BufferInfo *inInfo = *inQueue.begin();
        OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;
 BufferInfo *outInfo = *outQueue.begin();
        OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;

        outHeader->nTimeStamp = 0;
        outHeader->nFlags = 0;
        outHeader->nOffset = 0;
        outHeader->nFilledLen = 0;
        outHeader->nOffset = 0;

 uint8_t *outPtr = (uint8_t *) outHeader->pBuffer;
 int32_t dataLength = outHeader->nAllocLen;

 if (mNumInputFrames < 0) {
 if (!PVGetVolHeader(mHandle, outPtr, &dataLength, 0)) {
                ALOGE("Failed to get VOL header");
                mSignalledError = true;
                notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return;
 }
            ALOGV("Output VOL header: %d bytes", dataLength);
 ++mNumInputFrames;
            outHeader->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
            outHeader->nFilledLen = dataLength;
            outQueue.erase(outQueue.begin());
            outInfo->mOwnedByUs = false;
            notifyFillBufferDone(outHeader);
 return;
 }

 InputBufferInfo info;
        info.mTimeUs = inHeader->nTimeStamp;
        info.mFlags = inHeader->nFlags;
        mInputBufferInfoVec.push(info);

 if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
            mSawInputEOS = true;
 }

 if (inHeader->nFilledLen > 0) {
 const uint8_t *inputData = NULL;
 if (mInputDataIsMeta) {
                inputData =
                    extractGraphicBuffer(
                            mInputFrameData, (mWidth * mHeight * 3) >> 1,
                            inHeader->pBuffer + inHeader->nOffset, inHeader->nFilledLen,
                            mWidth, mHeight);
 if (inputData == NULL) {
                    ALOGE("Unable to extract gralloc buffer in metadata mode");
                    mSignalledError = true;
                    notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return;
 }
 } else {
                inputData = (const uint8_t *)inHeader->pBuffer + inHeader->nOffset;
 if (mColorFormat != OMX_COLOR_FormatYUV420Planar) {
 ConvertYUV420SemiPlanarToYUV420Planar(
                        inputData, mInputFrameData, mWidth, mHeight);
                    inputData = mInputFrameData;
 }
 }

            CHECK(inputData != NULL);

 VideoEncFrameIO vin, vout;
            memset(&vin, 0, sizeof(vin));
            memset(&vout, 0, sizeof(vout));
            vin.height = align(mHeight, 16);
            vin.pitch = align(mWidth, 16);
            vin.timestamp = (inHeader->nTimeStamp + 500) / 1000;  
            vin.yChan = (uint8_t *)inputData;
            vin.uChan = vin.yChan + vin.height * vin.pitch;
            vin.vChan = vin.uChan + ((vin.height * vin.pitch) >> 2);

 ULong modTimeMs = 0;
 int32_t nLayer = 0;
            MP4HintTrack hintTrack;
 if (!PVEncodeVideoFrame(mHandle, &vin, &vout,
 &modTimeMs, outPtr, &dataLength, &nLayer) ||
 !PVGetHintTrack(mHandle, &hintTrack)) {
                ALOGE("Failed to encode frame or get hink track at frame %" PRId64,
                    mNumInputFrames);
                mSignalledError = true;
                notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 }
            CHECK(NULL == PVGetOverrunBuffer(mHandle));
 if (hintTrack.CodeType == 0) {  
                outHeader->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
 }

 ++mNumInputFrames;
 } else {
            dataLength = 0;
 }

        inQueue.erase(inQueue.begin());
        inInfo->mOwnedByUs = false;
        notifyEmptyBufferDone(inHeader);

        outQueue.erase(outQueue.begin());
        CHECK(!mInputBufferInfoVec.empty());
 InputBufferInfo *inputBufInfo = mInputBufferInfoVec.begin();
        outHeader->nTimeStamp = inputBufInfo->mTimeUs;
        outHeader->nFlags |= (inputBufInfo->mFlags | OMX_BUFFERFLAG_ENDOFFRAME);
        outHeader->nFilledLen = dataLength;
        mInputBufferInfoVec.erase(mInputBufferInfoVec.begin());
        outInfo->mOwnedByUs = false;
        notifyFillBufferDone(outHeader);
 }
}
