OMX_ERRORTYPE SoftFlacEncoder::initCheck() const {
 if (mSignalledError) {
 if (mFlacStreamEncoder == NULL) {
            ALOGE("initCheck() failed due to NULL encoder");
 } else if (mInputBufferPcm32 == NULL) {
            ALOGE("initCheck() failed due to error allocating internal input buffer");
 }
 return OMX_ErrorUndefined;
 } else {
 return SimpleSoftOMXComponent::initCheck();
 }
}
