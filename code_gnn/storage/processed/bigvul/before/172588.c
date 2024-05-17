void SoftHEVC::onReset() {
    ALOGV("onReset called");
 SoftVideoDecoderOMXComponent::onReset();

    mSignalledError = false;
    resetDecoder();
    resetPlugin();
}
