status_t MediaRecorder::setInputSurface(const sp<PersistentSurface>& surface)
{
    ALOGV("setInputSurface");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 bool isInvalidState = (mCurrentState &
 (MEDIA_RECORDER_PREPARED |
                            MEDIA_RECORDER_RECORDING));
 if (isInvalidState) {
        ALOGE("setInputSurface is called in an invalid state: %d", mCurrentState);
 return INVALID_OPERATION;
 }

 return mMediaRecorder->setInputSurface(surface->getBufferConsumer());
}
