SoftG711::SoftG711(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mIsMLaw(true),
      mSignalledError(false),
      mNumChannels(1),
      mSamplingRate(8000) {
 if (!strcmp(name, "OMX.google.g711.alaw.decoder")) {
        mIsMLaw = false;
 } else {
        CHECK(!strcmp(name, "OMX.google.g711.mlaw.decoder"));
 }

    initPorts();
}
