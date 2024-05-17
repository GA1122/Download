SoftMP3::SoftMP3(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mConfig(new tPVMP3DecoderExternal),
      mDecoderBuf(NULL),
      mAnchorTimeUs(0),
      mNumFramesOutput(0),
      mNumChannels(2),
      mSamplingRate(44100),
      mSignalledError(false),
      mSawInputEos(false),
      mSignalledOutputEos(false),
      mOutputPortSettingsChange(NONE) {
    initPorts();
    initDecoder();
}
