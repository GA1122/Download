SoftOpus::SoftOpus(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mInputBufferCount(0),
      mDecoder(NULL),
      mHeader(NULL),
      mCodecDelay(0),
      mSeekPreRoll(0),
      mAnchorTimeUs(0),
      mNumFramesOutput(0),
      mOutputPortSettingsChange(NONE) {
    initPorts();
    CHECK_EQ(initDecoder(), (status_t)OK);
}
