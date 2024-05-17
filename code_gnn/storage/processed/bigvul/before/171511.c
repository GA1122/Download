void SoftMP3::initDecoder() {
    mConfig->equalizerType = flat;
    mConfig->crcEnabled = false;

 uint32_t memRequirements = pvmp3_decoderMemRequirements();
    mDecoderBuf = malloc(memRequirements);

    pvmp3_InitDecoder(mConfig, mDecoderBuf);

     mIsFirst = true;
 }
