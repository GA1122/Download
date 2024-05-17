SoftVPX::SoftVPX(
 const char *name,
 const char *componentRole,
        OMX_VIDEO_CODINGTYPE codingType,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoDecoderOMXComponent(
            name, componentRole, codingType,
            codingType == OMX_VIDEO_CodingVP8 ? NULL : kVP9ProfileLevels,
            codingType == OMX_VIDEO_CodingVP8 ? 0 : NELEM(kVP9ProfileLevels),
 320  , 240  , callbacks, appData, component),
      mMode(codingType == OMX_VIDEO_CodingVP8 ? MODE_VP8 : MODE_VP9),
      mEOSStatus(INPUT_DATA_AVAILABLE),
      mCtx(NULL),
      mFrameParallelMode(false),
      mTimeStampIdx(0),
      mImg(NULL) {
 const size_t kMinCompressionRatio = mMode == MODE_VP8 ? 2 : 4;
 const char *mime = mMode == MODE_VP8 ? MEDIA_MIMETYPE_VIDEO_VP8 : MEDIA_MIMETYPE_VIDEO_VP9;
 const size_t kMaxOutputBufferSize = 2048 * 2048 * 3 / 2;
    initPorts(
            kNumBuffers, kMaxOutputBufferSize / kMinCompressionRatio  ,
            kNumBuffers, mime, kMinCompressionRatio);
    CHECK_EQ(initDecoder(), (status_t)OK);
}
