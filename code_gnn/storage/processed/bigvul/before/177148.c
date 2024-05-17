SoftFlacEncoder::SoftFlacEncoder(
 const char *name,
 const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
 : SimpleSoftOMXComponent(name, callbacks, appData, component),
      mSignalledError(false),
      mNumChannels(1),
      mSampleRate(44100),
      mCompressionLevel(FLAC_COMPRESSION_LEVEL_DEFAULT),
      mEncoderWriteData(false),
      mEncoderReturnedEncodedData(false),
      mEncoderReturnedNbBytes(0),
      mInputBufferPcm32(NULL)
#ifdef WRITE_FLAC_HEADER_IN_FIRST_BUFFER
 , mHeaderOffset(0)
 , mWroteHeader(false)
#endif
{
    ALOGV("SoftFlacEncoder::SoftFlacEncoder(name=%s)", name);
    initPorts();

    mFlacStreamEncoder = FLAC__stream_encoder_new();
 if (mFlacStreamEncoder == NULL) {
        ALOGE("SoftFlacEncoder::SoftFlacEncoder(name=%s) error instantiating FLAC encoder", name);
        mSignalledError = true;
 }

 if (!mSignalledError) {  
        mInputBufferPcm32 = (FLAC__int32*) malloc(sizeof(FLAC__int32) * 2 * kMaxNumSamplesPerFrame);
 if (mInputBufferPcm32 == NULL) {
            ALOGE("SoftFlacEncoder::SoftFlacEncoder(name=%s) error allocating internal input buffer", name);
            mSignalledError = true;
 }
 }
}
