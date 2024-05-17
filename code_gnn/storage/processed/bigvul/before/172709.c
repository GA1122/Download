status_t MediaRecorder::setCamera(const sp<ICamera>& camera, const sp<ICameraRecordingProxy>& proxy)
{
    ALOGV("setCamera(%p,%p)", camera.get(), proxy.get());
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_IDLE)) {
        ALOGE("setCamera called in an invalid state(%d)", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setCamera(camera, proxy);
 if (OK != ret) {
        ALOGV("setCamera failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
 return ret;
}
