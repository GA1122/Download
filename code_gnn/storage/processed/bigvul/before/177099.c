SoftAMRWBEncoder::SoftAMRWBEncoder(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mEncoderHandle(NULL),
      mApiHandle(NULL),
      mMemOperator(NULL),
      mBitRate(0),
      mMode(VOAMRWB_MD66),
      mInputSize(0),
      mInputTimeUs(-1ll),
      mSawInputEOS(false),
      mSignalledError(false) {
    initPorts();
    CHECK_EQ(initEncoder(), (status_t)OK);
}
