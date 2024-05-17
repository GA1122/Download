uint32_t SoftVideoDecoderOMXComponent::outputBufferWidth() {
 return mIsAdaptive ? mAdaptiveMaxWidth : mWidth;
}
