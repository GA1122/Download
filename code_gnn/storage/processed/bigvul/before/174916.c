status_t Camera2Client::setPreviewCallbackTarget(
 const sp<IGraphicBufferProducer>& callbackProducer) {
    ATRACE_CALL();
    ALOGV("%s: E", __FUNCTION__);
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

    sp<ANativeWindow> window;
 if (callbackProducer != 0) {
        window = new Surface(callbackProducer);
 }

    res = mCallbackProcessor->setCallbackWindow(window);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to set preview callback surface: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

 SharedParameters::Lock l(mParameters);

 if (window != NULL) {
        l.mParameters.previewCallbackFlags = CAMERA_FRAME_CALLBACK_FLAG_NOOP;
        l.mParameters.previewCallbackOneShot = false;
        l.mParameters.previewCallbackSurface = true;
 } else {
        l.mParameters.previewCallbackSurface = false;
 }

 switch(l.mParameters.state) {
 case Parameters::PREVIEW:
            res = startPreviewL(l.mParameters, true);
 break;
 case Parameters::RECORD:
 case Parameters::VIDEO_SNAPSHOT:
            res = startRecordingL(l.mParameters, true);
 break;
 default:
 break;
 }
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to refresh request in state %s",
                __FUNCTION__, mCameraId,
 Parameters::getStateName(l.mParameters.state));
 }

 return OK;
}
