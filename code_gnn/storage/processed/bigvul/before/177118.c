void SoftAVCEncoder::onQueueFilled(OMX_U32  ) {
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
 uint32_t dataLength = outHeader->nAllocLen;

 if (!mSpsPpsHeaderReceived && mNumInputFrames < 0) {
            outPtr += 4;
            dataLength -= 4;
 }

 int32_t type;
 AVCEnc_Status encoderStatus = AVCENC_SUCCESS;

 while (!mSpsPpsHeaderReceived && mNumInputFrames <= 0) {
            encoderStatus = PVAVCEncodeNAL(mHandle, outPtr, &dataLength, &type);
 if (encoderStatus == AVCENC_WRONG_STATE) {
                mSpsPpsHeaderReceived = true;
                CHECK_EQ(0, mNumInputFrames);  
                outHeader->nFlags = OMX_BUFFERFLAG_CODECCONFIG;
                outQueue.erase(outQueue.begin());
                outInfo->mOwnedByUs = false;
                notifyFillBufferDone(outHeader);
 return;
 } else {
 switch (type) {
 case AVC_NALTYPE_SPS:
 ++mNumInputFrames;
                        memcpy((uint8_t *)outHeader->pBuffer, "\x00\x00\x00\x01", 4);
                        outHeader->nFilledLen = 4 + dataLength;
                        outPtr += (dataLength + 4);  
                        dataLength = outHeader->nAllocLen - outHeader->nFilledLen;
 break;
 default:
                        CHECK_EQ(AVC_NALTYPE_PPS, type);
 ++mNumInputFrames;
                        memcpy((uint8_t *) outHeader->pBuffer + outHeader->nFilledLen,
 "\x00\x00\x00\x01", 4);
                        outHeader->nFilledLen += (dataLength + 4);
                        outPtr += (dataLength + 4);
 break;
 }
 }
 }

 if (mReadyForNextFrame) {
 InputBufferInfo info;
            info.mTimeUs = inHeader->nTimeStamp;
            info.mFlags = inHeader->nFlags;
            mInputBufferInfoVec.push(info);
            mPrevTimestampUs = inHeader->nTimeStamp;

 if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
                mSawInputEOS = true;
 }

 if (inHeader->nFilledLen > 0) {
 AVCFrameIO videoInput;
                memset(&videoInput, 0, sizeof(videoInput));
                videoInput.height = align(mHeight, 16);
                videoInput.pitch = align(mWidth, 16);
                videoInput.coding_timestamp = (inHeader->nTimeStamp + 500) / 1000;  
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
                videoInput.YCbCr[0] = (uint8_t *)inputData;
                videoInput.YCbCr[1] = videoInput.YCbCr[0] + videoInput.height * videoInput.pitch;
                videoInput.YCbCr[2] = videoInput.YCbCr[1] +
 ((videoInput.height * videoInput.pitch) >> 2);
                videoInput.disp_order = mNumInputFrames;

                encoderStatus = PVAVCEncSetInput(mHandle, &videoInput);
 if (encoderStatus == AVCENC_SUCCESS || encoderStatus == AVCENC_NEW_IDR) {
                    mReadyForNextFrame = false;
 ++mNumInputFrames;
 if (encoderStatus == AVCENC_NEW_IDR) {
                        mIsIDRFrame = 1;
 }
 } else {
 if (encoderStatus < AVCENC_SUCCESS) {
                        ALOGE("encoderStatus = %d at line %d", encoderStatus, __LINE__);
                        mSignalledError = true;
                        notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return;
 } else {
                        ALOGV("encoderStatus = %d at line %d", encoderStatus, __LINE__);
                        inQueue.erase(inQueue.begin());
                        inInfo->mOwnedByUs = false;
                        notifyEmptyBufferDone(inHeader);
 return;
 }
 }
 }
 }

        CHECK(encoderStatus == AVCENC_SUCCESS || encoderStatus == AVCENC_NEW_IDR);
        dataLength = outHeader->nAllocLen;  
 if (inHeader->nFilledLen > 0) {
 if (outHeader->nAllocLen >= 4) {
                memcpy(outPtr, "\x00\x00\x00\x01", 4);
                outPtr += 4;
                dataLength -= 4;
 }
            encoderStatus = PVAVCEncodeNAL(mHandle, outPtr, &dataLength, &type);
            dataLength = outPtr + dataLength - outHeader->pBuffer;
 if (encoderStatus == AVCENC_SUCCESS) {
                CHECK(NULL == PVAVCEncGetOverrunBuffer(mHandle));
 } else if (encoderStatus == AVCENC_PICTURE_READY) {
                CHECK(NULL == PVAVCEncGetOverrunBuffer(mHandle));
 if (mIsIDRFrame) {
                    outHeader->nFlags |= OMX_BUFFERFLAG_SYNCFRAME;
                    mIsIDRFrame = false;
 }
                mReadyForNextFrame = true;
 AVCFrameIO recon;
 if (PVAVCEncGetRecon(mHandle, &recon) == AVCENC_SUCCESS) {
 PVAVCEncReleaseRecon(mHandle, &recon);
 }
 } else {
                dataLength = 0;
                mReadyForNextFrame = true;
 }

 if (encoderStatus < AVCENC_SUCCESS) {
                ALOGE("encoderStatus = %d at line %d", encoderStatus, __LINE__);
                mSignalledError = true;
                notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return;
 }
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
 if (mSawInputEOS) {
            outHeader->nFlags |= OMX_BUFFERFLAG_EOS;
 }
        outHeader->nFilledLen = dataLength;
        outInfo->mOwnedByUs = false;
        notifyFillBufferDone(outHeader);
        mInputBufferInfoVec.erase(mInputBufferInfoVec.begin());
 }
}
