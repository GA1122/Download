bool Camera3Device::PreparerThread::threadLoop() {
 status_t res;
 {
 Mutex::Autolock l(mLock);
 if (mCurrentStream == nullptr) {
 if (mPendingStreams.empty()) {
                ALOGV("%s: Preparer stream out of work", __FUNCTION__);
                mActive = false;
 return false;
 }

 auto it = mPendingStreams.begin();
            mCurrentStream = *it;
            mPendingStreams.erase(it);
            ATRACE_ASYNC_BEGIN("stream prepare", mCurrentStream->getId());
            ALOGV("%s: Preparing stream %d", __FUNCTION__, mCurrentStream->getId());
 } else if (mCancelNow) {
            mCurrentStream->cancelPrepare();
            ATRACE_ASYNC_END("stream prepare", mCurrentStream->getId());
            ALOGV("%s: Cancelling stream %d prepare", __FUNCTION__, mCurrentStream->getId());
            mCurrentStream.clear();
            mCancelNow = false;
 return true;
 }
 }

    res = mCurrentStream->prepareNextBuffer();
 if (res == NOT_ENOUGH_DATA) return true;
 if (res != OK) {
        ALOGE("%s: Stream %d returned error %d (%s) during prepare", __FUNCTION__,
                mCurrentStream->getId(), res, strerror(-res));
        mCurrentStream->cancelPrepare();
 }

 Mutex::Autolock l(mLock);
 if (mListener) {
        ALOGV("%s: Stream %d prepare done, signaling listener", __FUNCTION__,
                mCurrentStream->getId());
        mListener->notifyPrepared(mCurrentStream->getId());
 }

    ATRACE_ASYNC_END("stream prepare", mCurrentStream->getId());
    mCurrentStream.clear();

 return true;
}
