OMXCodec::OMXCodec(
 const sp<IOMX> &omx, IOMX::node_id node,
 uint32_t quirks, uint32_t flags,
 bool isEncoder,
 const char *mime,
 const char *componentName,
 const sp<MediaSource> &source,
 const sp<ANativeWindow> &nativeWindow)
 : mOMX(omx),
      mOMXLivesLocally(omx->livesLocally(node, getpid())),
      mNode(node),
      mQuirks(quirks),
      mFlags(flags),
      mIsEncoder(isEncoder),
      mIsVideo(!strncasecmp("video/", mime, 6)),
      mMIME(strdup(mime)),
      mComponentName(strdup(componentName)),
      mSource(source),
      mCodecSpecificDataIndex(0),
      mState(LOADED),
      mInitialBufferSubmit(true),
      mSignalledEOS(false),
      mNoMoreOutputData(false),
      mOutputPortSettingsHaveChanged(false),
      mSeekTimeUs(-1),
      mSeekMode(ReadOptions::SEEK_CLOSEST_SYNC),
      mTargetTimeUs(-1),
      mOutputPortSettingsChangedPending(false),
      mSkipCutBuffer(NULL),
      mLeftOverBuffer(NULL),
      mPaused(false),
      mNativeWindow(
 (!strncmp(componentName, "OMX.google.", 11))
 ? NULL : nativeWindow) {
    mPortStatus[kPortIndexInput] = ENABLED;
    mPortStatus[kPortIndexOutput] = ENABLED;

    setComponentRole();
}
