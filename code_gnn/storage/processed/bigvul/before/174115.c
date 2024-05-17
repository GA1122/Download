SoftAMR::SoftAMR(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mMode(MODE_NARROW),
      mState(NULL),
      mDecoderBuf(NULL),
      mDecoderCookie(NULL),
      mInputBufferCount(0),
      mAnchorTimeUs(0),
      mNumSamplesOutput(0),
      mSignalledError(false),
      mOutputPortSettingsChange(NONE) {
 if (!strcmp(name, "OMX.google.amrwb.decoder")) {
        mMode = MODE_WIDE;
 } else {
        CHECK(!strcmp(name, "OMX.google.amrnb.decoder"));
 }

    initPorts();
    CHECK_EQ(initDecoder(), (status_t)OK);
}
