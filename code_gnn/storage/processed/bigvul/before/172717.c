status_t MediaRecorder::setVideoEncoder(int ve)
{
    ALOGV("setVideoEncoder(%d)", ve);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!mIsVideoSourceSet) {
        ALOGE("try to set the video encoder without setting the video source first");
 return INVALID_OPERATION;
 }
 if (mIsVideoEncoderSet) {
        ALOGE("video encoder has already been set");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_DATASOURCE_CONFIGURED)) {
        ALOGE("setVideoEncoder called in an invalid state(%d)", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setVideoEncoder(ve);
 if (OK != ret) {
        ALOGV("setVideoEncoder failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mIsVideoEncoderSet = true;
 return ret;
}
