status_t MediaRecorder::start()
{
    ALOGV("start");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_PREPARED)) {
        ALOGE("start called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->start();
 if (OK != ret) {
        ALOGE("start failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mCurrentState = MEDIA_RECORDER_RECORDING;
 return ret;
}
