status_t MediaRecorder::setClientName(const String16& clientName)
{
    ALOGV("setClientName");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 bool isInvalidState = (mCurrentState &
 (MEDIA_RECORDER_PREPARED |
                            MEDIA_RECORDER_RECORDING |
                            MEDIA_RECORDER_ERROR));
 if (isInvalidState) {
        ALOGE("setClientName is called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }

    mMediaRecorder->setClientName(clientName);

 return NO_ERROR;
}
