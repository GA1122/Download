void Camera3Device::setErrorStateLockedV(const char *fmt, va_list args) {
 String8 errorCause = String8::formatV(fmt, args);
    ALOGE("Camera %d: %s", mId, errorCause.string());

 if (mStatus == STATUS_ERROR || mStatus == STATUS_UNINITIALIZED) return;

    mErrorCause = errorCause;

    mRequestThread->setPaused(true);
    internalUpdateStatusLocked(STATUS_ERROR);

 if (mListener != NULL) {
        mListener->notifyError(ICameraDeviceCallbacks::ERROR_CAMERA_DEVICE,
 CaptureResultExtras());
 }

 CameraTraces::saveTrace();
}
