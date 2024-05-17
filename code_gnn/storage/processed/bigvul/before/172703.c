status_t MediaRecorder::prepare()
{
    ALOGV("prepare");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_DATASOURCE_CONFIGURED)) {
        ALOGE("prepare called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }
 if (mIsAudioSourceSet != mIsAudioEncoderSet) {
 if (mIsAudioSourceSet) {
            ALOGE("audio source is set, but audio encoder is not set");
 } else {  
            ALOGE("audio encoder is set, but audio source is not set");
 }
 return INVALID_OPERATION;
 }

 if (mIsVideoSourceSet != mIsVideoEncoderSet) {
 if (mIsVideoSourceSet) {
            ALOGE("video source is set, but video encoder is not set");
 } else {  
            ALOGE("video encoder is set, but video source is not set");
 }
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->prepare();
 if (OK != ret) {
        ALOGE("prepare failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mCurrentState = MEDIA_RECORDER_PREPARED;
 return ret;
}
