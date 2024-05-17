void SoftVideoDecoderOMXComponent::handlePortSettingsChange(
 bool *portWillReset, uint32_t width, uint32_t height,
 CropSettingsMode cropSettingsMode, bool fakeStride) {
 *portWillReset = false;
 bool sizeChanged = (width != mWidth || height != mHeight);
 bool updateCrop = (cropSettingsMode == kCropUnSet);
 bool cropChanged = (cropSettingsMode == kCropChanged);
 bool strideChanged = false;
 if (fakeStride) {
        OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kOutputPortIndex)->mDef;
 if (def->format.video.nStride != (OMX_S32)width
 || def->format.video.nSliceHeight != (OMX_U32)height) {
            strideChanged = true;
 }
 }

 if (sizeChanged || cropChanged || strideChanged) {
        mWidth = width;
        mHeight = height;

 if ((sizeChanged && !mIsAdaptive)
 || width > mAdaptiveMaxWidth
 || height > mAdaptiveMaxHeight) {
 if (mIsAdaptive) {
 if (width > mAdaptiveMaxWidth) {
                    mAdaptiveMaxWidth = width;
 }
 if (height > mAdaptiveMaxHeight) {
                    mAdaptiveMaxHeight = height;
 }
 }
            updatePortDefinitions(updateCrop);
            notify(OMX_EventPortSettingsChanged, kOutputPortIndex, 0, NULL);
            mOutputPortSettingsChange = AWAITING_DISABLED;
 *portWillReset = true;
 } else {
            updatePortDefinitions(updateCrop);

 if (fakeStride) {
                OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kOutputPortIndex)->mDef;
                def->format.video.nStride = mWidth;
                def->format.video.nSliceHeight = mHeight;
 }

            notify(OMX_EventPortSettingsChanged, kOutputPortIndex,
                   OMX_IndexConfigCommonOutputCrop, NULL);
 }
 }
}
