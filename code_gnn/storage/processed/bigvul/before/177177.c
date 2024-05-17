uint32_t SoftVideoDecoderOMXComponent::outputBufferHeight() {
 return mIsAdaptive ? mAdaptiveMaxHeight : mHeight;
}
