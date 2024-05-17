SoftAVC::SoftAVC(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoDecoderOMXComponent(
            name, "video_decoder.avc", OMX_VIDEO_CodingAVC,
            kProfileLevels, ARRAY_SIZE(kProfileLevels),
 320  , 240  , callbacks, appData, component),
      mHandle(NULL),
      mInputBufferCount(0),
      mFirstPicture(NULL),
      mFirstPictureId(-1),
      mPicId(0),
      mHeadersDecoded(false),
      mEOSStatus(INPUT_DATA_AVAILABLE),
      mSignalledError(false) {
 const size_t kMinCompressionRatio = 2;
 const size_t kMaxOutputBufferSize = 2048 * 2048 * 3 / 2;
    initPorts(
            kNumInputBuffers, kMaxOutputBufferSize / kMinCompressionRatio  ,
            kNumOutputBuffers, MEDIA_MIMETYPE_VIDEO_AVC, kMinCompressionRatio);

    CHECK_EQ(initDecoder(), (status_t)OK);
}
