status_t MediaRecorder::setAudioSource(int as)
{
    ALOGV("setAudioSource(%d)", as);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (mCurrentState & MEDIA_RECORDER_IDLE) {
        ALOGV("Call init() since the media recorder is not initialized yet");
 status_t ret = init();
 if (OK != ret) {
 return ret;
 }
 }
 if (mIsAudioSourceSet) {
        ALOGE("audio source has already been set");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_INITIALIZED)) {
        ALOGE("setAudioSource called in an invalid state(%d)", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setAudioSource(as);
 if (OK != ret) {
        ALOGV("setAudioSource failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mIsAudioSourceSet = true;
 return ret;
}
