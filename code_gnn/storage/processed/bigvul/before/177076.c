OMX_ERRORTYPE SoftMPEG2::internalSetParameter(OMX_INDEXTYPE index, const OMX_PTR params) {
 const uint32_t oldWidth = mWidth;
 const uint32_t oldHeight = mHeight;
    OMX_ERRORTYPE ret = SoftVideoDecoderOMXComponent::internalSetParameter(index, params);
 if (mWidth != oldWidth || mHeight != oldHeight) {
        reInitDecoder();
 }

     return ret;
 }
