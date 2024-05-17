SoftMPEG4Encoder::SoftMPEG4Encoder(
 const char *name,
 const char *componentRole,
            OMX_VIDEO_CODINGTYPE codingType,
 const char *mime,
 const CodecProfileLevel *profileLevels,
 size_t numProfileLevels,
 const OMX_CALLBACKTYPE *callbacks,
            OMX_PTR appData,
            OMX_COMPONENTTYPE **component)
 : SoftVideoEncoderOMXComponent(
            name, componentRole, codingType,
            profileLevels, numProfileLevels,
 176  , 144  ,
            callbacks, appData, component),
      mEncodeMode(COMBINE_MODE_WITH_ERR_RES),
      mIDRFrameRefreshIntervalInSec(1),
      mNumInputFrames(-1),
      mStarted(false),
      mSawInputEOS(false),
      mSignalledError(false),
      mHandle(new tagvideoEncControls),
      mEncParams(new tagvideoEncOptions),
      mInputFrameData(NULL) {

 if (codingType == OMX_VIDEO_CodingH263) {
        mEncodeMode = H263_MODE;
 }

 const size_t kOutputBufferSize = 256 * 1024;

    initPorts(kNumBuffers, kNumBuffers, kOutputBufferSize, mime);

    ALOGI("Construct SoftMPEG4Encoder");
}
