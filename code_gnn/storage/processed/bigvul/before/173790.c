SoftVorbis::SoftVorbis(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mInputBufferCount(0),
      mState(NULL),
      mVi(NULL),
      mAnchorTimeUs(0),
      mNumFramesOutput(0),
      mNumFramesLeftOnPage(-1),
      mSawInputEos(false),
      mSignalledOutputEos(false),
      mOutputPortSettingsChange(NONE) {
    initPorts();
    CHECK_EQ(initDecoder(), (status_t)OK);
}
