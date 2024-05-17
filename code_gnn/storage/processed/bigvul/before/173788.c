void SoftOpus::onReset() {
    mInputBufferCount = 0;
    mNumFramesOutput = 0;
 if (mDecoder != NULL) {
        opus_multistream_decoder_destroy(mDecoder);
        mDecoder = NULL;
 }
 if (mHeader != NULL) {
 delete mHeader;
        mHeader = NULL;
 }

    mOutputPortSettingsChange = NONE;
}
