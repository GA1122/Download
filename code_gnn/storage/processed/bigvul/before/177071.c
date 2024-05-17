SoftMPEG2::SoftMPEG2(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoDecoderOMXComponent(
            name, componentName, codingType,
            kProfileLevels, ARRAY_SIZE(kProfileLevels),
 320  , 240  , callbacks,
            appData, component),
      mMemRecords(NULL),
      mFlushOutBuffer(NULL),
      mOmxColorFormat(OMX_COLOR_FormatYUV420Planar),
      mIvColorFormat(IV_YUV_420P),
      mNewWidth(mWidth),
      mNewHeight(mHeight),
      mChangingResolution(false) {
    initPorts(kNumBuffers, INPUT_BUF_SIZE, kNumBuffers, CODEC_MIME_TYPE);

    GENERATE_FILE_NAMES();
    CREATE_DUMP_FILE(mInFile);

    CHECK_EQ(initDecoder(), (status_t)OK);
}
