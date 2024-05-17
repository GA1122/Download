status_t MediaRecorder::setOutputFile(int fd, int64_t offset, int64_t length)
{
    ALOGV("setOutputFile(%d, %" PRId64 ", %" PRId64 ")", fd, offset, length);
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }
 if (mIsOutputFileSet) {
        ALOGE("output file has already been set");
 return INVALID_OPERATION;
 }
 if (!(mCurrentState & MEDIA_RECORDER_DATASOURCE_CONFIGURED)) {
        ALOGE("setOutputFile called in an invalid state(%d)", mCurrentState);
 return INVALID_OPERATION;
 }

 if (fd < 0) {
        ALOGE("Invalid file descriptor: %d", fd);
 return BAD_VALUE;
 }

 status_t ret = mMediaRecorder->setOutputFile(fd, offset, length);
 if (OK != ret) {
        ALOGV("setOutputFile failed: %d", ret);
        mCurrentState = MEDIA_RECORDER_ERROR;
 return ret;
 }
    mIsOutputFileSet = true;
 return ret;
}
