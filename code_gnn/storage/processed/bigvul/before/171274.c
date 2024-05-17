bool SoftMPEG4::handlePortSettingsChange() {
 uint32_t disp_width, disp_height;
 PVGetVideoDimensions(mHandle, (int32 *)&disp_width, (int32 *)&disp_height);

 uint32_t buf_width, buf_height;
 PVGetBufferDimensions(mHandle, (int32 *)&buf_width, (int32 *)&buf_height);

    CHECK_LE(disp_width, buf_width);
    CHECK_LE(disp_height, buf_height);

    ALOGV("disp_width = %d, disp_height = %d, buf_width = %d, buf_height = %d",
            disp_width, disp_height, buf_width, buf_height);

 CropSettingsMode cropSettingsMode = kCropUnSet;
 if (disp_width != buf_width || disp_height != buf_height) {
        cropSettingsMode = kCropSet;

 if (mCropWidth != disp_width || mCropHeight != disp_height) {
            mCropLeft = 0;
            mCropTop = 0;
            mCropWidth = disp_width;
            mCropHeight = disp_height;
            cropSettingsMode = kCropChanged;
 }
 }

 bool portWillReset = false;
 const bool fakeStride = true;
 SoftVideoDecoderOMXComponent::handlePortSettingsChange(
 &portWillReset, buf_width, buf_height, cropSettingsMode, fakeStride);
 if (portWillReset) {
 if (mMode == MODE_H263) {
 PVCleanUpVideoDecoder(mHandle);

 uint8_t *vol_data[1];
 int32_t vol_size = 0;

            vol_data[0] = NULL;
 if (!PVInitVideoDecoder(
                    mHandle, vol_data, &vol_size, 1, outputBufferWidth(), outputBufferHeight(),
                    H263_MODE)) {
                notify(OMX_EventError, OMX_ErrorUndefined, 0, NULL);
                mSignalledError = true;
 return true;
 }
 }

        mFramesConfigured = false;
 }

 return portWillReset;
}
