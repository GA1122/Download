status_t MediaRecorder::close()
{
    ALOGV("close");
 if (!(mCurrentState & MEDIA_RECORDER_INITIALIZED)) {
        ALOGE("close called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }
 status_t ret = mMediaRecorder->close();
 if (OK != ret) {
        ALOGE("close failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return UNKNOWN_ERROR;
 } else {
        mCurrentState = MEDIA_RECORDER_IDLE;
 }
 return ret;
}
