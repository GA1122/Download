SoftAVCEncoder::SoftAVCEncoder(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
            OMX_PTR appData,
            OMX_COMPONENTTYPE **component)
 : SoftVideoEncoderOMXComponent(
            name, "video_encoder.avc", OMX_VIDEO_CodingAVC,
            kProfileLevels, NELEM(kProfileLevels),
 176  , 144  ,
            callbacks, appData, component),
      mIDRFrameRefreshIntervalInSec(1),
      mAVCEncProfile(AVC_BASELINE),
      mAVCEncLevel(AVC_LEVEL2),
      mNumInputFrames(-1),
      mPrevTimestampUs(-1),
      mStarted(false),
      mSawInputEOS(false),
      mSignalledError(false),
      mHandle(new tagAVCHandle),
      mEncParams(new tagAVCEncParam),
      mInputFrameData(NULL),
      mSliceGroup(NULL) {

 const size_t kOutputBufferSize =
 320 * ConversionTable[NELEM(ConversionTable) - 1].maxMacroBlocks;

    initPorts(
            kNumBuffers, kNumBuffers, kOutputBufferSize,
            MEDIA_MIMETYPE_VIDEO_AVC, 2  );

    ALOGI("Construct SoftAVCEncoder");
}
