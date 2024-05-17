status_t Camera2Client::setPreviewWindowL(const sp<IBinder>& binder,
        sp<ANativeWindow> window) {
    ATRACE_CALL();
 status_t res;

 if (binder == mPreviewSurface) {
        ALOGV("%s: Camera %d: New window is same as old window",
                __FUNCTION__, mCameraId);
 return NO_ERROR;
 }

 Parameters::State state;
 {
 SharedParameters::Lock l(mParameters);
        state = l.mParameters.state;
 }
 switch (state) {
 case Parameters::DISCONNECTED:
 case Parameters::RECORD:
 case Parameters::STILL_CAPTURE:
 case Parameters::VIDEO_SNAPSHOT:
            ALOGE("%s: Camera %d: Cannot set preview display while in state %s",
                    __FUNCTION__, mCameraId,
 Parameters::getStateName(state));
 return INVALID_OPERATION;
 case Parameters::STOPPED:
 case Parameters::WAITING_FOR_PREVIEW_WINDOW:
 break;
 case Parameters::PREVIEW:
            res = stopStream();
 if (res != OK) {
                ALOGE("%s: Unable to stop preview to swap windows: %s (%d)",
                        __FUNCTION__, strerror(-res), res);
 return res;
 }
            state = Parameters::WAITING_FOR_PREVIEW_WINDOW;
 break;
 }

    mPreviewSurface = binder;
    res = mStreamingProcessor->setPreviewWindow(window);
 if (res != OK) {
        ALOGE("%s: Unable to set new preview window: %s (%d)",
                __FUNCTION__, strerror(-res), res);
 return res;
 }

 if (state == Parameters::WAITING_FOR_PREVIEW_WINDOW) {
 SharedParameters::Lock l(mParameters);
        l.mParameters.state = state;
 return startPreviewL(l.mParameters, false);
 }

 return OK;
}
