status_t MediaRecorder::setVideoFrameRate(int frames_per_second)
{
    ALOGV("setVideoFrameRate(%d)", frames_per_second);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_DATASOURCE_CONFIGURED)) {
        ALOGE("setVideoFrameRate called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }
 if (!mIsVideoSourceSet) {
        ALOGE("Cannot set video frame rate without setting video source first");
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setVideoFrameRate(frames_per_second);
 if (OK != ret) {
        ALOGE("setVideoFrameRate failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
 return ret;
}
