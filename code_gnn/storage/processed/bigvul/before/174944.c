static void disconnectWindow(const sp<ANativeWindow>& window) {
 if (window != 0) {
 status_t result = native_window_api_disconnect(window.get(),
                NATIVE_WINDOW_API_CAMERA);
 if (result != NO_ERROR) {
            ALOGW("native_window_api_disconnect failed: %s (%d)", strerror(-result),
                    result);
 }
 }
}
