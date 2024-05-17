status_t MediaRecorder::setOutputFormat(int of)
{
    ALOGV("setOutputFormat(%d)", of);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_INITIALIZED)) {
        ALOGE("setOutputFormat called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }
 if (mIsVideoSourceSet
 && of >= OUTPUT_FORMAT_AUDIO_ONLY_START  
 && of < OUTPUT_FORMAT_AUDIO_ONLY_END) {
        ALOGE("output format (%d) is meant for audio recording only"
 " and incompatible with video recording", of);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setOutputFormat(of);
 if (OK != ret) {
        ALOGE("setOutputFormat failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mCurrentState = MEDIA_RECORDER_DATASOURCE_CONFIGURED;
 return ret;
}
