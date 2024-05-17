OMX_ERRORTYPE SoftFlacEncoder::configureEncoder() {
    ALOGV("SoftFlacEncoder::configureEncoder() numChannel=%d, sampleRate=%d",
            mNumChannels, mSampleRate);

 if (mSignalledError || (mFlacStreamEncoder == NULL)) {
        ALOGE("can't configure encoder: no encoder or invalid state");
 return OMX_ErrorInvalidState;
 }

    FLAC__bool ok = true;
    ok = ok && FLAC__stream_encoder_set_channels(mFlacStreamEncoder, mNumChannels);
    ok = ok && FLAC__stream_encoder_set_sample_rate(mFlacStreamEncoder, mSampleRate);
    ok = ok && FLAC__stream_encoder_set_bits_per_sample(mFlacStreamEncoder, 16);
    ok = ok && FLAC__stream_encoder_set_compression_level(mFlacStreamEncoder,
 (unsigned)mCompressionLevel);
    ok = ok && FLAC__stream_encoder_set_verify(mFlacStreamEncoder, false);
 if (!ok) { goto return_result; }

    ok &= FLAC__STREAM_ENCODER_INIT_STATUS_OK ==
            FLAC__stream_encoder_init_stream(mFlacStreamEncoder,
                    flacEncoderWriteCallback     ,
                    NULL  ,
                    NULL  ,
                    NULL  ,
 (void *) this  );

return_result:
 if (ok) {
        ALOGV("encoder successfully configured");
 return OMX_ErrorNone;
 } else {
        ALOGE("unknown error when configuring encoder");
 return OMX_ErrorUndefined;
 }
}
