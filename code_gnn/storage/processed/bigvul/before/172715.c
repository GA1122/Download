status_t MediaRecorder::setParameters(const String8& params) {
    ALOGV("setParameters(%s)", params.string());
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }

 bool isInvalidState = (mCurrentState &
 (MEDIA_RECORDER_PREPARED |
                            MEDIA_RECORDER_RECORDING |
                            MEDIA_RECORDER_ERROR));
 if (isInvalidState) {
        ALOGE("setParameters is called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }

 status_t ret = mMediaRecorder->setParameters(params);
 if (OK != ret) {
        ALOGE("setParameters(%s) failed: %d", params.string(), ret);
 }

 return ret;
}
