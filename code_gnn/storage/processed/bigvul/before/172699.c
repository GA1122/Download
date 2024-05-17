status_t MediaRecorder::getMaxAmplitude(int* max)
{
    ALOGV("getMaxAmplitude");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (mCurrentState & MEDIA_RECORDER_ERROR) {
        ALOGE("getMaxAmplitude called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->getMaxAmplitude(max);
 if (OK != ret) {
        ALOGE("getMaxAmplitude failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
 return ret;
}
