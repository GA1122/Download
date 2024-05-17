SoftAVC::SoftAVC(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoEncoderOMXComponent(
            name, "video_encoder.avc", OMX_VIDEO_CodingAVC,
            kProfileLevels, NELEM(kProfileLevels),
 176  , 144  ,
            callbacks, appData, component),
      mBitrateUpdated(false),
      mKeyFrameRequested(false),
      mIvVideoColorFormat(IV_YUV_420P),
      mAVCEncProfile(IV_PROFILE_BASE),
      mAVCEncLevel(41),
      mStarted(false),
      mSawInputEOS(false),
      mSawOutputEOS(false),
      mSignalledError(false),
      mCodecCtx(NULL) {

    initPorts(kNumBuffers, kNumBuffers, ((mWidth * mHeight * 3) >> 1),
            MEDIA_MIMETYPE_VIDEO_AVC, 2);

    GENERATE_FILE_NAMES();
    CREATE_DUMP_FILE(mInFile);
    CREATE_DUMP_FILE(mOutFile);
    memset(mConversionBuffers, 0, sizeof(mConversionBuffers));
    memset(mInputBufferInfo, 0, sizeof(mInputBufferInfo));

    initEncParams();

}
