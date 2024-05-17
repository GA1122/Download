status_t MediaRecorder::setListener(const sp<MediaRecorderListener>& listener)
{
    ALOGV("setListener");
 Mutex::Autolock _l(mLock);
    mListener = listener;

 return NO_ERROR;
}
