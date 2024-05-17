status_t SoftVPXEncoder::releaseEncoder() {
 if (mCodecContext != NULL) {
        vpx_codec_destroy(mCodecContext);
 delete mCodecContext;
        mCodecContext = NULL;
 }

 if (mCodecConfiguration != NULL) {
 delete mCodecConfiguration;
        mCodecConfiguration = NULL;
 }

 if (mConversionBuffer != NULL) {
        free(mConversionBuffer);
        mConversionBuffer = NULL;
 }

    mCodecInterface = NULL;

 return OK;
}
