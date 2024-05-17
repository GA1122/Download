status_t MediaPlayer::prepare()
{
    ALOGV("prepare");
 Mutex::Autolock _l(mLock);
    mLockThreadId = getThreadId();
 if (mPrepareSync) {
        mLockThreadId = 0;
 return -EALREADY;
 }
    mPrepareSync = true;
 status_t ret = prepareAsync_l();
 if (ret != NO_ERROR) {
        mLockThreadId = 0;
 return ret;
 }

 if (mPrepareSync) {
        mSignal.wait(mLock);  
        mPrepareSync = false;
 }
    ALOGV("prepare complete - status=%d", mPrepareStatus);
    mLockThreadId = 0;
 return mPrepareStatus;
}
