SoftMPEG4::SoftMPEG4(
 const char *name,
 const char *componentRole,
        OMX_VIDEO_CODINGTYPE codingType,
 const CodecProfileLevel *profileLevels,
 size_t numProfileLevels,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SoftVideoDecoderOMXComponent(
            name, componentRole, codingType, profileLevels, numProfileLevels,
 352  , 288  , callbacks, appData, component),
      mMode(codingType == OMX_VIDEO_CodingH263 ? MODE_H263 : MODE_MPEG4),
      mHandle(new tagvideoDecControls),
      mInputBufferCount(0),
      mSignalledError(false),
      mInitialized(false),
      mFramesConfigured(false),
      mNumSamplesOutput(0),
      mPvTime(0) {
    initPorts(
            kNumInputBuffers,
 352 * 288 * 3 / 2  ,
            kNumOutputBuffers,
 (mMode == MODE_MPEG4)
 ? MEDIA_MIMETYPE_VIDEO_MPEG4 : MEDIA_MIMETYPE_VIDEO_H263);
    CHECK_EQ(initDecoder(), (status_t)OK);
}
