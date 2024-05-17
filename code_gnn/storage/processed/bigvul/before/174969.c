status_t CameraClient::setPreviewWindow(const sp<IBinder>& binder,
 const sp<ANativeWindow>& window) {
 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 if (binder == mSurface) {
 return NO_ERROR;
 }

 if (window != 0) {
        result = native_window_api_connect(window.get(), NATIVE_WINDOW_API_CAMERA);
 if (result != NO_ERROR) {
            ALOGE("native_window_api_connect failed: %s (%d)", strerror(-result),
                    result);
 return result;
 }
 }

 if (mHardware->previewEnabled()) {
 if (window != 0) {
            native_window_set_scaling_mode(window.get(),
                    NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW);
            native_window_set_buffers_transform(window.get(), mOrientation);
            result = mHardware->setPreviewWindow(window);
 }
 }

 if (result == NO_ERROR) {
        disconnectWindow(mPreviewWindow);
        mSurface = binder;
        mPreviewWindow = window;
 } else {
        disconnectWindow(window);
 }

 return result;
}
