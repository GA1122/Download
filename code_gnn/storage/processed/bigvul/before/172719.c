status_t MediaRecorder::setVideoSize(int width, int height)
{
    ALOGV("setVideoSize(%d, %d)", width, height);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_DATASOURCE_CONFIGURED)) {
        ALOGE("setVideoSize called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }
 if (!mIsVideoSourceSet) {
        ALOGE("Cannot set video size without setting video source first");
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setVideoSize(width, height);
 if (OK != ret) {
        ALOGE("setVideoSize failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }

 return ret;
}
