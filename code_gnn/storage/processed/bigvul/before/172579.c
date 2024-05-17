SoftHEVC::SoftHEVC(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoDecoderOMXComponent(name, componentName, codingType,
            kProfileLevels, ARRAY_SIZE(kProfileLevels),
 320  , 240  , callbacks,
            appData, component),
      mCodecCtx(NULL),
      mFlushOutBuffer(NULL),
      mOmxColorFormat(OMX_COLOR_FormatYUV420Planar),
      mIvColorFormat(IV_YUV_420P),
      mChangingResolution(false),
      mSignalledError(false),
      mStride(mWidth) {
 const size_t kMinCompressionRatio = 4  ;
 const size_t kMaxOutputBufferSize = 2048 * 2048 * 3 / 2;
    initPorts(
            kNumBuffers, max(kMaxOutputBufferSize / kMinCompressionRatio, (size_t)INPUT_BUF_SIZE),
            kNumBuffers, CODEC_MIME_TYPE, kMinCompressionRatio);
}
