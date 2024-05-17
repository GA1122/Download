void SoftVideoDecoderOMXComponent::updatePortDefinitions(bool updateCrop, bool updateInputSize) {
    OMX_PARAM_PORTDEFINITIONTYPE *outDef = &editPortInfo(kOutputPortIndex)->mDef;
    outDef->format.video.nFrameWidth = outputBufferWidth();
    outDef->format.video.nFrameHeight = outputBufferHeight();
    outDef->format.video.nStride = outDef->format.video.nFrameWidth;
    outDef->format.video.nSliceHeight = outDef->format.video.nFrameHeight;

    outDef->nBufferSize =
 (outDef->format.video.nStride * outDef->format.video.nSliceHeight * 3) / 2;

    OMX_PARAM_PORTDEFINITIONTYPE *inDef = &editPortInfo(kInputPortIndex)->mDef;
    inDef->format.video.nFrameWidth = mWidth;
    inDef->format.video.nFrameHeight = mHeight;
    inDef->format.video.nStride = 0;
    inDef->format.video.nSliceHeight = 0;

 if (updateInputSize) {
        inDef->nBufferSize = max(
                outDef->nBufferSize / mMinCompressionRatio,
                max(mMinInputBufferSize, inDef->nBufferSize));
 }

 if (updateCrop) {
        mCropLeft = 0;
        mCropTop = 0;
        mCropWidth = mWidth;
        mCropHeight = mHeight;
 }
}
