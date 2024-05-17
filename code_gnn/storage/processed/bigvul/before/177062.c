void SoftAVC::onReset() {
 SoftVideoDecoderOMXComponent::onReset();

    mSignalledError = false;
    resetDecoder();

     resetPlugin();
 }
