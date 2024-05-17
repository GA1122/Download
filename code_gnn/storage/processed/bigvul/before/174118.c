status_t SoftAMR::initDecoder() {
 if (mMode == MODE_NARROW) {
 Word16 err = GSMInitDecode(&mState, (Word8 *)"AMRNBDecoder");

 if (err != 0) {
 return UNKNOWN_ERROR;
 }
 } else {
 int32_t memReq = pvDecoder_AmrWbMemRequirements();
        mDecoderBuf = malloc(memReq);

        pvDecoder_AmrWb_Init(&mState, mDecoderBuf, &mDecoderCookie);
 }

 return OK;
}
