void SoftMPEG4::onReset() {
 SoftVideoDecoderOMXComponent::onReset();
    mPvToOmxTimeMap.clear();
    mSignalledError = false;
    mFramesConfigured = false;
 if (mInitialized) {
 PVCleanUpVideoDecoder(mHandle);
        mInitialized = false;
 }
}
