OMX_ERRORTYPE SoftMPEG4Encoder::initEncParams() {
    CHECK(mHandle != NULL);
    memset(mHandle, 0, sizeof(tagvideoEncControls));

    CHECK(mEncParams != NULL);
    memset(mEncParams, 0, sizeof(tagvideoEncOptions));
 if (!PVGetDefaultEncOption(mEncParams, 0)) {
        ALOGE("Failed to get default encoding parameters");
 return OMX_ErrorUndefined;
 }
 if (mVideoFrameRate == 0) {
        ALOGE("Framerate should not be 0");
 return OMX_ErrorUndefined;
 }
    mEncParams->encMode = mEncodeMode;
    mEncParams->encWidth[0] = mVideoWidth;
    mEncParams->encHeight[0] = mVideoHeight;
    mEncParams->encFrameRate[0] = mVideoFrameRate;
    mEncParams->rcType = VBR_1;
    mEncParams->vbvDelay = 5.0f;

    mEncParams->profile_level = CORE_PROFILE_LEVEL2;
    mEncParams->packetSize = 32;
    mEncParams->rvlcEnable = PV_OFF;
    mEncParams->numLayers = 1;
    mEncParams->timeIncRes = 1000;
    mEncParams->tickPerSrc = mEncParams->timeIncRes / mVideoFrameRate;

    mEncParams->bitRate[0] = mVideoBitRate;
    mEncParams->iQuant[0] = 15;
    mEncParams->pQuant[0] = 12;
    mEncParams->quantType[0] = 0;
    mEncParams->noFrameSkipped = PV_OFF;

 if (mVideoColorFormat != OMX_COLOR_FormatYUV420Planar
 || mStoreMetaDataInBuffers) {
        free(mInputFrameData);
        mInputFrameData = NULL;
 if (((uint64_t)mVideoWidth * mVideoHeight) > ((uint64_t)INT32_MAX / 3)) {
            ALOGE("b/25812794, Buffer size is too big.");
 return OMX_ErrorBadParameter;
 }
        mInputFrameData =
 (uint8_t *) malloc((mVideoWidth * mVideoHeight * 3 ) >> 1);
        CHECK(mInputFrameData != NULL);
 }

 if (mVideoWidth % 16 != 0 || mVideoHeight % 16 != 0) {
        ALOGE("Video frame size %dx%d must be a multiple of 16",
            mVideoWidth, mVideoHeight);
 return OMX_ErrorBadParameter;
 }

 if (mIDRFrameRefreshIntervalInSec < 0) {
        mEncParams->intraPeriod = -1;
 } else if (mIDRFrameRefreshIntervalInSec == 0) {
        mEncParams->intraPeriod = 1;  
 } else {
        mEncParams->intraPeriod =
 (mIDRFrameRefreshIntervalInSec * mVideoFrameRate);
 }

    mEncParams->numIntraMB = 0;
    mEncParams->sceneDetect = PV_ON;
    mEncParams->searchRange = 16;
    mEncParams->mv8x8Enable = PV_OFF;
    mEncParams->gobHeaderInterval = 0;
    mEncParams->useACPred = PV_ON;
    mEncParams->intraDCVlcTh = 0;

 return OMX_ErrorNone;
}
