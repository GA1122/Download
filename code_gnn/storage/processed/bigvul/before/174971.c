status_t CameraClient::startPreviewMode() {
    LOG1("startPreviewMode");
 status_t result = NO_ERROR;

 if (mHardware->previewEnabled()) {
 return NO_ERROR;
 }

 if (mPreviewWindow != 0) {
        native_window_set_scaling_mode(mPreviewWindow.get(),
                NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
        native_window_set_buffers_transform(mPreviewWindow.get(),
                mOrientation);
 }
    mHardware->setPreviewWindow(mPreviewWindow);
    result = mHardware->startPreview();

 return result;
}
