status_t MediaRecorder::initCheck()
{
 return mMediaRecorder != 0 ? NO_ERROR : NO_INIT;
}
