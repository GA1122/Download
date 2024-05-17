status_t MediaRecorder::doReset()
{
    ALOGV("doReset");
 status_t ret = mMediaRecorder->reset();
 if (OK != ret) {
        ALOGE("doReset failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 } else {
        mCurrentState = MEDIA_RECORDER_INITIALIZED;
 }
 return ret;
}
