status_t MediaRecorder::release()
{
    ALOGV("release");
 if (mMediaRecorder != NULL) {
 return mMediaRecorder->release();
 }
 return INVALID_OPERATION;
}
