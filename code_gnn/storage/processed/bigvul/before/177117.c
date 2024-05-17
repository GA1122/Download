OMX_ERRORTYPE SoftAVCEncoder::initEncoder() {
    CHECK(!mStarted);

    OMX_ERRORTYPE errType = OMX_ErrorNone;
 if (OMX_ErrorNone != (errType = initEncParams())) {
        ALOGE("Failed to initialized encoder params");
        mSignalledError = true;
        notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return errType;
 }

 AVCEnc_Status err;
    err = PVAVCEncInitialize(mHandle, mEncParams, NULL, NULL);
 if (err != AVCENC_SUCCESS) {
        ALOGE("Failed to initialize the encoder: %d", err);
        mSignalledError = true;
        notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return OMX_ErrorUndefined;
 }

    mNumInputFrames = -2;  
    mSpsPpsHeaderReceived = false;
    mReadyForNextFrame = true;
    mIsIDRFrame = false;
    mStarted = true;

 return OMX_ErrorNone;
}
