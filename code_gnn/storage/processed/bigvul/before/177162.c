SoftRaw::SoftRaw(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mSignalledError(false),
      mChannelCount(2),
      mSampleRate(44100) {
    initPorts();
    CHECK_EQ(initDecoder(), (status_t)OK);
}
