status_t MediaRecorder::init()
{
    ALOGV("init");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_IDLE)) {
        ALOGE("init called in an invalid state(%d)", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->init();
 if (OK != ret) {
        ALOGV("init failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }

    ret = mMediaRecorder->setListener(this);
 if (OK != ret) {
        ALOGV("setListener failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }

    mCurrentState = MEDIA_RECORDER_INITIALIZED;
 return ret;
}
