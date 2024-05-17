status_t MediaRecorder::reset()
{
    ALOGV("reset");
 if (mMediaRecorder == NULL) {
        ALOGE("media recorder is not initialized yet");
 return INVALID_OPERATION;
 }

    doCleanUp();
 status_t ret = UNKNOWN_ERROR;
 switch (mCurrentState) {
 case MEDIA_RECORDER_IDLE:
            ret = OK;
 break;

 case MEDIA_RECORDER_RECORDING:
 case MEDIA_RECORDER_DATASOURCE_CONFIGURED:
 case MEDIA_RECORDER_PREPARED:
 case MEDIA_RECORDER_ERROR: {
            ret = doReset();
 if (OK != ret) {
 return ret;  
 }
 }  
 case MEDIA_RECORDER_INITIALIZED:
            ret = close();
 break;

 default: {
            ALOGE("Unexpected non-existing state: %d", mCurrentState);
 break;
 }
 }
 return ret;
}
